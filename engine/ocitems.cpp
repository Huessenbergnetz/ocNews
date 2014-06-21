#include <QtNetwork>
#include "ocitems.h"

OcItems::OcItems(QObject *parent) :
    QObject(parent)
{
    itemsToMark = QStringList();
    hashesToStar = QStringList();
    markAllReadNewestId = "";
}


void OcItems::requestItems(const QString &batchSize, const QString &offset, const QString &type, const QString &id, const QString &getRead)
{
    if (network.isFlightMode())
    {
        emit requestedItemsError(tr("Device is in flight mode."));
    } else {

#ifdef QT_DEBUG
        qDebug() << "Start to fetch items from server.";
#endif

        QList<QPair<QString, QString> > query;
        query << qMakePair(QString("batchSize"), batchSize);
        query << qMakePair(QString("offset"), offset);
        query << qMakePair(QString("type"), type);
        query << qMakePair(QString("id"), id);
        query << qMakePair(QString("getRead"), getRead);

        replyRequestItems = network.get(helper.buildRequest("items", 0, query));

        connect(replyRequestItems,SIGNAL(finished()),this,SLOT(itemsRequested()));
    }

}


void OcItems::itemsRequested()
{
    if (replyRequestItems->error() == QNetworkReply::NoError)
    {
#ifdef QT_DEBUG
        qDebug() << "Start to parse JSON into QVariantMap.";
#endif
        QVariantMap requestItemsResult = helper.jsonToQt(replyRequestItems->readAll());

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
        QString type = QUrlQuery(replyRequestItems->url()).queryItemValue("type");
        QString fid = QUrlQuery(replyRequestItems->url()).queryItemValue("id");
#else
        QString type = replyRequestItems->url().queryItemValue("type");
        QString fid = replyRequestItems->url().queryItemValue("id");
#endif

        replyRequestItems->deleteLater();
#ifdef QT_DEBUG
        qDebug() << "Finished parsing.";
#endif
        if (requestItemsResult.isEmpty())
        {
            notify.showNotification(tr("Server reply was empty."), tr("Error in server reply"), OcNotifications::Error);
            emit requestedItemsError(tr("Server reply was empty."));

        } else {
            itemsRequestedUpdateDb(requestItemsResult, type, fid);
        }

    } else {
        notify.showNotification(replyRequestItems->errorString(), tr("HTTP error"), OcNotifications::Error);
        emit requestedItemsError(replyRequestItems->errorString());
#ifdef QT_DEBUG
        qDebug() << "HTTP-Error:" << replyRequestItems->errorString();
#endif
        replyRequestItems->deleteLater();
    }
}

void OcItems::itemsRequestedUpdateDb(const QVariantMap &requestItemsResult, const QString &type, const QString &fId)
{
#ifdef QT_DEBUG
    qDebug() << "Start updating items database.";
#endif

    QList<int> newEventItems; // list for the new items, for sending to event feed
    QList<int> newItems; // list for fetching images
    QList<int> updatedItems;
    int unreadCount = 0;
    QString feedsForEventView = config.getSetting(QString("event/feeds"), QDBusVariant("")).variant().toString();
    QStringList feedsForEventsList = feedsForEventView.split(",");

#ifdef QT_DEBUG
    qDebug() << "EventViewString: " << feedsForEventView;
    qDebug() << "EventViewList: " << feedsForEventsList;
#endif

    QSqlQuery query;

    QSqlDatabase::database().transaction();
    foreach (QVariant item, requestItemsResult["items"].toList())
    {

        // remap QVariantMap
        QMap<QString, QVariant> map = item.value<QVariantMap>();

        // check if item is already in database
        query.exec(QString("SELECT id, lastModified FROM items WHERE id = %1").arg(map["id"].toInt()));
        if (query.next())
        {
            // check if item was modified on server
            if (query.value(1).toInt() < map["lastModified"].toInt())
            {
#ifdef QT_DEBUG
                qDebug() << "Update ID:" << map["id"].toInt() << "|" << map["lastModified"].toInt();
#endif
                updatedItems << map["id"].toInt();

                query.prepare("UPDATE items SET "
                              "guid = :guid, "
                              "guidHash = :guidHash, "
                              "url = :url, "
                              "title = :title, "
                              "author = :author, "
                              "pubDate = :pubDate, "
                              "enclosureMime = :enclosureMime, "
                              "enclosureLink = :enclosureLink, "
                              "feedId = :feedId, "
                              "unread = :unread, "
                              "starred = :starred, "
                              "lastModified = :lastModified "
                              "WHERE id = :id;"
                              );
                query.bindValue(":guid", map["guid"].toString());
                query.bindValue(":guidHash", map["guidHash"].toString());
                query.bindValue(":url", map["url"].toString());
                query.bindValue(":title", map["title"].toString());
                query.bindValue(":author", map["author"].toString());
                query.bindValue(":pubDate", map["pubDate"].toInt());
//                query.bindValue(":body", map["body"].toString());
                query.bindValue(":enclosureMime", map["enclosureMime"].toString());
                query.bindValue(":enclosureLink", map["enclosureLink"].toString());
                query.bindValue(":feedId", map["feedId"].toInt());
                query.bindValue(":unread", map["unread"].toBool());
                query.bindValue(":starred", map["starred"].toBool());
                query.bindValue(":lastModified", map["lastModified"].toInt());
                query.bindValue(":id", map["id"].toInt());
                query.exec();
            }

        } else { // if item is not in database, create it there
#ifdef QT_DEBUG
            qDebug() << "Create ID:" << map["id"].toInt();
#endif

            newItems.append(map["id"].toInt());

            if (map["unread"].toBool())
                unreadCount++;

            // collect ids of new items for the event feed
            if (!feedsForEventsList.isEmpty())
            {
                if (feedsForEventsList.contains(map["feedId"].toString()))
                    newEventItems.append(map["id"].toInt());
            }

            query.prepare("INSERT INTO items (id, guid, guidHash, url, title, author, pubDate, body, enclosureMime, enclosureLink, feedId, unread, starred, lastModified) "
                          "VALUES (:id, :guid, :guidHash, :url, :title, :author, :pubDate, :body, :enclosureMime, :enclosureLink, :feedId, :unread, :starred, :lastModified);");
            query.bindValue(":id", map["id"].toInt());
            query.bindValue(":guid", map["guid"].toString());
            query.bindValue(":guidHash", map["guidHash"].toString());
            query.bindValue(":url", map["url"].toString());
            query.bindValue(":title", map["title"].toString());
            query.bindValue(":author", map["author"].toString());
            query.bindValue(":pubDate", map["pubDate"].toInt());
            query.bindValue(":body", map["body"].toString());
            query.bindValue(":enclosureMime", map["enclosureMime"].toString());
            query.bindValue(":enclosureLink", map["enclosureLink"].toString());
            query.bindValue(":feedId", map["feedId"].toInt());
            query.bindValue(":unread", map["unread"].toBool());
            query.bindValue(":starred", map["starred"].toBool());
            query.bindValue(":lastModified", map["lastModified"].toInt());
            query.exec();
        }

    }
    QSqlDatabase::database().commit();

#ifdef QT_DEBUG
    qDebug() << "New Items for EventView:" << newEventItems;
#endif


    // now check if items were deleted on server

    QList<int> idList;
    QList<int> idListDeleted;

    // put all the fetched ids into a list
    foreach (QVariant item, requestItemsResult["items"].toList())
    {
        QMap<QString, QVariant> map = item.value<QVariantMap>();

        idList << map["id"].toInt();
    }

    // compare the ids and put the deleted ids into a list
    if (type == "3") {
        query.exec(QString("SELECT id FROM items;"));
    } else if (type == "2") {
        query.exec(QString("SELECT id FROM items where starred = ").append(SQL_TRUE));
    } else if (type == "1") {
        query.exec(QString("SELECT id FROM items WHERE feedId IN (SELECT id FROM feeds WHERE folderId = %1);").arg(fId.toInt()));
    } else if (type == "0") {
        query.exec(QString("SELECT id FROM items WHERE feedId = %1").arg(fId.toInt()));
    }

    while (query.next())
    {
        if (!idList.contains(query.value(0).toInt()))
             idListDeleted << query.value(0).toInt();
    }


    QSqlDatabase::database().transaction();
    // delete the serverside deleted ids in the database
    for (int i = 0; i < idListDeleted.size(); ++i)
    {
        query.exec(QString("DELETE FROM items WHERE id = %1").arg(idListDeleted.at(i)));
#ifdef QT_DEBUG
        qDebug() << "Deleted ID:" << idListDeleted.at(i);
#endif
    }
    QSqlDatabase::database().commit();


#ifdef QT_DEBUG
    qDebug() << "Finished updating database, emit requestedItemsSuccess";
#endif
    emit requestedItemsSuccess(updatedItems, newItems, idListDeleted);

    if ((unreadCount > 0) && config.getSetting(QString("notifications/newItems"), QDBusVariant(false)).variant().toBool())
    {
//        QString notificationBody(tr("%n new", "", newItems.count()));
//        notificationBody.append(", ").append(tr("%n unread", "", unreadCount));
        notify.showNotification(tr("%n new unread item(s)", "", unreadCount), tr("New articles available"), OcNotifications::Success);
    }

    if (!newEventItems.isEmpty())
        updateEventFeed(newEventItems);
}




void OcItems::updateItems(const QString &lastModified, const QString &type, const QString &id)
{
    QString t_lastModified = lastModified;

    if (network.isFlightMode())
    {
        emit updatedItemsError(tr("Device is in flight mode."));

    } else {

        QSqlQuery query;

        // work queued items
        query.exec("SELECT id FROM queue LIMIT 1");
        if (query.next())
        {
            QEventLoop loop;
            connect(this,SIGNAL(dequeueFinished()),&loop,SLOT(quit()));
            dequeueRead();
            loop.exec();
        }


    #ifdef QT_DEBUG
        qDebug() << "Start updating items from Server: " << t_lastModified << " | " << type << " | " << id;
    #endif
        if (t_lastModified == "0")
        {
                QString querystring = "";
                if (type == "0") {
                    querystring = QString("SELECT MAX(lastModified) FROM items WHERE feedId = %1;").arg(id.toInt());
                } else if (type == "1") {
                    querystring = QString("SELECT MIN(lastModified) FROM "
                                            "(SELECT it.id, it.lastModified, it.feedId FROM items it "
                                                "INNER JOIN (SELECT id, MAX(lastModified) AS MaxLastMod FROM items WHERE feedId IN "
                                                                "(SELECT id FROM feeds WHERE folderId = %1) "
                                                            "GROUP BY feedId) q "
                                                "ON it.id = q.id AND it.lastModified = q.MaxLastMod);").arg(id.toInt());
                } else if (type == "2") {
                    querystring = QString("SELECT MAX(lastModified) FROM items WHERE starred = ").append(SQL_TRUE);
                } else if (type == "3") {
                    t_lastModified = config.getSetting(QString("storage/lastFullUpdate"), QDBusVariant("")).variant().toString();
                    if (t_lastModified == "")
                        querystring = QString("SELECT MAX(lastModified) FROM items;");
                }

                if (querystring != "")
                {
                    query.exec(querystring);
                    if(query.next())
                        t_lastModified = query.value(0).toString();
                }
        }


    #ifdef QT_DEBUG
        qDebug() << "Last Modified: " << t_lastModified;
    #endif

        QList<QPair<QString, QString> > urlQuery;
        urlQuery << qMakePair(QString("lastModified"), t_lastModified);
        urlQuery << qMakePair(QString("type"), type);
        urlQuery << qMakePair(QString("id"), id);

        replyUpdateItems = network.get(helper.buildRequest("items/updated", 0, urlQuery));

        connect(replyUpdateItems,SIGNAL(finished()),this,SLOT(itemsUpdated()));
    }
}



void OcItems::itemsUpdated()
{
    if (replyUpdateItems->error() == QNetworkReply::NoError)
    {
#ifdef QT_DEBUG
        qDebug() << "Start to parse JSON into QVariantMap.";
#endif

        QVariantMap updateItemsResult = helper.jsonToQt(replyUpdateItems->readAll());

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
        QString type = QUrlQuery(replyUpdateItems->url()).queryItemValue("type");
        QString id = QUrlQuery(replyUpdateItems->url()).queryItemValue("id");
#else
        QString type = replyUpdateItems->url().queryItemValue("type");
        QString id = replyUpdateItems->url().queryItemValue("id");
#endif

        replyUpdateItems->deleteLater();

#ifdef QT_DEBUG
        qDebug() << updateItemsResult;
        qDebug() << "Finished parsing.";
#endif

        if (updateItemsResult.isEmpty())
        {
            notify.showNotification(tr("Server reply was empty."), tr("Error in server reply"), OcNotifications::Error);
            emit updatedItemsError(tr("Server reply was empty."));
        } else {
            itemsUpdatedUpdateDb(updateItemsResult, type, id);
        }

    } else {
#ifdef QT_DEBUG
        qDebug() << "HTTP-Error:" << replyUpdateItems->errorString();
#endif
        notify.showNotification(replyUpdateItems->errorString(), tr("HTTP error"), OcNotifications::Error);
        emit updatedItemsError(replyUpdateItems->errorString());
    }
}



void OcItems::itemsUpdatedUpdateDb(const QVariantMap &updateItemsResult, const QString &type, const QString &fid)
{
#ifdef QT_DEBUG
    qDebug() << "Start updating database.";
#endif

    QList<int> newEventItems; // list for the new items, for sending to event feed
    QList<int> newItems; // list for fetching images
    QList<int> updatedItems;
    int unreadCount = 0;
    QString feedsForEventView = config.getSetting(QString("event/feeds"), QDBusVariant("")).variant().toString();
    QStringList feedsForEventsList = feedsForEventView.split(",");

#ifdef QT_DEBUG
    qDebug() << "EventViewString: " << feedsForEventView;
    qDebug() << "EventViewList: " << feedsForEventsList;
#endif

    QSqlQuery query;

    QSqlDatabase::database().transaction();

    foreach (QVariant item, updateItemsResult["items"].toList())
    {

        // remap QVariantMap
        QMap<QString, QVariant> map = item.value<QVariantMap>();

        // check if item is already in database
        query.exec(QString("SELECT id, lastModified FROM items WHERE id = %1").arg(map["id"].toInt()));
        if (query.next())
        {
            // check if item was modified on server
            if (query.value(1).toInt() < map["lastModified"].toInt())
            {
#ifdef QT_DEBUG
                qDebug() << "Update Item ID:" << map["id"].toInt() << "|" << map["lastModified"].toInt();
#endif
                updatedItems << map["id"].toInt();

                query.prepare("UPDATE items SET "
                              "guid = :guid, "
                              "guidHash = :guidHash, "
                              "url = :url, "
                              "title = :title, "
                              "author = :author, "
                              "pubDate = :pubDate, "
                              "enclosureMime = :enclosureMime, "
                              "enclosureLink = :enclosureLink, "
                              "feedId = :feedId, "
                              "unread = :unread, "
                              "starred = :starred, "
                              "lastModified = :lastModified "
                              "WHERE id = :id;"
                              );
                query.bindValue(":guid", map["guid"].toString());
                query.bindValue(":guidHash", map["guidHash"].toString());
                query.bindValue(":url", map["url"].toString());
                query.bindValue(":title", map["title"].toString());
                query.bindValue(":author", map["author"].toString());
                query.bindValue(":pubDate", map["pubDate"].toInt());
//                query.bindValue(":body", map["body"].toString());
                query.bindValue(":enclosureMime", map["enclosureMime"].toString());
                query.bindValue(":enclosureLink", map["enclosureLink"].toString());
                query.bindValue(":feedId", map["feedId"].toInt());
                query.bindValue(":unread", map["unread"].toBool());
                query.bindValue(":starred", map["starred"].toBool());
                query.bindValue(":lastModified", map["lastModified"].toInt());
                query.bindValue(":id", map["id"].toInt());
                query.exec();
            }

        } else { // if item is not in database, create it there
#ifdef QT_DEBUG
            qDebug() << "Create Item ID:" << map["id"].toInt();
#endif

            newItems.append(map["id"].toInt());

            if (map["unread"].toBool())
                unreadCount++;

            // collect ids of new items for the event feed
            if (!feedsForEventsList.isEmpty())
            {
                if (feedsForEventsList.contains(map["feedId"].toString()))
                    newEventItems.append(map["id"].toInt());
            }
#ifdef QT_DEBUG
            qDebug() << "New Items for EventView:" << newEventItems;
#endif

            query.prepare("INSERT INTO items (id, guid, guidHash, url, title, author, pubDate, body, enclosureMime, enclosureLink, feedId, unread, starred, lastModified) "
                          "VALUES (:id, :guid, :guidHash, :url, :title, :author, :pubDate, :body, :enclosureMime, :enclosureLink, :feedId, :unread, :starred, :lastModified);");
            query.bindValue(":id", map["id"].toInt());
            query.bindValue(":guid", map["guid"].toString());
            query.bindValue(":guidHash", map["guidHash"].toString());
            query.bindValue(":url", map["url"].toString());
            query.bindValue(":title", map["title"].toString());
            query.bindValue(":author", map["author"].toString());
            query.bindValue(":pubDate", map["pubDate"].toInt());
            query.bindValue(":body", map["body"].toString());
            query.bindValue(":enclosureMime", map["enclosureMime"].toString());
            query.bindValue(":enclosureLink", map["enclosureLink"].toString());
            query.bindValue(":feedId", map["feedId"].toInt());
            query.bindValue(":unread", map["unread"].toBool());
            query.bindValue(":starred", map["starred"].toBool());
            query.bindValue(":lastModified", map["lastModified"].toInt());
            query.exec();
        }

    }

    QSqlDatabase::database().commit();


    // set lastFullUpdate in settings
    if (type == "3")
    {
        QDateTime ts;
        config.setSetting("storage/lastFullUpdate", QDBusVariant(ts.currentDateTimeUtc().toTime_t()));
    }



    // now delete Items that exceed the maximum amount of items to store per feed
    // get feed ids
#ifdef QT_DEBUG
    qDebug() << "Start cleaning Database";
#endif

    QList<int> imageItemIdsToDelete;

    if (type != "2")
    {
        QList<int> feedIds;
        int lowestId = 0;

        if (type == "0")
        {
            feedIds << fid.toInt();
        } else if (type == "1") {
            query.exec(QString("SELECT id FROM feeds WHERE folderId = %1").arg(fid.toInt()));
            while (query.next())
                feedIds << query.value(0).toInt();
        } else if (type == "3") {
            query.exec("SELECT id FROM feeds;");
            while (query.next())
                feedIds << query.value(0).toInt();
        }

        // get lowest itemID
        QSqlDatabase::database().transaction();
        for (int i = 0; i < feedIds.size(); ++i)
        {
            query.exec(QString("SELECT MIN(id) FROM (SELECT id FROM items WHERE feedId = %1 ORDER BY id DESC LIMIT %2);").arg(feedIds.at(i)).arg(config.getSetting(QString("storage/maxitems"), QDBusVariant(100)).variant().toInt()));
            if (query.next())
                lowestId = query.value(0).toInt();


            // get item ids to delte cached image files
            query.exec(QString("SELECT id FROM items WHERE starred = %3 AND id < %1 AND feedId = %2;").arg(lowestId).arg(feedIds.at(i)).arg(SQL_FALSE));
            while(query.next())
            {
                imageItemIdsToDelete << query.value(0).toInt();
            }

            query.exec(QString("DELETE FROM items WHERE starred = %3 AND id < %1 AND feedId = %2;").arg(lowestId).arg(feedIds.at(i)).arg(SQL_FALSE));

        }
        QSqlDatabase::database().commit();

    }
#ifdef QT_DEBUG
    qDebug() << "Emit updatedItemsSuccess";
#endif
    emit updatedItemsSuccess(updatedItems, newItems, imageItemIdsToDelete);

    if ((unreadCount > 0) && config.getSetting(QString("notifications/newItems"), QDBusVariant(false)).variant().toBool())
    {
//        QString notificationBody(tr("%n new", "", newItems.count()));
//        notificationBody.append(", ").append(tr("%n unread", "", unreadCount));
        notify.showNotification(tr("%n new unread item(s)", "", unreadCount), tr("New articles available"), OcNotifications::Success);
    }

    if (!newEventItems.isEmpty())
        updateEventFeed(newEventItems);
}


void OcItems::markItemsTillThis(const QString &action, const QDBusVariant &pubDate, const QDBusVariant &feedId)
{
    QVariant t_pubDate = pubDate.variant();
    QVariant t_feedId = feedId.variant();

#ifdef QT_DEBUG
    qDebug() << action << " | " << t_pubDate << " | " << t_feedId;
#endif

#if defined(MEEGO_EDITION_HARMATTAN)
    QString readStatus;
    if (action == "read")
    {
        readStatus = "true";
    } else {
        readStatus = "false";
    }
#else
    int readStatus;
    if (action == "read")
    {
        readStatus = 1;
    } else {
        readStatus = 0;
    }
#endif

    QSqlQuery query;
    query.exec(QString("SELECT id FROM items WHERE pubDate <= %1 AND feedId = %2 AND unread = %3").arg(t_pubDate.toInt()).arg(t_feedId.toInt()).arg(readStatus));

#ifdef QT_DEBUG
    qDebug() << query.lastError();
#endif

    QVariantList itemIds;
    while(query.next())
        itemIds << query.value(0).toString();

#ifdef QT_DEBUG
    qDebug() << itemIds;
#endif

    if (!itemIds.isEmpty()) {
        markItems(action, itemIds);
    } else {
        emit markedItemsSuccess(QStringList(), QString());
    }

}




void OcItems::markItems(const QString &action, const QVariantList &ids)
{
#ifdef QT_DEBUG
    qDebug() << "markItems: " << ids;
#endif

    if (ids.isEmpty()) {
        emit markedItemsSuccess(QStringList(), QString());
        return;
    }

    itemsToMark.clear();

    for (int i = 0; i < ids.size(); ++i)
        itemsToMark << ids.at(i).toString();

    // Create the JSON string and convert QVariantList to QStringList
    QByteArray parameters("{\"items\": [");
    parameters.append(itemsToMark.join(", "));
    parameters.append("]}");

#ifdef QT_DEBUG
    qDebug() << "markItems: " << itemsToMark;
    qDebug() << parameters;
#endif

    if (network.isFlightMode())
    {
        if (action == "read")
        {
            queueItems(OcItems::MarkRead, itemsToMark);
        } else {
            queueItems(OcItems::MarkUnread, itemsToMark);
        }

        itemsToMark << action;
        itemsMarkedUpdateDb(itemsToMark);

    } else {

        if (action == "read")
        {
            replyMarkItems = network.put(helper.buildRequest("items/read/multiple", parameters.size()), parameters);
        } else {
            replyMarkItems = network.put(helper.buildRequest("items/unread/multiple", parameters.size()), parameters);
        }

        itemsToMark << action;
        connect(replyMarkItems, SIGNAL(finished()), this, SLOT(itemsMarked()));
    }

}

void OcItems::itemsMarked()
{

    if (replyMarkItems->error() == QNetworkReply::NoError)
    {
        itemsMarkedUpdateDb(itemsToMark);
    } else {

#ifdef QT_DEBUG
        qDebug() << "HTTP-Error:" << replyMarkItems->errorString();
        qDebug() << replyMarkItems->error();
#endif

        if (replyMarkItems->error() == QNetworkReply::TimeoutError || replyMarkItems->error() == QNetworkReply::UnknownNetworkError)
        {
            QStringList queueItemIds = itemsToMark;
            QString action = queueItemIds.last();
            queueItemIds.removeLast();

            if (action == "read")
            {
                queueItems(OcItems::MarkRead, queueItemIds);
            } else {
                queueItems(OcItems::MarkUnread, queueItemIds);
            }

            itemsMarkedUpdateDb(itemsToMark);

        } else {
            notify.showNotification(replyMarkItems->errorString(), tr("Failed to mark articles"), OcNotifications::Error);
            emit markedItemsError(replyMarkItems->errorString());
        }
    }

    replyMarkItems->deleteLater();
}


void OcItems::itemsMarkedUpdateDb(const QStringList &ids)
{
#ifdef QT_DEBUG
    qDebug() << "markItems Update DB: " << ids;
#endif
    QStringList t_ids = ids;
    QString action = t_ids.last();
    t_ids.removeLast();

    QSqlQuery query;
    QDateTime ts;
    QSqlDatabase::database().transaction();
    if (action == "read")
    {
        for (int i = 0; i < t_ids.size(); ++i)
            query.exec(QString("UPDATE items SET unread = %3, lastModified = %2 WHERE id = %1;").arg(t_ids.at(i)).arg(ts.currentDateTimeUtc().toTime_t()).arg(SQL_FALSE));

    } else {
        for (int i = 0; i < t_ids.size(); ++i)
            query.exec(QString("UPDATE items SET unread = %3, lastModified = %2 WHERE id = %1;").arg(t_ids.at(i)).arg(ts.currentDateTimeUtc().toTime_t()).arg(SQL_TRUE));
    }

    QSqlDatabase::database().commit();
#ifdef QT_DEBUG
    qDebug() << "markItems DB Error: " <<  QSqlDatabase::database().lastError();
#endif


    emit markedItemsSuccess(t_ids, action);
}





void OcItems::starItems(const QString &action, const QVariantMap &itemIds)
{

    hashesToStar.clear();

    // Create the JSON string
    QMapIterator<QString, QVariant> i(itemIds);
    QByteArray parameters("{\"items\": [");
    int count = 0;
    while (i.hasNext()) {
        i.next();
        if (count != 0)
            parameters.append(", ");
        parameters.append("{\"feedId\": ");
        parameters.append(i.key());
        parameters.append(", ");
        parameters.append("\"guidHash\": \"");
        parameters.append(i.value().toString());
        parameters.append("\"}");
        hashesToStar << i.value().toString();
        count++;
    }
    parameters.append("]}");

#ifdef QT_DEBUG
    qDebug() << parameters;
#endif



    if (network.isFlightMode())
    {
        if (action == "star")
        {
            queueItems(OcItems::Star, hashesToStar);
        } else {
            queueItems(OcItems::Unstar, hashesToStar);
        }

        hashesToStar << action;

        itemsStarredUpdateDb(hashesToStar);

    } else {

        hashesToStar << action;

        if (action == "star")
        {
            replyStarItems = network.put(helper.buildRequest("items/star/multiple", parameters.length()), parameters);
        } else {
            replyStarItems = network.put(helper.buildRequest("items/unstar/multiple", parameters.length()), parameters);
        }

        connect(replyStarItems, SIGNAL(finished()), this, SLOT(itemsStarred()));
    }
}

void OcItems::itemsStarred()
{
    if (replyStarItems->error() == QNetworkReply::NoError)
    {
        replyStarItems->deleteLater();

        itemsStarredUpdateDb(hashesToStar);

    } else {
#ifdef QT_DEBUG
        qDebug() << "HTTP-Error:" << replyStarItems->errorString();
#endif
        if (replyStarItems->error() == QNetworkReply::TimeoutError || replyStarItems->error() == QNetworkReply::UnknownNetworkError)
        {

            QStringList queueHashes = hashesToStar;
            QString action = queueHashes.last();
            queueHashes.removeLast();


            if (action == "star")
            {
                queueItems(OcItems::Star, queueHashes);
            } else {
                queueItems(OcItems::Unstar, queueHashes);
            }
             itemsStarredUpdateDb(hashesToStar);

        } else {
            notify.showNotification(replyStarItems->errorString(), tr("Failed to un/star articles"), OcNotifications::Error);
            emit starredItemsError(replyStarItems->errorString());
        }

        replyStarItems->deleteLater();
    }
}

void OcItems::itemsStarredUpdateDb(const QStringList &hashes)
{
    QStringList t_hashes = hashes;
    QString action = t_hashes.last();
    t_hashes.removeLast();
#ifdef QT_DEBUG
    qDebug() << t_hashes;
#endif

    QDateTime ts;
    QSqlQuery query;
    if (action == "star")
    {
        for (int i = 0; i < t_hashes.size(); ++i)
            query.exec(QString("UPDATE items SET starred = %3, lastModified = %2  WHERE guidHash = \"%1\";").arg(t_hashes.at(i)).arg(ts.currentDateTimeUtc().toTime_t()).arg(SQL_TRUE));

    } else {
        for (int i = 0; i < t_hashes.size(); ++i)
            query.exec(QString("UPDATE items SET starred = %3, lastModified = %2 WHERE guidHash = \"%1\";").arg(t_hashes.at(i)).arg(ts.currentDateTimeUtc().toTime_t()).arg(SQL_FALSE));
    }

    emit starredItemsSuccess(t_hashes, action);
}





void OcItems::markAllItemsRead()
{
    markAllReadNewestId.clear();

    if (network.isFlightMode())
    {
        emit markedAllItemsReadError(tr("Device is in flight mode."));

    } else {
        // get newest item id
        QSqlQuery query;
        query.exec("SELECT MAX(id) FROM items;");
        if (query.next())
            markAllReadNewestId = query.value(0).toString();

        // build request parameters
        QByteArray parameters("{\"newestItemId\": ");
        parameters.append(markAllReadNewestId);
        parameters.append("}");

    #ifdef QT_DEBUG
        qDebug() << parameters;
    #endif

        replyMarkAllItemsRead = network.put(helper.buildRequest("items/read", parameters.length()), parameters);

        connect(replyMarkAllItemsRead, SIGNAL(finished()), this, SLOT(itemsMarkedAllRead()));
    }

}

void OcItems::itemsMarkedAllRead()
{
    if (replyMarkAllItemsRead->error() == QNetworkReply::NoError)
    {
        replyMarkAllItemsRead->deleteLater();

        itemsMarkedAllReadUpdateDb(markAllReadNewestId);

    } else {
#ifdef QT_DEBUG
        qDebug() << "HTTP-Error:" << replyMarkAllItemsRead->errorString();
#endif
        notify.showNotification(replyMarkAllItemsRead->errorString(), tr("Failed to mark articles"), OcNotifications::Error);
        emit markedAllItemsReadError(replyMarkAllItemsRead->errorString());
        replyMarkAllItemsRead->deleteLater();
    }
}

void OcItems::itemsMarkedAllReadUpdateDb(const QString &newestItemId)
{
    QDateTime ts;
    QSqlQuery query;
    query.exec(QString("UPDATE items SET unread = %3, lastModified = %2 WHERE id <= %1 AND unread = %4;").arg(newestItemId.toInt()).arg(ts.currentDateTimeUtc().toTime_t()).arg(SQL_FALSE).arg(SQL_TRUE));

    emit markedAllItemsReadSuccess();
}


void OcItems::cleanItems(const int &feedId)
{
    QList<int> feeds;

    feeds << feedId;

    cleanItems(QList<int>(), QList<int>(), feeds);
}


void OcItems::cleanItems(const QList<int> &feedIds)
{
    cleanItems(QList<int>(), QList<int>(), feedIds);
}



void OcItems::cleanItems(const QList<int> &updated, const QList<int> &newFeeds, const QList<int> &deleted)
{
    if (!deleted.isEmpty())
    {
        QList<int> itemsToDelete;

        for (int i = 0; i < deleted.size(); ++i)
        {

#ifdef QT_DEBUG
            qDebug() << "Deleting feed ID: " << deleted.at(i);
#endif

            QSqlQuery query;
            query.exec(QString("SELECT id FROM items WHERE feedId = %1;").arg(deleted.at(i)));

            while(query.next())
            {
                itemsToDelete << query.value(0).toInt();
            }

            query.exec(QString("DELETE FROM items WHERE feedId = %1;").arg(deleted.at(i)));

#if defined(MEEGO_EDITION_HARMATTAN)
            // remove items from event feed
            MEventFeed::instance()->removeItemsBySourceName(QString("ocnews-feed-").append(QString::number(deleted.at(i))));
#endif
        }

        emit cleanedItems(QList<int>(), QList<int>(), itemsToDelete);
    }
}


void OcItems::queueItems(int action, QStringList &itemIds)
{
    QSqlQuery query;
    QDateTime ts;

#ifdef QT_DEBUG
    qDebug() << itemIds;
#endif

    if (action == OcItems::MarkRead || action == OcItems::MarkUnread)
    {
        for (int i = 0; i < itemIds.size(); ++i)
        {
            query.exec(QString("SELECT action FROM queue WHERE itemId = %1").arg(itemIds.at(i).toInt()));
            if (query.next())
            {
                if(query.value(0).toInt() != action)
                {
                    query.exec(QString("DELETE FROM queue WHERE itemId = %1").arg(itemIds.at(i).toInt()));
                } else {
                    query.exec(QString("UPDATE queue SET entryDate = %1 WHERE itemId = %2").arg(ts.currentDateTimeUtc().toTime_t()).arg(itemIds.at(i).toInt()));
                }
            } else {
                query.exec(QString("INSERT INTO queue (action, itemId, entryDate) VALUES (%1, %2, %3);").arg(action).arg(itemIds.at(i).toInt()).arg(ts.currentDateTimeUtc().toTime_t()));
            }
        }
    } else {
        for (int i = 0; i < itemIds.size(); ++i)
        {
            query.exec(QString("SELECT action FROM queue WHERE guidHash = \"%1\"").arg(itemIds.at(i)));
            if (query.next())
            {
                if(query.value(0).toInt() != action)
                {
                    query.exec(QString("DELETE FROM queue WHERE guidHash = \"%1\"").arg(itemIds.at(i)));
                } else {
                    query.exec(QString("UPDATE queue SET entryDate = %1 WHERE guidHash = \"%2\"").arg(ts.currentDateTimeUtc().toTime_t()).arg(itemIds.at(i)));
                }
            } else {
                query.exec(QString("INSERT INTO queue (action, guidHash, entryDate) VALUES (%1, \"%2\", %3);").arg(action).arg(itemIds.at(i)).arg(ts.currentDateTimeUtc().toTime_t()));
            }
        }
    }
}


void OcItems::dequeueItems()
{
    QSqlQuery query;

    query.exec("SELECT id FROM queue LIMIT 1");
    if (query.next())
    {
#ifdef QT_DEBUG
        qDebug() << "Start dequeueing items.";
#endif
        dequeueRead();
    } else {
        dequeueFinish();
    }
}


void OcItems::dequeueRead()
{
    QVariantList markItemIds;
    QSqlQuery query;

    query.exec(QString("SELECT itemId FROM queue WHERE action = %1").arg(OcItems::MarkRead));
    while (query.next())
    {
        markItemIds << query.value(0);
    }
    if (!markItemIds.isEmpty())
    {

#ifdef QT_DEBUG
        qDebug() << "Dequeue read items.";
#endif
        connect(this,SIGNAL(markedItemsSuccess(QStringList,QString)),this,SLOT(dequeueUnread()),Qt::UniqueConnection);
        markItems("read", markItemIds);
        query.exec(QString("DELETE FROM queue WHERE action = %1").arg(OcItems::MarkRead));
    } else {
        dequeueUnread();
    }
}


void OcItems::dequeueUnread()
{
    QVariantList markItemIds;
    QSqlQuery query;

    query.exec(QString("SELECT itemId FROM queue WHERE action = %1").arg(OcItems::MarkUnread));
    while (query.next())
    {
        markItemIds << query.value(0);
    }
    if (!markItemIds.isEmpty())
    {

#ifdef QT_DEBUG
        qDebug() << "Dequeue unread items.";
#endif
        connect(this,SIGNAL(markedItemsSuccess(QStringList,QString)),this,SLOT(dequeueStarred()),Qt::UniqueConnection);
        markItems("unread", markItemIds);
        query.exec(QString("DELETE FROM queue WHERE action = %1").arg(OcItems::MarkUnread));
    } else {
        dequeueStarred();
    }
}


void OcItems::dequeueStarred()
{
    QVariantMap starItemHashes;
    QSqlQuery query;

    query.exec(QString("SELECT qu.guidHash, (SELECT feedId FROM items WHERE guidHash = qu.guidHash) AS feedId FROM queue qu WHERE action = %1").arg(OcItems::Star));
    while (query.next())
    {
        starItemHashes[query.value(1).toString()] = query.value(0).toString();
    }
    if (!starItemHashes.isEmpty())
    {

#ifdef QT_DEBUG
        qDebug() << "Dequeue starred items.";
#endif
        connect(this,SIGNAL(starredItemsSuccess(QStringList,QString)),this,SLOT(dequeueUnstarred()),Qt::UniqueConnection);
        starItems("star", starItemHashes);
        query.exec(QString("DELETE FROM queue WHERE action = %1").arg(OcItems::Star));
    } else  {
        dequeueUnstarred();
    }
}


void OcItems::dequeueUnstarred()
{
    QVariantMap starItemHashes;
    QSqlQuery query;

    query.exec(QString("SELECT qu.guidHash, (SELECT feedId FROM items WHERE guidHash = qu.guidHash) AS feedId FROM queue qu WHERE action = %1").arg(OcItems::Unstar));
    while (query.next())
    {
        starItemHashes[query.value(1).toString()] = query.value(0).toString();
    }
    if (!starItemHashes.isEmpty())
    {

#ifdef QT_DEBUG
        qDebug() << "Dequeue unstarred items.";
#endif
        connect(this,SIGNAL(starredItemsSuccess(QStringList,QString)),this,SLOT(dequeueFinish()),Qt::UniqueConnection);
        starItems("unstar", starItemHashes);
        query.exec(QString("DELETE FROM queue WHERE action = %1").arg(OcItems::Unstar));
    } else {
        emit dequeueFinished();
    }
}


void OcItems::dequeueFinish()
{
    emit dequeueFinished();
}

void OcItems::updateEventFeed(const QList<int> &newsFeedItems)
{

#if defined(MEEGO_EDITION_HARMATTAN)
    QSqlQuery query;

    for (int i = 0; i < newsFeedItems.size(); ++i) {

        query.exec(QString("SELECT it.title, "
                           "it.body, it.pubDate, "
                           "it.url, (SELECT title FROM feeds WHERE id = it.feedId) AS feedName, it.feedId, it.author "
                           "FROM items it WHERE it.id = %1;").arg(newsFeedItems.at(i)));
        if (query.next())
        {
            QString bodyText(query.value(0).toString() + ": " + query.value(1).toString().remove(QRegExp("<[^>]*>")));
            QString authorInFooter = "";
            if (query.value(6).toString() != "")
                authorInFooter = query.value(6).toString();

            qlonglong id  = MEventFeed::instance()->addItem(QString("icon-m-ocnews"),
                                            QString(query.value(4).toString()),
                                            bodyText,
                                            QStringList(),
                                            QDateTime::fromTime_t(query.value(2).toInt()),
                                            QString(authorInFooter),
                                            false,
                                            QUrl(QString("ocnews://").append(QString::number(newsFeedItems.at(i)))),
                                            QString("ocnews-feed-").append(query.value(5).toString()),
                                            QString(query.value(4).toString()));
#ifdef QT_DEBUG
        qDebug() << "Event ID: " << id;
#endif

        }
    }
#endif
}
