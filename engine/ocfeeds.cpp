#include <QtNetwork>
#include "ocfeeds.h"
#include "../common/globals.h"


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

        urlRequestFeeds = helper.buildUrl("feeds");

        replyRequestFeeds = network.get(QNetworkRequest(QUrl(urlRequestFeeds)));

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
    connect(this,SIGNAL(requestedFeeds(QVariantMap)),this,SLOT(feedsRequestedUpdateDb(QVariantMap)), Qt::UniqueConnection);

    if (replyRequestFeeds->error() == QNetworkReply::NoError)
    {
        QVariantMap feedsresult = helper.jsonToQt(replyRequestFeeds->readAll());

        replyRequestFeeds->deleteLater();

        if (feedsresult.isEmpty())
        {
            emit requestedFeedsError(tr("Server reply was empty."));
        } else {
            emit requestedFeeds(feedsresult);
        }

    } else {

#ifdef QT_DEBUG
        qDebug() << "HTTP-Error:" << replyRequestFeeds->errorString();
#endif
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
 * \param feedsresult       QVariantMap sent from feedsRequested()
 */

void OcFeeds::feedsRequestedUpdateDb(QVariantMap feedsresult)
{
#ifdef QT_DEBUG
    qDebug() << "Start updating feeds database";
#endif
    QSqlQuery query;

    foreach (QVariant feed, feedsresult["feeds"].toList())
    {

        // remap QVariantMap
        QMap<QString, QVariant> map = feed.value<QVariantMap>();

        // check if folder is already in database
        query.exec(QString("SELECT title, faviconLink, folderId, unreadCount, link FROM feeds WHERE id = %1").arg(map["id"].toInt()));
        if (query.next())
        {
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
            }

            // check if feed has changed it's favicon link
            if (faviconLink != map["faviconLink"].toString())
            {
                query.prepare("UPDATE feeds SET faviconLink = :faviconLink WHERE id = :id;");
                query.bindValue(":faviconLink", map["faviconLink"].toString());
                query.bindValue(":id", map["id"].toInt());
                query.exec();
                getFavicon( map["id"].toString(), map["faviconLink"].toString());
            }

            // check if feed has changed it's folder id
            if (folderId != map["folderId"].toInt())
            {
                query.prepare("UPDATE feeds SET folderId = :folderId WHERE id = :id;");
                query.bindValue(":folderId", map["folderId"].toInt());
                query.bindValue(":id", map["id"].toInt());
                query.exec();
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
            }


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
            getFavicon( map["id"].toString(), map["faviconLink"].toString());
        }


    }

    // now check if feeds were deleted on server
    QList<int> idList;
    QList<int> idListDeleted;

    // put all the ids into a list
    foreach (QVariant feed, feedsresult["feeds"].toList())
    {
        QMap<QString, QVariant> map = feed.value<QVariantMap>();

        idList << map["id"].toInt();
    }

    // compare the ids and put the deleted ids into a list
    query.exec(QString("SELECT id FROM feeds;"));
    while (query.next())
    {
        if (!idList.contains(query.value(0).toInt()))
            idListDeleted << query.value(0).toInt();
    }

    // delete the serverside deleted ids in the database
//    QSqlDatabase::database().transaction();
    for (int i = 0; i < idListDeleted.size(); ++i) {
        query.exec(QString("DELETE FROM feeds WHERE id = %1").arg(idListDeleted.at(i)));
//        query.exec(QString("DELETE FROM items WHERE feedId = %1").arg(idListDeleted.at(i)));
        items.cleanItems(idListDeleted.at(i));
        qDebug() << "Deleted Feed ID: " << idListDeleted.at(i);
    }
//    QSqlDatabase::database().commit();

#ifdef QT_DEBUG
    qDebug() << "Emit requestedFeedsSuccess signal";
#endif
    emit requestedFeedsSuccess();
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

void OcFeeds::createFeed(const QString &url, const QString &folderId, bool eventView)
{
    if (network.isFlightMode())
    {
        emit createdFeedError(tr("Device is in flight mode."));
    } else {
        // Create the API URL
        urlCreateFeed = helper.buildUrl("feeds");

        QString t_folderId = folderId;

        // Create the JSON string
        QByteArray parameters("{\"url\": \"");
        parameters.append(url);
        parameters.append("\", \"folderId\": ");
        parameters.append(t_folderId.replace(QString("/"), QString("\\/")));
        parameters.append("}");

        // Calculate content length header
        QByteArray postDataSize = QByteArray::number(parameters.size());

        // Building the request
        QNetworkRequest request(urlCreateFeed);

        // Add the headers
        request.setRawHeader("Content-Type", "application/json; charset=utf-8");
        request.setRawHeader("Content-Length", postDataSize);

        replyCreateFeed = network.post(request, parameters);

        // check if feed should be added to the event view
        int ev = eventView ? 1 : 0;

        // map the eventView decision into the signal
        QSignalMapper *createFeedSignalMapper = new QSignalMapper(this);
        createFeedSignalMapper->setMapping(replyCreateFeed, ev);

        connect(replyCreateFeed, SIGNAL(finished()), createFeedSignalMapper, SLOT(map()));

        connect(createFeedSignalMapper, SIGNAL(mapped(int)), this, SLOT(feedCreated(int)));
    }
}




/*!
 * \fn void OcFeeds::feedCreated(int eventView)
 * \brief Handles the network reply of createFeed()
 *
 * This internal function handles the reply of the createFeed() network operation. If it was successful
 * the result will be emitted to feedCreatedUpdateDb(), otherwise the signal createdFeedError() will
 * be emitted.
 *
 * \param eventView         1 if should be added to event view, 0 if not
 */

void OcFeeds::feedCreated(int eventView)
{
    connect(this,SIGNAL(createdFeed(QVariantMap)),this,SLOT(feedCreatedUpdateDb(QVariantMap)));

    if (replyCreateFeed->error() == QNetworkReply::NoError)
    {
        QVariantMap createFeedResult = helper.jsonToQt(replyCreateFeed->readAll());

        replyCreateFeed->deleteLater();

        if (createFeedResult.isEmpty())
        {
            emit createdFeedError(tr("Server reply was empty."));
        } else {
            // add event view decision to the result
            createFeedResult["eventView"] = eventView;
            emit createdFeed(createFeedResult);
        }

    } else {
        QVariantMap createFeedResult = helper.jsonToQt(replyCreateFeed->readAll());

        QString createFeedResultError = createFeedResult["message"].toString();

        replyCreateFeed->deleteLater();

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

void OcFeeds::feedCreatedUpdateDb(QVariantMap createFeedResult)
{
    connect(this,SIGNAL(createdFeedUpdateDbSuccess(QVariantMap, QString)),this,SLOT(feedCreatedFetchItems(QVariantMap, QString)));

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
        if (createFeedResult["eventView"] == 1)
        {
            QString feedsForEventView = config.getSetting(QString("event/feeds"), QDBusVariant("")).variant().toString();
            feedsForEventView.append(",");
            feedsForEventView.append(map["id"].toString());
            config.setSetting(QString("event/feeds"), QDBusVariant(feedsForEventView));
        }

        emit createdFeedUpdateDbSuccess(createFeedResult, map["title"].toString());
    }
}




/*!
 * \fn void OcFeeds::feedCreatedFetchItems(QVariantMap createFeedResult, QString feedName)
 * \brief Fetches items of new created feed
 *
 * This internal function is called after a feed was successfully created and fetches the new
 * available items of this feed.
 *
 * \param createFeedResult the result of the network reply
 * \param feedName      the name of the feed
 */

void OcFeeds::feedCreatedFetchItems(QVariantMap createFeedResult, QString feedName)
{
    QEventLoop loop;
    connect(&items,SIGNAL(requestedItemsSuccess()),&loop,SLOT(quit()));
    foreach (QVariant feed, createFeedResult["feeds"].toList())
    {
        QMap<QString, QVariant> map = feed.value<QVariantMap>();
        items.requestItems("100", "0", "0", map["id"].toString(), "true"); // batchSize, offset, type, id, getRead
    }
    loop.exec();
    emit createdFeedSuccess(feedName);
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
        urlDeleteFeed = helper.buildUrl(feed);

        // Building the request
        QNetworkRequest request(urlDeleteFeed);

        replyDeleteFeed = network.deleteResource(request);

        connect(replyDeleteFeed,SIGNAL(finished()),this,SLOT(feedDeleted()));
    }
}




/*!
 * \fn void OcFeeds::feedDeleted()
 * \brief Handles the reply of delteFeed()
 *
 * This internal function handles the reply of the network delete request performed by deleteFeed().
 * If successfull it emits the answer to feedDeletedUpdateDb() and feedDeletedCleanItems(),
 * otherwise it emits the signal edeltedFeedError().
 */

void OcFeeds::feedDeleted()
{
    connect(this,SIGNAL(deletedFeed(int)),this,SLOT(feedDeletedUpdateDb(int)));
//    connect(this,SIGNAL(deletedFeed(int)),this,SLOT(feedDeletedCleanItems(int)));

    if (replyDeleteFeed->error() == QNetworkReply::NoError)
    {
        // extract folder id out of URL
        QString url = replyDeleteFeed->url().toString();
        QStringList urlParts = url.split("/");
        int id = urlParts.last().toInt();

        replyDeleteFeed->deleteLater();
        emit deletedFeed(id);

    } else {
        QVariantMap deleteFeedResult;
        deleteFeedResult = helper.jsonToQt(replyDeleteFeed->readAll());

        QString deleteFeedResultError = deleteFeedResult["message"].toString();

        replyDeleteFeed->deleteLater();

#ifdef QT_DEBUG
        qDebug() << deleteFeedResultError;
#endif

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

void OcFeeds::feedDeletedUpdateDb(int id)
{
    QSqlQuery query;

    query.exec(QString("DELETE FROM feeds WHERE id = %1").arg(id));

    // remove id from the list of the feeds for the event view
    QString feedsForEventView = config.getSetting(QString("event/feeds"), QDBusVariant("")).variant().toString();
    QStringList feedsForEventsList = feedsForEventView.split(",");

    feedsForEventsList.removeOne(QString::number(id));
    feedsForEventView = feedsForEventsList.join(",");

    config.setSetting(QString("event/feeds"), QDBusVariant(feedsForEventView));

    items.cleanItems(id);

    emit deletedFeedSuccess();
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
    items.cleanItems(id);
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
        // Create the API URL
        QString feed = "feeds/";
        feed.append(id);
        feed.append("/move");
        urlMoveFeed = helper.buildUrl(feed);

        // Create the JSON string
        QByteArray parameters("{\"folderId\": ");
        parameters.append(folderId);
        parameters.append("}");

        // Calculate content length header
        QByteArray postDataSize = QByteArray::number(parameters.size());

        // Building the request
        QNetworkRequest request(urlMoveFeed);

        // Add the headers
        request.setRawHeader("Content-Type", "application/json; charset=utf-8");
        request.setRawHeader("Content-Length", postDataSize);

        replyMoveFeed = network.put(request, parameters);

        // map the name into the signal
        QSignalMapper *signalMapper = new QSignalMapper(this);
        signalMapper->setMapping(replyMoveFeed, folderId);

        connect(replyMoveFeed, SIGNAL(finished()), signalMapper, SLOT(map()));

        connect(signalMapper, SIGNAL(mapped(QString)), this, SLOT(feedMoved(QString)));
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
 *
 * \param folderId      ID of the target folder
 */

void OcFeeds::feedMoved(QString folderId)
{
    connect(this,SIGNAL(movedFeed(int,QString)),this,SLOT(feedMovedUpdateDb(int, QString)), Qt::UniqueConnection);

    if (replyMoveFeed->error() == QNetworkReply::NoError)
    {
        // extract feed id out of URL
        QString url = replyMoveFeed->url().toString();
        QStringList urlParts = url.split("/");
        urlParts.removeLast();
        int id = urlParts.last().toInt();


        replyMoveFeed->deleteLater();

        emit movedFeed(id, folderId);

    } else {
        QVariantMap moveresult;
        moveresult = helper.jsonToQt(replyMoveFeed->readAll());

        QString moveFeedResultError = moveresult["message"].toString();

        replyMoveFeed->deleteLater();

#ifdef QT_DEBUG
        qDebug() << moveFeedResultError;
#endif

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

void OcFeeds::feedMovedUpdateDb(int id, QString folderId)
{
    QSqlQuery query;

    query.prepare("UPDATE feeds SET folderId = :folderId WHERE id = :id;");
    query.bindValue(":folderId", folderId);
    query.bindValue(":id", id);
    query.exec();

    emit movedFeedSuccess();
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

        QString storagePath(QDir::homePath());
        storagePath.append(BASE_PATH).append("/favicons");
        storagePath.append(QDir::separator()).append(feedId).append("_").append(fileInfo.fileName());
        storagePath = QDir::toNativeSeparators(storagePath);
        QImage favicon;
        if(favicon.loadFromData(replyGetFavicon->readAll()))
        {
            if(favicon.save(storagePath))
            {
                QSqlQuery query;

                query.prepare("UPDATE feeds SET iconSource = :source, iconWidth = :width, iconHeight = :height WHERE id = :id;");
                query.bindValue(":source", storagePath);
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
        QUrl urlMarkFeedRead = helper.buildUrl(feed);

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

        // Calculate content length header
        QByteArray postDataSize = QByteArray::number(parameters.size());

        // Building the request
        QNetworkRequest request(urlMarkFeedRead);

        // Add the headers
        request.setRawHeader("Content-Type", "application/json; charset=utf-8");
        request.setRawHeader("Content-Length", postDataSize);

        replyMarkFeedRead = network.put(request, parameters);

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
    connect(this,SIGNAL(markedReadFeed(int)),this,SLOT(feedMarkedReadUpdateDb(int)),Qt::UniqueConnection);

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

        emit markedReadFeed(id);
    } else {
        QVariantMap markFeedReadResult = helper.jsonToQt(replyMarkFeedRead->readAll());

        QString markedReadFeedErrorResult = markFeedReadResult["message"].toString();

        emit markedReadFeedError(markedReadFeedErrorResult);
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

void OcFeeds::feedMarkedReadUpdateDb(int id)
{
    QDateTime ts;
    QSqlQuery query;
    QSqlDatabase::database().transaction();
#if defined(MEEGO_EDITION_HARMATTAN)
    query.exec(QString("UPDATE items SET unread = \"false\", lastModified = %2 WHERE unread = \"true\" AND feedId = %1").arg(id).arg(ts.currentDateTimeUtc().toTime_t()));
#else
    query.exec(QString("UPDATE items SET unread = 0, lastModified = %2 WHERE unread = 1 AND feedId = %1").arg(id).arg(ts.currentDateTimeUtc().toTime_t()));
#endif
    QSqlDatabase::database().commit();

    emit markedReadFeedSuccess();
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
