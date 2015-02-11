#include <QtNetwork>
#include "QsLog.h"
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
        QLOG_INFO() << "Can not request items: Device is in flight mode.";
        emit requestedItemsError(tr("Device is in flight mode."));
    } else {
        QLOG_INFO() << "Start to request items from server.";

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
        QVariantMap requestItemsResult = helper.jsonToQt(replyRequestItems->readAll());

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
        QString type = QUrlQuery(replyRequestItems->url()).queryItemValue("type");
        QString fid = QUrlQuery(replyRequestItems->url()).queryItemValue("id");
#else
        QString type = replyRequestItems->url().queryItemValue("type");
        QString fid = replyRequestItems->url().queryItemValue("id");
#endif

        replyRequestItems->deleteLater();

        if (requestItemsResult.isEmpty())
        {
            QLOG_ERROR() << "Failed to request items: Server reply was empty.";
            notify.showNotification(tr("Server reply was empty."), tr("Error in server reply"), OcNotifications::Error);
            emit requestedItemsError(tr("Server reply was empty."));

        } else {
            itemsRequestedUpdateDb(requestItemsResult, type, fid);
        }

    } else {
        notify.showNotification(replyRequestItems->errorString(), tr("HTTP error"), OcNotifications::Error);
        emit requestedItemsError(replyRequestItems->errorString());

        QLOG_ERROR() << "Failed to request items: " << replyRequestItems->errorString();

        replyRequestItems->deleteLater();
    }
}

void OcItems::itemsRequestedUpdateDb(const QVariantMap &requestItemsResult, const QString &type, const QString &fId)
{
    QLOG_INFO() << "Updating database after requesting items from server.";

    QList<int> newEventItems; // list for the new items, for sending to event feed
    QList<int> newItems; // list for fetching images
    QList<int> updatedItems;
    int unreadCount = 0;
    QStringList feedsForEventsList = config.value("event/feeds", "").toString().split(",");

    QLOG_DEBUG() << "Request items: Event view feeds list: " << feedsForEventsList;

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
                QLOG_DEBUG() << "Updating item ID " << map["id"].toInt() << " | " << map["lastModified"].toInt();

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
                if (!query.exec()) {
                    QLOG_ERROR() << "Updating item database error: " << query.lastError().text();
                }
            }

        } else { // if item is not in database, create it there

            QLOG_DEBUG() << "Creating new item ID: " << map["id"].toInt();

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
            if (!query.exec()) {
                QLOG_ERROR() << "Creating item database error: " << query.lastError().text();
            }
        }

    }
    QSqlDatabase::database().commit();

    QLOG_INFO() << "New items for EventView: " << newEventItems;

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

        QLOG_DEBUG() << "Deleted item ID: " << idListDeleted.at(i);
    }
    QSqlDatabase::database().commit();

    if ((unreadCount > 0) && config.value("notifications/newItems", false).toBool())
    {
        notify.showNotification(tr("%n new unread item(s)", "", unreadCount), tr("New articles available"), OcNotifications::Success);
    }

    if (!newEventItems.isEmpty())
        updateEventFeed(newEventItems);

    emit requestedItemsSuccess(updatedItems, newItems, idListDeleted);
}




void OcItems::updateItems(const QString &lastModified, const QString &type, const QString &id)
{
    QString t_lastModified = lastModified;

    if (network.isFlightMode())
    {
        QLOG_INFO() << "Can not update items: Device is in flight mode.";
        emit updatedItemsError(tr("Device is in flight mode."));

    } else {

        QSqlQuery query;

        // work queued items
        if (!query.exec("SELECT id FROM queue LIMIT 1")) {
            QLOG_ERROR() << "Updating items DB error while selecting queued items: " << query.lastError().text();
        }

        if (query.next())
        {
            QEventLoop loop;
            connect(this,SIGNAL(dequeueFinished()),&loop,SLOT(quit()));
            dequeueRead();
            loop.exec();
        }

        QLOG_INFO() << "Start updating items from Server: Last modified: " << t_lastModified << " | Update type: " << type << " | ID: " << id;

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
                    t_lastModified = config.value("storage/lastFullUpdate", "").toString();
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

        QLOG_DEBUG() << "Update items: Last Modified: " << t_lastModified;

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

        QVariantMap updateItemsResult = helper.jsonToQt(replyUpdateItems->readAll());

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
        QString type = QUrlQuery(replyUpdateItems->url()).queryItemValue("type");
        QString id = QUrlQuery(replyUpdateItems->url()).queryItemValue("id");
#else
        QString type = replyUpdateItems->url().queryItemValue("type");
        QString id = replyUpdateItems->url().queryItemValue("id");
#endif

        replyUpdateItems->deleteLater();

        if (updateItemsResult.isEmpty())
        {
            QLOG_ERROR() << "Failed to request updated items from server: Server reply was empty.";
            notify.showNotification(tr("Server reply was empty."), tr("Error in server reply"), OcNotifications::Error);
            emit updatedItemsError(tr("Server reply was empty."));
        } else {
            itemsUpdatedUpdateDb(updateItemsResult, type, id);
        }

    } else {

        QLOG_ERROR() << "Failed to request updated items from server: " << replyUpdateItems->errorString();

        notify.showNotification(replyUpdateItems->errorString(), tr("HTTP error"), OcNotifications::Error);
        emit updatedItemsError(replyUpdateItems->errorString());
    }
}



void OcItems::itemsUpdatedUpdateDb(const QVariantMap &updateItemsResult, const QString &type, const QString &fid)
{
    QLOG_INFO() << "Updating database after requesting updated items.";

    QList<int> newEventItems; // list for the new items, for sending to event feed
    QList<int> newItems; // list for fetching images
    QList<int> updatedItems;
    int unreadCount = 0;
    QStringList feedsForEventsList = config.value("event/feeds", "").toString().split(",");


    QLOG_INFO() << "Updating items: Event view list: " << feedsForEventsList;

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
                QLOG_DEBUG() << "Update Item ID:" << map["id"].toInt() << "|" << map["lastModified"].toInt();

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
                if (!query.exec()) {
                    QLOG_ERROR() << "Error while updating DB after updating item: " << query.lastError().text();
                }
            }

        } else { // if item is not in database, create it there

            QLOG_DEBUG() << "Update items: Create item ID: " << map["id"].toInt();

            newItems.append(map["id"].toInt());

            if (map["unread"].toBool())
                unreadCount++;

            // collect ids of new items for the event feed
            if (!feedsForEventsList.isEmpty())
            {
                if (feedsForEventsList.contains(map["feedId"].toString()))
                    newEventItems.append(map["id"].toInt());
            }

            QLOG_DEBUG() << "Item updates: New items for EventView: " << newEventItems;

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
            if (!query.exec()) {
                QLOG_ERROR() << "Database error while updating item: " << query.lastError().text();
            }
        }

    }

    QSqlDatabase::database().commit();


    // set lastFullUpdate in settings
    if (type == "3")
    {
        QDateTime ts;
        config.setValue("storage/lastFullUpdate", ts.currentDateTimeUtc().toTime_t());
    }



    // now delete Items that exceed the maximum amount of items to store per feed
    // get feed ids
    QLOG_INFO() << "Updating items, start to clean database.";


    QList<int> imageItemIdsToDelete;

    if (type != "2")
    {
        QList<int> feedIds;
        int lowestId = 0;

        if (type == "0")
        {
            feedIds << fid.toInt();
        } else if (type == "1") {
            if (!query.exec(QString("SELECT id FROM feeds WHERE folderId = %1").arg(fid.toInt()))) {
                QLOG_ERROR() << "Update items: database error: " << query.lastError().text();
            }
            while (query.next())
                feedIds << query.value(0).toInt();
        } else if (type == "3") {
            if (!query.exec("SELECT id FROM feeds;")) {
                QLOG_ERROR() << "Update items: database error: " << query.lastError().text();
            }
            while (query.next())
                feedIds << query.value(0).toInt();
        }

        // get lowest itemID
        QSqlDatabase::database().transaction();
        for (int i = 0; i < feedIds.size(); ++i)
        {
            if (!query.exec(QString("SELECT MIN(id) FROM (SELECT id FROM items WHERE feedId = %1 ORDER BY id DESC LIMIT %2);").arg(feedIds.at(i)).arg(config.value("storage/maxitems", 100).toInt()))) {
                QLOG_ERROR() << "Update items: Error while getting lowest ID from database: " << query.lastError().text();
            }
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

    if ((unreadCount > 0) && config.value("notifications/newItems", false).toBool())
    {
        notify.showNotification(tr("%n new unread item(s)", "", unreadCount), tr("New articles available"), OcNotifications::Success);
    }

    if (!newEventItems.isEmpty())
        updateEventFeed(newEventItems);

    emit updatedItemsSuccess(updatedItems, newItems, imageItemIdsToDelete);
}








void OcItems::markItemsTillThis(const QString &action, const QDBusVariant &pubDate, const QDBusVariant &feedId)
{
    QVariant t_pubDate = pubDate.variant();
    QVariant t_feedId = feedId.variant();

    QLOG_INFO() << "Mark items till this: Action: " << action << " | Pubdate: " << t_pubDate << " | Feed ID: " << t_feedId;

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

    if (!query.exec(QString("SELECT id FROM items WHERE pubDate <= %1 AND feedId = %2 AND unread = %3").arg(t_pubDate.toInt()).arg(t_feedId.toInt()).arg(readStatus))) {
        QLOG_ERROR() << "Mark items till this: Error while selecting IDs from items: " << query.lastError().text();
    }

    QVariantList itemIds;
    while(query.next())
        itemIds << query.value(0).toString();

    QLOG_DEBUG() << "Mark items till this: Item IDs: " << itemIds;

    if (!itemIds.isEmpty()) {
        markItems(action, itemIds);
    } else {
        emit markedItemsSuccess(QStringList(), QString());
    }

}




void OcItems::markItems(const QString &action, const QVariantList &ids)
{

    QLOG_INFO() << "Start to request marking items: action: " << action;

    if (ids.isEmpty()) {
        QLOG_INFO() << "No items to mark.";
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

    QLOG_DEBUG() << "Items to mark: " << itemsToMark;

    if (network.isFlightMode())
    {

        QLOG_INFO() << "Mark items: Device is in flight mode. Add items to queue.";
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
        QLOG_INFO() << "Successfully requested the marking of items on the server.";
        itemsMarkedUpdateDb(itemsToMark);
    } else {

        QLOG_ERROR() << "Failed to mark items: " << replyMarkItems->errorString();

        if (replyMarkItems->error() == QNetworkReply::TimeoutError || replyMarkItems->error() == QNetworkReply::UnknownNetworkError)
        {
            QLOG_INFO() << "Queue items to mark: " << itemsToMark;

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
            QLOG_ERROR() << "Failed to request the marking of items: " << replyMarkItems->errorString();

            notify.showNotification(replyMarkItems->errorString(), tr("Failed to mark articles"), OcNotifications::Error);
            emit markedItemsError(replyMarkItems->errorString());
        }
    }

    replyMarkItems->deleteLater();
}


void OcItems::itemsMarkedUpdateDb(const QStringList &ids)
{
    QLOG_INFO() << "Updating DB after marking items.";

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

    if (QSqlDatabase::database().lastError().type() !=  QSqlError::NoError) {
        QLOG_ERROR() << "Marking items DB error: " << QSqlDatabase::database().lastError().text();
    }

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

    if (network.isFlightMode())
    {
        QLOG_INFO() << "Can not star or unstar items: Device is in flight mode. Adding action and items to queue.";

        if (action == "star")
        {
            queueItems(OcItems::Star, hashesToStar);
        } else {
            queueItems(OcItems::Unstar, hashesToStar);
        }

        hashesToStar << action;

        itemsStarredUpdateDb(hashesToStar);

    } else {

        QLOG_INFO() << "Starting a request to star or unstar items: " << action;

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

        QLOG_INFO() << "Successfully starred or unstarred items.";

        itemsStarredUpdateDb(hashesToStar);

    } else {

        QLOG_ERROR() << "Failed to star or unstar items: " << replyStarItems->errorString();

        if (replyStarItems->error() == QNetworkReply::TimeoutError || replyStarItems->error() == QNetworkReply::UnknownNetworkError)
        {

            QLOG_INFO() << "Adding items to star/unstar to queue.";

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
            QLOG_ERROR() << "Failed to un/star items: " << replyStarItems->errorString();
            notify.showNotification(replyStarItems->errorString(), tr("Failed to un/star articles"), OcNotifications::Error);
            emit starredItemsError(replyStarItems->errorString());
        }

        replyStarItems->deleteLater();
    }
}

void OcItems::itemsStarredUpdateDb(const QStringList &hashes)
{
    QLOG_INFO() << "Updating database after un/staring items.";

    QStringList t_hashes = hashes;
    QString action = t_hashes.last();
    t_hashes.removeLast();

    QLOG_DEBUG() << "Hashes to un/star: " << t_hashes;

    QDateTime ts;
    QSqlQuery query;
    if (action == "star")
    {
        for (int i = 0; i < t_hashes.size(); ++i)
            if (!query.exec(QString("UPDATE items SET starred = %3, lastModified = %2  WHERE guidHash = \"%1\";").arg(t_hashes.at(i)).arg(ts.currentDateTimeUtc().toTime_t()).arg(SQL_TRUE))) {
                QLOG_ERROR() << "Failed to update database after staring item: " << query.lastError().text();
            }

    } else {
        for (int i = 0; i < t_hashes.size(); ++i)
            if (!query.exec(QString("UPDATE items SET starred = %3, lastModified = %2 WHERE guidHash = \"%1\";").arg(t_hashes.at(i)).arg(ts.currentDateTimeUtc().toTime_t()).arg(SQL_FALSE))) {
                QLOG_ERROR() << "Failed to update database after unstaring item: " << query.lastError().text();
            }
    }

    emit starredItemsSuccess(t_hashes, action);
}





void OcItems::markAllItemsRead()
{
    markAllReadNewestId.clear();

    if (network.isFlightMode())
    {
        QLOG_INFO() << "Can not mark all items as read: Device is in flight mode.";

        emit markedAllItemsReadError(tr("Device is in flight mode."));

    } else {

        QLOG_INFO() << "Starting to request the marking of all items as read.";

        // get newest item id
        QSqlQuery query;
        query.exec("SELECT MAX(id) FROM items;");
        if (query.next())
            markAllReadNewestId = query.value(0).toString();

        // build request parameters
        QByteArray parameters("{\"newestItemId\": ");
        parameters.append(markAllReadNewestId);
        parameters.append("}");

        replyMarkAllItemsRead = network.put(helper.buildRequest("items/read", parameters.length()), parameters);

        connect(replyMarkAllItemsRead, SIGNAL(finished()), this, SLOT(itemsMarkedAllRead()));
    }

}

void OcItems::itemsMarkedAllRead()
{
    if (replyMarkAllItemsRead->error() == QNetworkReply::NoError)
    {
        replyMarkAllItemsRead->deleteLater();

        QLOG_INFO() << "Successfully requested the marking of all items as read.";

        itemsMarkedAllReadUpdateDb(markAllReadNewestId);

    } else {

        QLOG_ERROR() << "Failed to request the marking of all items as read: " << replyMarkAllItemsRead->errorString();

        notify.showNotification(replyMarkAllItemsRead->errorString(), tr("Failed to mark articles"), OcNotifications::Error);
        emit markedAllItemsReadError(replyMarkAllItemsRead->errorString());
        replyMarkAllItemsRead->deleteLater();
    }
}

void OcItems::itemsMarkedAllReadUpdateDb(const QString &newestItemId)
{
    QLOG_INFO() << "Updating database after marking all items as read.";

    QDateTime ts;
    QSqlQuery query;
    if (!query.exec(QString("UPDATE items SET unread = %3, lastModified = %2 WHERE id <= %1 AND unread = %4;").arg(newestItemId.toInt()).arg(ts.currentDateTimeUtc().toTime_t()).arg(SQL_FALSE).arg(SQL_TRUE))) {
        QLOG_ERROR() << "Failed to update database after marking all items as read: " << query.lastError().text();
    }

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
    Q_UNUSED(updated);
    Q_UNUSED(newFeeds);
    if (!deleted.isEmpty())
    {
        QList<int> itemsToDelete;

        for (int i = 0; i < deleted.size(); ++i)
        {
            QLOG_DEBUG() << "Clean items: Deleting feed ID: " << deleted.at(i);

            QSqlQuery query;
            if (!query.exec(QString("SELECT id FROM items WHERE feedId = %1;").arg(deleted.at(i)))) {
                QLOG_ERROR() << "Failed to select item IDs to delete from database: " << query.lastError().text();
            }

            while(query.next())
            {
                itemsToDelete << query.value(0).toInt();
            }

            if (!query.exec(QString("DELETE FROM items WHERE feedId = %1;").arg(deleted.at(i)))) {
                QLOG_ERROR() << "Failed to delete items in DB: " << query.lastError().text();
            }

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

    QLOG_DEBUG() << "Queue item IDs: " << itemIds;

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

    if (!query.exec("SELECT id FROM queue LIMIT 1")) {
        QLOG_ERROR() << "Failed to select item ID from DB: " << query.lastError().text();
    }

    if (query.next())
    {
        QLOG_INFO() << "Start dequeueing items.";

        dequeueRead();
    } else {
        dequeueFinish();
    }
}


void OcItems::dequeueRead()
{
    QVariantList markItemIds;
    QSqlQuery query;

    if (!query.exec(QString("SELECT itemId FROM queue WHERE action = %1").arg(OcItems::MarkRead))) {
        QLOG_ERROR() << "Failed to select item IDs from queue DB table: " << query.lastError().text();
    }

    while (query.next())
    {
        markItemIds << query.value(0);
    }
    if (!markItemIds.isEmpty())
    {
        QLOG_INFO() << "Dequeeue read items.";

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

    if (!query.exec(QString("SELECT itemId FROM queue WHERE action = %1").arg(OcItems::MarkUnread))) {
        QLOG_ERROR() << "Failed to select item IDs from queue DB table: " << query.lastError().text();
    }

    while (query.next())
    {
        markItemIds << query.value(0);
    }
    if (!markItemIds.isEmpty())
    {
        QLOG_INFO() << "Dequeue unread items.";

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

    if (!query.exec(QString("SELECT qu.guidHash, (SELECT feedId FROM items WHERE guidHash = qu.guidHash) AS feedId FROM queue qu WHERE action = %1").arg(OcItems::Star))) {
        QLOG_ERROR() << "Failed to select item hashes from queue DB table: " << query.lastError().text();
    }

    while (query.next())
    {
        starItemHashes[query.value(1).toString()] = query.value(0).toString();
    }
    if (!starItemHashes.isEmpty())
    {
        QLOG_INFO() << "Dequeue starred items.";

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

    if (!query.exec(QString("SELECT qu.guidHash, (SELECT feedId FROM items WHERE guidHash = qu.guidHash) AS feedId FROM queue qu WHERE action = %1").arg(OcItems::Unstar))) {
        QLOG_ERROR() << "Failed to select item hashes from queue DB table: " << query.lastError().text();
    }

    while (query.next())
    {
        starItemHashes[query.value(1).toString()] = query.value(0).toString();
    }
    if (!starItemHashes.isEmpty())
    {
        QLOG_INFO() << "Dequeue unstarred items.";

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

            QLOG_DEBUG() << "New ID for even view: " << id;
        }
    }
#else
    Q_UNUSED(newsFeedItems); // has to be removed when SailfishOS supports it.
#endif
}
