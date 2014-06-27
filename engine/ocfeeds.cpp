#include <QtNetwork>
#include "ocfeeds.h"


/*!
 * \class OcFeeds
 * \brief The OcFeeds class provides a network interface to the ownCloud News App feeds
 *
 * This class provides an interface to the API functions that operates on feeds, like
 * adding, removing and moving.
*/

OcFeeds::OcFeeds(QObject *parent) :
    QObject(parent)
{
    addFeedToEventView = false;
    folderIdToMoveTo = "0";
    newFeedName = "";
}




/*!
 * \fn void OcFeeds::requestFeeds()
 * \brief Request the feeds from the API
 *
 * This function sends a get operation to the ownCloud News API to request the available feeds.
 */

void OcFeeds::requestFeeds()
{
    if (network.isFlightMode())
    {
        emit requestedFeedsError(tr("Device is in flight mode."));
    } else {
#ifdef QT_DEBUG
        qDebug() << "Start to fetch feeds from server.";
#endif

        replyRequestFeeds = network.get(helper.buildRequest("feeds"));

        connect(replyRequestFeeds,SIGNAL(finished()),this,SLOT(feedsRequested()));
    }
}



/*!
 * \fn void OcFeeds::feedsRequested()
 * \brief Handles the network reply of requestFeeds()
 *
 * This internal function handles the network reply of requestFeeds(). If it was successfull
 * it sends the result to feedsRequestedUpdateDb, otherwise it emits the signal requestedFeedsError().
 */

void OcFeeds::feedsRequested()
{

#ifdef QT_DEBUG
    qDebug() << "Requested Feeds from Server";
#endif

    if (replyRequestFeeds->error() == QNetworkReply::NoError)
    {
        QVariantMap feedsresult = helper.jsonToQt(replyRequestFeeds->readAll());

        replyRequestFeeds->deleteLater();

        if (feedsresult.isEmpty())
        {
            notify.showNotification(tr("Server reply was empty."), tr("Failed to request feeds"), OcNotifications::Error);
            emit requestedFeedsError(tr("Server reply was empty."));
            return;
        }

        feedsRequestedUpdateDb(feedsresult);

    } else {

#ifdef QT_DEBUG
        qDebug() << "HTTP-Error:" << replyRequestFeeds->errorString();
#endif
        notify.showNotification(replyRequestFeeds->errorString(), tr("Failed to request feeds"), OcNotifications::Error);
        emit requestedFeedsError(replyRequestFeeds->errorString());
        replyRequestFeeds->deleteLater();
    }

}



/*!
 * \fn void OcFeeds::feedsRequestedUpdateDb(QVariantMap feedsresult)
 * \brief Updates the database with the result of a successful requestFeeds()
 *
 * This internal function updates the database with the result of a successlful
 * requestFeeds().
 *
 * \param feedsresult       QVariantMap given from feedsRequested()
 */

void OcFeeds::feedsRequestedUpdateDb(const QVariantMap &feedsresult)
{
#ifdef QT_DEBUG
    qDebug() << "Start updating feeds database";
#endif
    QSqlQuery query;
    QStringList newFeeds = QStringList();

    QList<int> newFeedIds = QList<int>();
    QList<int> updatedFeeds = QList<int>();

    foreach (QVariant feed, feedsresult["feeds"].toList())
    {

        // remap QVariantMap
        QMap<QString, QVariant> map = feed.value<QVariantMap>();

        // check if feed is already in database
        query.exec(QString("SELECT title, faviconLink, folderId, unreadCount, link FROM feeds WHERE id = %1").arg(map["id"].toInt()));
        if (query.next())
        {
            bool updated = false;

            // extract data from the last query
            QString title = query.value(0).toString();
            QString faviconLink = query.value(1).toString();
            int folderId = query.value(2).toInt();
            int unreadCount = query.value(3).toInt();
            QString link = query.value(4).toString();

            // check if feed has changed it's title
            if (title != map["title"].toString())
            {
                query.prepare("UPDATE feeds SET title = :title WHERE id = :id;");
                query.bindValue(":title", map["title"].toString());
                query.bindValue(":id", map["id"].toInt());
                query.exec();

                updated = true;
            }

            // check if feed has changed it's favicon link
            if (faviconLink != map["faviconLink"].toString())
            {
                query.prepare("UPDATE feeds SET faviconLink = :faviconLink WHERE id = :id;");
                query.bindValue(":faviconLink", map["faviconLink"].toString());
                query.bindValue(":id", map["id"].toInt());
                query.exec();
                getFavicon( map["id"].toString(), map["faviconLink"].toString());

                updated = true;
            }

            // check if feed has changed it's folder id
            if (folderId != map["folderId"].toInt())
            {
                query.prepare("UPDATE feeds SET folderId = :folderId WHERE id = :id;");
                query.bindValue(":folderId", map["folderId"].toInt());
                query.bindValue(":id", map["id"].toInt());
                query.exec();

                updated = true;
            }

            // check if feed has changed it's unread count
            if (unreadCount != map["unreadCount"].toInt())
            {
                query.prepare("UPDATE feeds SET unreadCount = :unreadCount WHERE id = :id;");
                query.bindValue(":unreadCount", map["unreadCount"].toInt());
                query.bindValue(":id", map["id"].toInt());
                query.exec();
            }

            // check if feed has changed it's link
            if (link != map["link"].toString())
            {
                query.prepare("UPDATE feeds SET link = :link WHERE id = :id;");
                query.bindValue(":link", map["link"].toString());
                query.bindValue(":id", map["id"].toInt());
                query.exec();

                updated = true;
            }

            if (updated)
                updatedFeeds << map["id"].toInt();

        } else { // if feed is not in database, create it there
            query.prepare("INSERT INTO feeds (id, url, title, faviconLink, added, folderId, unreadCount, link) VALUES (:id, :url, :tit, :fav, :add, :fol, :unr, :lin);");
            query.bindValue(":id", map["id"].toInt());
            query.bindValue(":url", map["url"].toString());
            query.bindValue(":tit", map["title"].toString());
            query.bindValue(":fav", map["faviconLink"].toString());
            query.bindValue(":add", map["added"].toInt());
            query.bindValue(":fol", map["folderId"].toInt());
            query.bindValue(":unr", map["unreadCount"].toInt());
            query.bindValue(":lin", map["link"].toString());
            query.exec();
#ifdef QT_DEBUG
            qDebug() << "Added Feed: " << map["title"].toString();
#endif
            newFeeds << map["title"].toString();
            newFeedIds << map["id"].toInt();

            getFavicon( map["id"].toString(), map["faviconLink"].toString());
        }


    }

    // now check if feeds were deleted on server
    QList<int> idList = QList<int>();
    QList<int> idListDeleted = QList<int>();
    QStringList deletedFeedNames;

    // put all the ids into a list
    foreach (QVariant feed, feedsresult["feeds"].toList())
    {
        QMap<QString, QVariant> map = feed.value<QVariantMap>();

        idList << map["id"].toInt();
    }

    // compare the ids and put the deleted ids into a list
    query.exec(QString("SELECT id, title FROM feeds;"));
    while (query.next())
    {
        if (!idList.contains(query.value(0).toInt())) {
            idListDeleted << query.value(0).toInt();
            deletedFeedNames << query.value(1).toString();
        }
    }

    if (!idListDeleted.isEmpty()) {

        QString ftdList("(");

        for (int i = 0; i < idListDeleted.size(); ++i)
        {
            ftdList.append(QString::number(idListDeleted.at(i)));
            ftdList.append(", ");
        }

        ftdList.chop(2);
        ftdList.append(")");

        query.exec(QString("DELETE FROM feeds WHERE id IN %1").arg(ftdList));
    }


    if ((!newFeeds.isEmpty() || !idListDeleted.isEmpty()) && config.getSetting(QString("notifications/feedsFolders"), QDBusVariant(false)).variant().toBool())
    {
        QString summary = "";
        QString body = "";

        if (!newFeeds.isEmpty()) {
            summary = tr("%n feed(s) added", "", newFeeds.count());
            body = tr("Added:").append(" ");
            body.append(newFeeds.join(", "));
        }

        if (summary != "") {
            summary.append(", ");
            body.append("; ");
        }

        if (!deletedFeedNames.isEmpty())
        {
            summary.append(tr("%n feed(s) removed", "", deletedFeedNames.count()));
            body.append(tr("Removed:")).append(" ");
            body.append(deletedFeedNames.join(", "));
        }

        notify.showNotification(body, summary);

    }


#ifdef QT_DEBUG
    qDebug() << "Emit requestedFeedsSuccess signal";
#endif
    emit requestedFeedsSuccess(updatedFeeds, newFeedIds, idListDeleted);
}




/*!
 * \fn void OcFeeds::createFeed(const QString &url, const QString &folderId, bool eventView)
 * \brief Create a new feed
 *
 * This function sends a post operation to the ownCloud News API to create a new feed, the
 * answer will be set to feedCreated().
 *
 * \param url           the URL of the feed
 * \param folderId      the ID of the folder the feed should be added to
 * \param eventView     set to true if the feed should be shown in the event view
 */

void OcFeeds::createFeed(const QString &url, const QString &folderId, const bool &eventView)
{
    if (network.isFlightMode())
    {
        emit createdFeedError(tr("Device is in flight mode."));
    } else {
        QString t_folderId = folderId;
        addFeedToEventView = eventView;

        QString t_url = url;

        if (!t_url.contains(QRegExp("^(http:\\/\\/\\w|https:\\/\\/\\w)")))
            t_url.prepend("http://");

        // Create the JSON string
        QByteArray parameters("{\"url\": \"");
        parameters.append(t_url);
        parameters.append("\", \"folderId\": ");
        parameters.append(t_folderId.replace(QString("/"), QString("\\/")));
        parameters.append("}");

        replyCreateFeed = network.post(helper.buildRequest("feeds", parameters.size()), parameters);

        connect(replyCreateFeed, SIGNAL(finished()), this, SLOT(feedCreated()));
    }
}




/*!
 * \fn void OcFeeds::feedCreated(int eventView)
 * \brief Handles the network reply of createFeed()
 *
 * This internal function handles the reply of the createFeed() network operation. If it was successful
 * the result will be given to feedCreatedUpdateDb(), otherwise the signal createdFeedError() will
 * be emitted.
 */

void OcFeeds::feedCreated()
{
    if (replyCreateFeed->error() == QNetworkReply::NoError)
    {
        QVariantMap createFeedResult = helper.jsonToQt(replyCreateFeed->readAll());

        replyCreateFeed->deleteLater();

        if (createFeedResult.isEmpty())
        {
            notify.showNotification(tr("Server reply was empty."), tr("Failed to add feed"), OcNotifications::Error);
            emit createdFeedError(tr("Server reply was empty."));
            return;
        }

        feedCreatedUpdateDb(createFeedResult);

    } else {
        QVariantMap createFeedResult = helper.jsonToQt(replyCreateFeed->readAll());

        QString createFeedResultError = createFeedResult["message"].toString();

        if (createFeedResultError.isEmpty())
            createFeedResultError = replyCreateFeed->errorString();

        replyCreateFeed->deleteLater();

        notify.showNotification(createFeedResultError, tr("Failed to add feed"), OcNotifications::Error);

        emit createdFeedError(createFeedResultError);

    }
}




/*!
 * \fn void OcFeeds::feedCreatedUpdateDb(QVariantMap createFeedResult)
 * \brief Updates the database with the result of createFeed()
 *
 * This internal function updats the database with the result of the post network operation
 * initiated by createFeed.
 *
 * \param createFeedResult      the result of the createFeedResult() post operation
 */

void OcFeeds::feedCreatedUpdateDb(const QVariantMap &createFeedResult)
{
    QSqlQuery query;
    foreach (QVariant feed, createFeedResult["feeds"].toList())
    {
        QMap<QString, QVariant> map = feed.value<QVariantMap>();
        query.prepare("INSERT INTO feeds (id, url, title, faviconLink, added, folderId, unreadCount, link) VALUES (:id, :url, :tit, :fav, :add, :fol, :unr, :lin);");
        query.bindValue(":id", map["id"].toInt());
        query.bindValue(":url", map["url"].toString());
        query.bindValue(":tit", map["title"].toString());
        query.bindValue(":fav", map["faviconLink"].toString());
        query.bindValue(":add", map["added"].toInt());
        query.bindValue(":fol", map["folderId"].toInt());
        query.bindValue(":unr", map["unreadCount"].toInt());
        query.bindValue(":lin", map["link"].toString());
        query.exec();
        getFavicon( map["id"].toString(), map["faviconLink"].toString());

        // check if the feed should be shown in the event view
        if (addFeedToEventView)
        {
            QString feedsForEventView = config.getSetting(QString("event/feeds"), QDBusVariant("")).variant().toString();
            feedsForEventView.append(",").append(map["id"].toString());
            config.setSetting(QString("event/feeds"), QDBusVariant(feedsForEventView));
        }

        addFeedToEventView = false;

        emit feedCreatedFetchItems("100", "0", "0", map["id"].toString(), "true");
        emit createdFeedSuccess(map["title"].toString(), map["id"].toInt());
    }
}





/*!
 * \fn void OcFeeds::deleteFeed(const QString &id)
 * \brief Deletes a feed
 *
 * This function sends a delete operation to the ownCloud News API to delete a feed,
 * it sends the result to the feedDelted() function.
 *
 * \param id        ID of the feed to delete
 */

void OcFeeds::deleteFeed(const QString &id)
{
    if (network.isFlightMode())
    {
        emit deletedFeedError(tr("Device is in flight mode."));
    } else {
        // Create the API URL
        QString feed = "feeds/";
        feed.append(id);

        replyDeleteFeed = network.deleteResource(helper.buildRequest(feed));

        connect(replyDeleteFeed,SIGNAL(finished()),this,SLOT(feedDeleted()));
    }
}




/*!
 * \fn void OcFeeds::feedDeleted()
 * \brief Handles the reply of delteFeed()
 *
 * This internal function handles the reply of the network delete request performed by deleteFeed().
 * If successfull it gives the answer to feedDeletedUpdateDb(), otherwise it emits the signal deletedFeedError().
 */

void OcFeeds::feedDeleted()
{
    if (replyDeleteFeed->error() == QNetworkReply::NoError)
    {
        int id = replyDeleteFeed->url().toString().split("/").last().toInt();

        replyDeleteFeed->deleteLater();

        feedDeletedUpdateDb(id);

    } else {
        QVariantMap deleteFeedResult;
        deleteFeedResult = helper.jsonToQt(replyDeleteFeed->readAll());

        QString deleteFeedResultError = deleteFeedResult["message"].toString();

        if (deleteFeedResultError.isEmpty())
            deleteFeedResultError = replyDeleteFeed->errorString();

        replyDeleteFeed->deleteLater();

#ifdef QT_DEBUG
        qDebug() << deleteFeedResultError;
#endif

        notify.showNotification(deleteFeedResultError, tr("Failed to delete feed"), OcNotifications::Error);

        emit deletedFeedError(deleteFeedResultError);
    }
}




/*!
 * \fn void OcFeeds::feedDeletedUpdateDb(int id)
 * \brief Updates the database after a successfull feed deletion
 *
 * This internal function updates the database after a successful deletion of a feed
 * on the server.
 *
 * \param id        feed ID to delete
 */

void OcFeeds::feedDeletedUpdateDb(const int &id)
{
    QSqlQuery query;

    query.exec(QString("DELETE FROM feeds WHERE id = %1").arg(id));

    // remove id from the list of the feeds for the event view
    QString feedsForEventView = config.getSetting(QString("event/feeds"), QDBusVariant("")).variant().toString();
    QStringList feedsForEventsList = feedsForEventView.split(",");

    feedsForEventsList.removeOne(QString::number(id));
    feedsForEventView = feedsForEventsList.join(",");

    config.setSetting(QString("event/feeds"), QDBusVariant(feedsForEventView));

//    items.cleanItems(id);

    emit deletedFeedSuccess(id);
}




/*!
 * \fn void OcFeeds::feedDeletedCleanItems(int id)
 * \brief Deletes items of a deleted feed
 *
 * This internal functions deletes the items of a deleted feed in the database.
 *
 * \param id    ID of the the feed
 */

void OcFeeds::feedDeletedCleanItems(int id)
{
//    items.cleanItems(id);
}




/*!
 * \fn void OcFeeds::moveFeed(const QString &id, const QString &folderId)
 * \brief Moves a feed to a different folder
 *
 * This function sends a put request to the ownCloud News API to move a feed
 * to a different folder. The result is sent to feedMoved().
 *
 * \param id            ID of the feed to move
 * \param folderId      ID of the target folder
 */

void OcFeeds::moveFeed(const QString &id, const QString &folderId)
{
    if (network.isFlightMode())
    {
        emit movedFeedError(tr("Device is in flight mode."));
    } else {

        QSqlQuery query;

        query.exec(QString("SELECT folderId FROM feeds WHERE id = %1").arg(id.toInt()));

        query.next();

        if (query.value(0).toString() == folderId) {
            emit movedFeedError(tr("Feed is already part of the destination folder"));
            return;
        }

        // Create the API URL
        QString feed = "feeds/";
        feed.append(id);
        feed.append("/move");

        // Create the JSON string
        QByteArray parameters("{\"folderId\": ");
        parameters.append(folderId);
        parameters.append("}");

        folderIdToMoveTo = folderId;

        replyMoveFeed = network.put(helper.buildRequest(feed, parameters.length()), parameters);

        connect(replyMoveFeed, SIGNAL(finished()), this, SLOT(feedMoved()));
    }
}



/*!
 * \fn void OcFeeds::feedMoved(QString folderId)
 * \brief Handles the network reply of moveFeed()
 *
 * This internal function handles the reply of the network put operation to move
 * a feed to a different folder (executed in moveFeed()). If successful it emits
 * the signal movedFeed which is handled by feedMovedUpdateDb, otherwise it emits
 * the signal movedFeedError.
 */

void OcFeeds::feedMoved()
{
    if (replyMoveFeed->error() == QNetworkReply::NoError)
    {
        // extract feed id out of URL
        QString url = replyMoveFeed->url().toString();
        QStringList urlParts = url.split("/");
        urlParts.removeLast();
        int id = urlParts.last().toInt();


        replyMoveFeed->deleteLater();

        feedMovedUpdateDb(id, folderIdToMoveTo);

    } else {
        QVariantMap moveresult;
        moveresult = helper.jsonToQt(replyMoveFeed->readAll());

        QString moveFeedResultError = moveresult["message"].toString();

        if (moveFeedResultError.isEmpty())
            moveFeedResultError = replyMoveFeed->errorString();

        replyMoveFeed->deleteLater();

#ifdef QT_DEBUG
        qDebug() << moveFeedResultError;
#endif
        notify.showNotification(moveFeedResultError, tr("Failed to move feed"), OcNotifications::Error);

        emit movedFeedError(moveFeedResultError);
    }
}



/*!
 * \fn void OcFeeds::feedMovedUpdateDb(int id, QString folderId)
 * \brief Updates the database after successfully moving a feed
 *
 * This internal function is called by feedMoved() after a feed was successfully moved
 * to a different folder.
 *
 * \param id            ID of the feed
 * \param folderId      ID of the target folder
 */

void OcFeeds::feedMovedUpdateDb(const int &id, const QString &folderId)
{
    QSqlQuery query;

    query.prepare("UPDATE feeds SET folderId = :folderId WHERE id = :id;");
    query.bindValue(":folderId", folderId);
    query.bindValue(":id", id);
    query.exec();

    emit movedFeedSuccess(id, folderId.toInt());
}




/*!
 * \fn void OcFeeds::getFavicon(QString feedId, QString faviconLink)
 * \brief Get the Favicon of a feed
 *
 * This internal function gets a Favicon of a feed and stores it on the device.
 * It analyses the Favicon image and stores its path and some metadata in the database.
 *
 * \param feedId        ID of the feed
 * \param faviconLink   URL of the Favicon
 */

void OcFeeds::getFavicon(QString feedId, QString faviconLink)
{
    // Building the request
    QNetworkRequest request(faviconLink);

    replyGetFavicon = network.get(request);

    QEventLoop dlLoop;
    connect(replyGetFavicon, SIGNAL(finished()), &dlLoop, SLOT(quit()));
    dlLoop.exec();

    if (replyGetFavicon->error() == QNetworkReply::NoError)
    {
        QUrl fileUrl(faviconLink);
        QFileInfo fileInfo = fileUrl.path();

        QString fileName(feedId);
        fileName += "_";
        fileName += fileInfo.fileName();

        QString storagePath(QDir::homePath());
        storagePath.append(BASE_PATH).append("/favicons");
        storagePath.append(QDir::separator()).append(fileName);
        storagePath = QDir::toNativeSeparators(storagePath);
        QImage favicon;
        if(favicon.loadFromData(replyGetFavicon->readAll()))
        {
            if(favicon.save(storagePath))
            {
                QSqlQuery query;

                query.prepare("UPDATE feeds SET iconSource = :source, iconWidth = :width, iconHeight = :height WHERE id = :id;");
                query.bindValue(":source", fileName);
                query.bindValue(":width", favicon.width());
                query.bindValue(":height", favicon.height());
                query.bindValue(":id", feedId.toInt());
                query.exec();
            }
            else
            {
                qDebug() << "Failed to save icon";
            }
        }
        else
        {
            qDebug() << "Favicon can not be load";
        }
    }

    replyGetFavicon->deleteLater();
}




/*!
 * \fn void OcFeeds::markFeedRead(const QString &feedId)
 * \brief Marks all items of a feed as read
 *
 * This function sends a put request to the ownCloud News API to mark all
 * items of a feed as read. It is connected to feedMarkedRead() to handle
 * the result of the request.
 *
 * \param feedId    ID of the feed that's items should be marked as read
 */

void OcFeeds::markFeedRead(const QString &feedId)
{
    if (network.isFlightMode())
    {
        emit markedReadFeedError(tr("Device is in flight mode."));
    } else {
        // Create API URL
        QString feed = "feeds/";
        feed.append(feedId).append("/read");

        // Determine newest item ID
        QSqlQuery query;
        QString newestItemId;
        query.exec(QString("SELECT MAX(id) FROM items WHERE feedId = %1;").arg(feedId.toInt()));
        if (query.next())
            newestItemId = query.value(0).toString();

#ifdef QT_DEBUG
        qDebug() << __func__ << "- newest itemId: " << newestItemId;
#endif

        // Create JSON String
        QByteArray parameters("{\"newestItemId\": ");
        parameters.append(newestItemId);
        parameters.append("}");

#ifdef QT_DEBUG
        qDebug() << __func__ << "- parameters: " << parameters;
#endif

        replyMarkFeedRead = network.put(helper.buildRequest(feed, parameters.length()), parameters);

        connect(replyMarkFeedRead,SIGNAL(finished()),this,SLOT(feedMarkedRead()));

    }
}




/*!
 * \fn void OcFeeds::feedMarkedRead()
 * \brief Handles the network reply of markFeedRead()
 *
 * This internal function handles the network reply of the put operation in markFeedRead().
 * If successfull it calls feedMarkedReadUpdateDb(), otherwise it emits the signal markedReadFeedError().
 */

void OcFeeds::feedMarkedRead()
{
    if (replyMarkFeedRead->error() == QNetworkReply::NoError)
    {
        // extract feed id out of URL
        QStringList urlParts = replyMarkFeedRead->url().toString().split("/");
        urlParts.removeLast();
        int id = urlParts.last().toInt();

#ifdef QT_DEBUG
        qDebug() << __func__ << "- feedId: " << id;
#endif

        replyMarkFeedRead->deleteLater();

        feedMarkedReadUpdateDb(id);

    } else {
        QVariantMap markFeedReadResult = helper.jsonToQt(replyMarkFeedRead->readAll());

        QString markedReadFeedErrorResult = markFeedReadResult["message"].toString();

        if (markedReadFeedErrorResult.isEmpty())
            markedReadFeedErrorResult = replyMarkFeedRead->errorString();

        notify.showNotification(markedReadFeedErrorResult, tr("Failed to mark feed as read"), OcNotifications::Error);

        emit markedReadFeedError(markedReadFeedErrorResult);

        replyMarkFeedRead->deleteLater();
    }
}




/*!
 * \fn void OcFeeds::feedMarkedReadUpdateDb(int id)
 * \brief Updates database after marking a feed as read
 *
 * This internal function updates the database after a feed was successfully
 * marked as read. It updates the items in the database that belong to this feed.
 *
 * \param id    ID of the feed that was marked as read
 */

void OcFeeds::feedMarkedReadUpdateDb(const int &id)
{
    QDateTime ts;
    QSqlQuery query;
    QSqlDatabase::database().transaction();
    query.exec(QString("UPDATE items SET unread = %3, lastModified = %2 WHERE unread = %4 AND feedId = %1").arg(id).arg(ts.currentDateTimeUtc().toTime_t()).arg(SQL_FALSE).arg(SQL_TRUE));
    QSqlDatabase::database().commit();

    emit markedReadFeedSuccess(id);
}


/*!
 * \fn QVariantMap OcFeeds::getFeeds()
 * \brief Get IDs and titles of all feeds
 *
 * This function returns ID and title of all feeds in the database
 *
 * \return IDs and titles in a QVariantMap
 */

QVariantMap OcFeeds::getFeeds()
{
    QSqlQuery query;
    QVariantMap feeds;
    query.exec(QString("SELECT id, title FROM feeds ORDER BY title COLLATE NOCASE ASC;"));
    while (query.next())
        feeds[query.value(1).toString()] = query.value(0).toString();

    return feeds;
}



/*!
 * \fn void OcFeeds::renameFeed(const QString &id, const QString &newName)
 * \brief Renames a feed
 *
 * Renames a feed on the server and on success also in the local database
 *
 * \param id ID of the feed to rename
 * \param newName New name of the feed
 */
void OcFeeds::renameFeed(const QString &id, const QString &newName)
{
    if (network.isFlightMode())
    {
        emit renamedFeedError(tr("Device is in flight mode."));
    } else {
        // Create the API URL
        QString feed = "feeds/";
        feed.append(id);
        feed.append("/rename");

        // Create the JSON string
        QByteArray parameters("{\"feedTitle\": \"");
        parameters.append(newName);
        parameters.append("\"}");

        newFeedName = newName;

        replyRenameFeed = network.put(helper.buildRequest(feed, parameters.length()), parameters);

        connect(replyRenameFeed, SIGNAL(finished()), this, SLOT(feedRenamed()));
    }
}


/*!
 * \fn OcFeeds::feedRenamed(QString name)
 * \brief Handles network reply for feed rename
 *
 * This internal function handles the network reply for the feed rename request.
 */
void OcFeeds::feedRenamed()
{
    if (replyRenameFeed->error() == QNetworkReply::NoError)
    {
        // extract folder id out of URL
        QString url = replyRenameFeed->url().toString();
        QStringList urlParts = url.split("/");
        urlParts.removeLast();
        int id = urlParts.last().toInt();

        replyRenameFeed->deleteLater();

        feedRenamedUpdateDb(id, newFeedName);

    } else {
        QVariantMap renameresult = helper.jsonToQt(replyRenameFeed->readAll());

        QString renameresulterror = renameresult["message"].toString();

        if (renameresulterror.isEmpty())
            renameresulterror = replyRenameFeed->errorString();

        replyRenameFeed->deleteLater();
#ifdef QT_DEBUG
        qDebug() << renameresulterror;
#endif

        notify.showNotification(renameresulterror, tr("Failed to rename feed"));

        emit renamedFeedError(renameresulterror);
    }
}


/*!
 * \fn OcFeeds::feedRenamedUpdateDb(int id, const QString &name)
 * \brief Updates database after feeed renamed
 *
 * This internal functions updates the local database after changing the feeds name on the server.
 *
 * \param id ID of the feed to rename
 * \param name New name of the feed
 */
void OcFeeds::feedRenamedUpdateDb(const int &id, const QString &name)
{
    QSqlQuery query;

    query.prepare("UPDATE feeds SET title = :name WHERE id = :id;");
    query.bindValue(":name", name);
    query.bindValue(":id", id);
    query.exec();

    emit renamedFeedSuccess(name, id);
}
