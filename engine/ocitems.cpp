#include <QtNetwork>
#include "ocitems.h"
#include "../common/globals.h"

OcItems::OcItems(QObject *parent) :
    QObject(parent)
{
    itemsToFetchImages = 0;
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

        urlRequestItems = helper.buildUrl("items");

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
        QUrlQuery query;
        query.addQueryItem("batchSize", batchSize);
        query.addQueryItem("offset", offset);
        query.addQueryItem("type", type);
        query.addQueryItem("id", id);
        query.addQueryItem("getRead", getRead);
        urlRequestItems.setQuery(query);
#else
        urlRequestItems.addQueryItem("batchSize", batchSize);
        urlRequestItems.addQueryItem("offset", offset);
        urlRequestItems.addQueryItem("type", type);
        urlRequestItems.addQueryItem("id", id);
        urlRequestItems.addQueryItem("getRead", getRead);
#endif

        replyRequestItems = network.get(QNetworkRequest(urlRequestItems));

        connect(replyRequestItems,SIGNAL(finished()),this,SLOT(itemsRequested()));
    }

}


void OcItems::itemsRequested()
{
    connect(this,SIGNAL(requestedItems(QVariantMap, QString, QString)),this,SLOT(itemsRequestedUpdateDb(QVariantMap, QString, QString)), Qt::UniqueConnection);

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
            emit requestedItemsError(tr("Server reply was empty."));
        } else {
            emit requestedItems(requestItemsResult, type, fid);
        }

    } else {
        emit requestedItemsError(replyRequestItems->errorString());
#ifdef QT_DEBUG
        qDebug() << "HTTP-Error:" << replyRequestItems->errorString();
#endif
    }
}

void OcItems::itemsRequestedUpdateDb(QVariantMap requestItemsResult, QString type, QString fId)
{
#ifdef QT_DEBUG
    qDebug() << "Start updating items database.";
#endif

    int imgHandling = config.getSetting(QString("display/handleimgs"), QDBusVariant(0)).variant().toInt();
    QList<int> newEventItems; // list for the new items, for sending to event feed
    QList<int> newItems; // list for fetching images
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
            if (imgHandling == 2)
            {
                query.bindValue(":body", map["body"].toString());
            } else {
                query.bindValue(":body", cacheImages(map["body"].toString(), map["id"].toInt(), imgHandling));
            }
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
#if defined(MEEGO_EDITION_HARMATTAN)
        query.exec(QString("SELECT id FROM items where starred = \"true\";"));
#else
        query.exec(QString("SELECT id FROM items where starred = 1;"));
#endif
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
    emit requestedItemsSuccess();

    if (!newEventItems.isEmpty())
        updateEventFeed(newEventItems);

    if (imgHandling == 2 && !newItems.isEmpty())
        fetchImages(newItems);
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
#if defined(MEEGO_EDITION_HARMATTAN)
                    querystring = QString("SELECT MAX(lastModified) FROM items WHERE starred = \"true\";");
#else
                    querystring = QString("SELECT MAX(lastModified) FROM items WHERE starred = 1;");
#endif
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

        urlUpdateItems = helper.buildUrl("items/updated");

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
        QUrlQuery urlQuery;
        urlQuery.addQueryItem("lastModified", t_lastModified);
        urlQuery.addQueryItem("type", type);
        urlQuery.addQueryItem("id", id);
        urlUpdateItems.setQuery(urlQuery);
#else
        urlUpdateItems.addQueryItem("lastModified", t_lastModified);
        urlUpdateItems.addQueryItem("type", type);
        urlUpdateItems.addQueryItem("id", id);
#endif

        replyUpdateItems = network.get(QNetworkRequest(QUrl(urlUpdateItems)));

        connect(replyUpdateItems,SIGNAL(finished()),this,SLOT(itemsUpdated()));
    }
}



void OcItems::itemsUpdated()
{
    connect(this,SIGNAL(updatedItems(QVariantMap, QString, QString)),this,SLOT(itemsUpdatedUpdateDb(QVariantMap, QString, QString)),Qt::UniqueConnection);

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
        qDebug() << "Finished parsing.";
#endif

        if (updateItemsResult.isEmpty())
        {
            emit updatedItemsError(tr("Server reply was empty."));
        } else {
            emit updatedItems(updateItemsResult, type, id);
        }

    } else {
#ifdef QT_DEBUG
        qDebug() << "HTTP-Error:" << replyUpdateItems->errorString();
#endif
        emit updatedItemsError(replyUpdateItems->errorString());
    }
}



void OcItems::itemsUpdatedUpdateDb(QVariantMap updateItemsResult, QString type, QString fid)
{
#ifdef QT_DEBUG
    qDebug() << "Start updating database.";
#endif

    QList<int> newEventItems; // list for the new items, for sending to event feed
    QList<int> newItems; // list for fetching images
    int imgHandling = config.getSetting(QString("display/handleimgs"), QDBusVariant(0)).variant().toInt();
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
            if (imgHandling == 2)
            {
                query.bindValue(":body", map["body"].toString());
            } else {
                query.bindValue(":body", cacheImages(map["body"].toString(), map["id"].toInt(), imgHandling));
            }
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

    if (type != "2")
    {
        QList<int> feedIds;
        QList<int> imageItemIds;
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
#if defined(MEEGO_EDITION_HARMATTAN)
            query.exec(QString("SELECT id FROM items WHERE starred = \"false\" AND id < %1 AND feedId = %2;").arg(lowestId).arg(feedIds.at(i)));
#else
            query.exec(QString("SELECT id FROM items WHERE starred = 0 AND id < %1 AND feedId = %2;").arg(lowestId).arg(feedIds.at(i)));
#endif
            while(query.next())
            {
                imageItemIds << query.value(0).toInt();
            }


#if defined(MEEGO_EDITION_HARMATTAN)
            query.exec(QString("DELETE FROM items WHERE starred = \"false\" AND id < %1 AND feedId = %2;").arg(lowestId).arg(feedIds.at(i)));
#else
            query.exec(QString("DELETE FROM items WHERE starred = 0 AND id < %1 AND feedId = %2;").arg(lowestId).arg(feedIds.at(i)));
#endif

        }
        QSqlDatabase::database().commit();

        deleteCachedImages(imageItemIds);
    }
#ifdef QT_DEBUG
    qDebug() << "Emit updatedItemsSuccess";
#endif
    emit updatedItemsSuccess();

    if (!newEventItems.isEmpty())
        updateEventFeed(newEventItems);

    if (imgHandling == 2 && !newItems.isEmpty())
        fetchImages(newItems);
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
#if defined(MEEGO_EDITION_HARMATTAN)
    query.exec(QString("SELECT id FROM items WHERE pubDate <= %1 AND feedId = %2 AND unread = \"%3\"").arg(t_pubDate.toInt()).arg(t_feedId.toInt()).arg(readStatus));
#else
    query.exec(QString("SELECT id FROM items WHERE pubDate <= %1 AND feedId = %2 AND unread = %3").arg(t_pubDate.toInt()).arg(t_feedId.toInt()).arg(readStatus));
#endif

#ifdef QT_DEBUG
    qDebug() << query.lastError();
#endif

    QVariantList itemIds;
    while(query.next())
        itemIds << query.value(0).toString();

#ifdef QT_DEBUG
    qDebug() << itemIds;
#endif

    if (!itemIds.isEmpty())
        markItems(action, itemIds);

}


void OcItems::markItems(const QString &action, const QVariantList &ids)
{
#ifdef QT_DEBUG
    qDebug() << "markItems: " << ids;
#endif

    // Create the JSON string and convert QVariantList to QStringList
    QStringList slIds;
    QByteArray parameters("{\"items\": [");
    for (int i = 0; i < ids.size(); ++i)
    {
        if (i != 0)
            parameters.append(", ");
        parameters.append(ids.at(i).toString());
        slIds.append(ids.at(i).toString());
    }
    parameters.append("]}");

#ifdef QT_DEBUG
    qDebug() << "markItems: " << slIds;
    qDebug() << parameters;
#endif

    slIds << action;

    if (network.isFlightMode())
    {
        connect(this,SIGNAL(markedItems(QStringList)),this,SLOT(itemsMarkedUpdateDb(QStringList)),Qt::UniqueConnection);

        QStringList queueItemIds = slIds;
        queueItemIds.removeLast();

        if (action == "read")
        {
            queueItems(OcItems::MarkRead, queueItemIds);
        } else {
            queueItems(OcItems::MarkUnread, queueItemIds);
        }
#ifdef QT_DEBUG
    qDebug() << "markItems Flight Mode: " << queueItemIds;
#endif

        emit markedItems(slIds);

    } else {

        if (action == "read")
        {
            urlMarkItems = helper.buildUrl("items/read/multiple");
        } else {
            urlMarkItems = helper.buildUrl("items/unread/multiple");
        }

        // prepare ids and action for signal sending
        QString joinedIds = slIds.join(",");

        // Calculate content length header
        QByteArray postDataSize = QByteArray::number(parameters.size());

        // Building the request
        QNetworkRequest request(urlMarkItems);

        // Add the headers
        request.setRawHeader("Content-Type", "application/json; charset=utf-8");
        request.setRawHeader("Content-Length", postDataSize);

        replyMarkItems = network.put(request, parameters);

        // map the name into the signal
        QSignalMapper *signalMapper = new QSignalMapper(this);
        signalMapper->setMapping(replyMarkItems, joinedIds);

        connect(replyMarkItems, SIGNAL(finished()), signalMapper, SLOT(map()));

        connect(signalMapper, SIGNAL(mapped(QString)), this, SLOT(itemsMarked(QString)));
    }

}

void OcItems::itemsMarked(QString joinedIds)
{
#ifdef QT_DEBUG
    qDebug() << joinedIds;
#endif

    connect(this,SIGNAL(markedItems(QStringList)),this,SLOT(itemsMarkedUpdateDb(QStringList)),Qt::UniqueConnection);

    if (replyMarkItems->error() == QNetworkReply::NoError)
    {
        QStringList ids = joinedIds.split(",");

        emit markedItems(ids);
    } else {

#ifdef QT_DEBUG
        qDebug() << "HTTP-Error:" << replyMarkItems->errorString();
        qDebug() << replyMarkItems->error();
#endif

        if (replyMarkItems->error() == QNetworkReply::TimeoutError || replyMarkItems->error() == QNetworkReply::UnknownNetworkError)
        {
            QStringList ids = joinedIds.split(",");
            QString action = ids.last();
            QStringList queueItemIds = ids;
            queueItemIds.removeLast();

            if (action == "read")
            {
                queueItems(OcItems::MarkRead, queueItemIds);
            } else {
                queueItems(OcItems::MarkUnread, queueItemIds);
            }

            emit markedItems(ids);
        } else {
            emit markedItemsError(replyMarkItems->errorString());
        }
    }
    replyMarkItems->deleteLater();
}


void OcItems::itemsMarkedUpdateDb(QStringList ids)
{
#ifdef QT_DEBUG
    qDebug() << "markItems Update DB: " << ids;
#endif
    QString action = ids.last();
    ids.removeLast();

    QSqlQuery query;
    QDateTime ts;
    QSqlDatabase::database().transaction();
    if (action == "read")
    {
        for (int i = 0; i < ids.size(); ++i)
#if defined(MEEGO_EDITION_HARMATTAN)
            query.exec(QString("UPDATE items SET unread = \"false\", lastModified = %2 WHERE id = %1;").arg(ids.at(i)).arg(ts.currentDateTimeUtc().toTime_t()));
#else
            query.exec(QString("UPDATE items SET unread = 0, lastModified = %2 WHERE id = %1;").arg(ids.at(i)).arg(ts.currentDateTimeUtc().toTime_t()));
#endif
    } else {
        for (int i = 0; i < ids.size(); ++i)
#if defined(MEEGO_EDITION_HARMATTAN)
            query.exec(QString("UPDATE items SET unread = \"true\", lastModified = %2 WHERE id = %1;").arg(ids.at(i)).arg(ts.currentDateTimeUtc().toTime_t()));
#else
            query.exec(QString("UPDATE items SET unread = 1, lastModified = %2 WHERE id = %1;").arg(ids.at(i)).arg(ts.currentDateTimeUtc().toTime_t()));
#endif
    }

    QSqlDatabase::database().commit();
#ifdef QT_DEBUG
    qDebug() << "markItems DB Error: " <<  QSqlDatabase::database().lastError();
#endif

    emit markedItemsSuccess();
}





void OcItems::starItems(const QString &action, const QVariantMap &itemIds)
{

    QStringList guidHashes;

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
        guidHashes << i.value().toString();
        count++;
    }
    parameters.append("]}");

#ifdef QT_DEBUG
    qDebug() << parameters;
#endif


    guidHashes << action;


    if (network.isFlightMode())
    {
        connect(this,SIGNAL(starredItems(QStringList)),this,SLOT(itemsStarredUpdateDb(QStringList)),Qt::UniqueConnection);

        QStringList queueHashes = guidHashes;
        queueHashes.removeLast();

        if (action == "star")
        {
            queueItems(OcItems::Star, queueHashes);
        } else {
            queueItems(OcItems::Unstar, queueHashes);
        }

        emit starredItems(guidHashes);

    } else {

        if (action == "star")
        {
            urlStarItems = helper.buildUrl("items/star/multiple");
        } else {
            urlStarItems = helper.buildUrl("items/unstar/multiple");
        }


        QString joinedHashes = guidHashes.join(",");

        // Calculate content length header
        QByteArray postDataSize = QByteArray::number(parameters.size());

        // Building the request
        QNetworkRequest request(urlStarItems);

        // Add the headers
        request.setRawHeader("Content-Type", "application/json; charset=utf-8");
        request.setRawHeader("Content-Length", postDataSize);

        replyStarItems = network.put(request, parameters);

        // map the hashes into the signal
        QSignalMapper *signalMapper = new QSignalMapper(this);
        signalMapper->setMapping(replyStarItems, joinedHashes);

        connect(replyStarItems, SIGNAL(finished()), signalMapper, SLOT(map()));

        connect(signalMapper, SIGNAL(mapped(QString)), this, SLOT(itemsStarred(QString)));
    }
}

void OcItems::itemsStarred(QString joinedHashes)
{
#ifdef QT_DEBUG
    qDebug() << joinedHashes;
#endif
    connect(this,SIGNAL(starredItems(QStringList)),this,SLOT(itemsStarredUpdateDb(QStringList)),Qt::UniqueConnection);

    if (replyStarItems->error() == QNetworkReply::NoError)
    {
        QStringList hashes = joinedHashes.split(",");

        replyStarItems->deleteLater();

        emit starredItems(hashes);
    } else {
#ifdef QT_DEBUG
        qDebug() << "HTTP-Error:" << replyStarItems->errorString();
#endif
        if (replyStarItems->error() == QNetworkReply::TimeoutError || replyStarItems->error() == QNetworkReply::UnknownNetworkError)
        {
            QStringList hashes = joinedHashes.split(",");
            QString action = hashes.last();
            QStringList queueHashes = hashes;
            queueHashes.removeLast();

            if (action == "star")
            {
                queueItems(OcItems::Star, queueHashes);
            } else {
                queueItems(OcItems::Unstar, queueHashes);
            }
             emit starredItems(hashes);
        } else {
            emit starredItemsError(replyStarItems->errorString());
        }

        replyStarItems->deleteLater();
    }
}

void OcItems::itemsStarredUpdateDb(QStringList hashes)
{
    QString action = hashes.last();
    hashes.removeLast();
#ifdef QT_DEBUG
    qDebug() << hashes;
#endif

    QDateTime ts;
    QSqlQuery query;
    if (action == "star")
    {
        for (int i = 0; i < hashes.size(); ++i)
#if defined(MEEGO_EDITION_HARMATTAN)
            query.exec(QString("UPDATE items SET starred = \"true\", lastModified = %2  WHERE guidHash = \"%1\";").arg(hashes.at(i)).arg(ts.currentDateTimeUtc().toTime_t()));
#else
            query.exec(QString("UPDATE items SET starred = 1, lastModified = %2  WHERE guidHash = \"%1\";").arg(hashes.at(i)).arg(ts.currentDateTimeUtc().toTime_t()));
#endif
    } else {
        for (int i = 0; i < hashes.size(); ++i)
#if defined(MEEGO_EDITION_HARMATTAN)
            query.exec(QString("UPDATE items SET starred = \"false\", lastModified = %2 WHERE guidHash = \"%1\";").arg(hashes.at(i)).arg(ts.currentDateTimeUtc().toTime_t()));
#else
            query.exec(QString("UPDATE items SET starred = 0, lastModified = %2 WHERE guidHash = \"%1\";").arg(hashes.at(i)).arg(ts.currentDateTimeUtc().toTime_t()));
#endif
    }

    emit starredItemsSuccess();
}


void OcItems::markAllItemsRead()
{
    if (network.isFlightMode())
    {
        emit markedAllItemsReadError(tr("Device is in flight mode."));

    } else {
        // build request url
        urlMarkAllItemsRead = helper.buildUrl("items/read");

        // get newest item id
        QSqlQuery query;
        QString newestItemId;
        query.exec("SELECT MAX(id) FROM items;");
        if (query.next())
            newestItemId = query.value(0).toString();

        // build request parameters
        QByteArray parameters("{\"newestItemId\": ");
        parameters.append(newestItemId);
        parameters.append("}");

    #ifdef QT_DEBUG
        qDebug() << parameters;
    #endif

        // Calculate content length header
        QByteArray postDataSize = QByteArray::number(parameters.size());

        // Building the request
        QNetworkRequest request(urlMarkAllItemsRead);

        // Add the headers
        request.setRawHeader("Content-Type", "application/json; charset=utf-8");
        request.setRawHeader("Content-Length", postDataSize);

        replyMarkAllItemsRead = network.put(request, parameters);

        // map the hashes into the signal
        QSignalMapper *signalMapper = new QSignalMapper(this);
        signalMapper->setMapping(replyMarkAllItemsRead, newestItemId);

        connect(replyMarkAllItemsRead, SIGNAL(finished()), signalMapper, SLOT(map()));

        connect(signalMapper, SIGNAL(mapped(QString)), this, SLOT(itemsMarkedAllRead(QString)));
    }

}

void OcItems::itemsMarkedAllRead(QString newestItemId)
{
    connect(this,SIGNAL(markedAllItemsRead(QString)),this,SLOT(itemsMarkedAllReadUpdateDb(QString)),Qt::UniqueConnection);

    if (replyMarkAllItemsRead->error() == QNetworkReply::NoError)
    {
        replyMarkAllItemsRead->deleteLater();

        emit markedAllItemsRead(newestItemId);
    } else {
#ifdef QT_DEBUG
        qDebug() << "HTTP-Error:" << replyMarkAllItemsRead->errorString();
#endif
        emit markedAllItemsReadError(replyMarkAllItemsRead->errorString());
        replyMarkAllItemsRead->deleteLater();
    }
}

void OcItems::itemsMarkedAllReadUpdateDb(QString newestItemId)
{
    QDateTime ts;
    QSqlQuery query;
#if defined(MEEGO_EDITION_HARMATTAN)
    query.exec(QString("UPDATE items SET unread = \"false\", lastModified = %2 WHERE id <= %1 AND unread = \"true\";").arg(newestItemId.toInt()).arg(ts.currentDateTimeUtc().toTime_t()));
#else
    query.exec(QString("UPDATE items SET unread = 0, lastModified = %2 WHERE id <= %1 AND unread = 1;").arg(newestItemId.toInt()).arg(ts.currentDateTimeUtc().toTime_t()));
#endif

    emit markedAllItemsReadSuccess();
}


void OcItems::cleanItems(int id)
{
    QSqlQuery query;
    query.exec(QString("SELECT id FROM items WHERE feedId = %1;").arg(id));

    QList<int> imageIdsToDelete;
    while(query.next())
    {
        imageIdsToDelete << query.value(0).toInt();
    }

    query.exec(QString("DELETE FROM items WHERE feedId = %1;").arg(id));

    deleteCachedImages(imageIdsToDelete);

#if defined(MEEGO_EDITION_HARMATTAN)
    // remove items from event feed
    MEventFeed::instance()->removeItemsBySourceName(QString("ocnews-feed-").append(QString::number(id)));
#endif
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
        connect(this,SIGNAL(markedItemsSuccess()),this,SLOT(dequeueUnread()),Qt::UniqueConnection);
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
        connect(this,SIGNAL(markedItemsSuccess()),this,SLOT(dequeueStarred()),Qt::UniqueConnection);
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
        connect(this,SIGNAL(starredItemsSuccess()),this,SLOT(dequeueUnstarred()),Qt::UniqueConnection);
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
        connect(this,SIGNAL(starredItemsSuccess()),this,SLOT(dequeueFinish()),Qt::UniqueConnection);
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

#if defined(MEEGO_EDITION_HARMATTAN)
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
#endif
        }
    }
}



void OcItems::fetchImages(const QList<int> &newItems)
{

#ifdef QT_DEBUG
    qDebug() << "Start fetching images.";
#endif


    itemsToFetchImages = newItems.size();
    emit startedFetchingImages(itemsToFetchImages);

    for (int i = 0; i < newItems.size(); ++i)
    {
        emit fetchingImages(i);
        QString body = QString("");

        QSqlQuery query;
        query.exec(QString("SELECT body FROM items WHERE id = %1").arg(newItems.at(i)));

        if (query.next())
            body = cacheImages(query.value(0).toString(), newItems.at(i), 2);

        if (body != "")
        {
            QSqlQuery query2;
            query2.prepare("UPDATE items SET body = :body WHERE id = :id;");
            query2.bindValue(":body", body);
            query2.bindValue(":id", newItems.at(i));
            query2.exec();
        }

    }

    itemsToFetchImages = 0;
    emit finishedFetchingImages();

#ifdef QT_DEBUG
    qDebug() << "Finished fetching images.";
#endif

}



QString OcItems::cacheImages(const QString &bodyText, int id, int imageHandling)
{
    QString newBodyText = bodyText;
    QRegExp findImg("<img[^>]*>");
    QStringList foundImages;
    int pos = 0;

    while((pos = findImg.indexIn(bodyText, pos)) != -1)
    {
        foundImages << findImg.cap(0);
        pos += findImg.matchedLength();
    }
#ifdef QT_DEBUG
    qDebug() << "Found imgs: " << foundImages;
#endif

    if (!foundImages.isEmpty())
    {
        QSqlQuery query;

        for (int i = 0; i < foundImages.size(); ++i)
        {
            QVariantMap imageInfo;

            if (imageHandling != 2)
            {
                imageInfo = extractImgData(foundImages.at(i), false);
            }
            else
            {
                imageInfo = extractImgData(foundImages.at(i), true);

                QUrl fileUrl(imageInfo["src"].toString());
                QFileInfo fileInfo = fileUrl.path();
                QString fileName("_image_");
                fileName.prepend(QString::number(id));
                fileName.append(QString::number(i)).append(".");
                fileName.append(fileInfo.suffix());

                QString storagePath(QDir::homePath());
                storagePath.append(IMAGE_CACHE);
                storagePath.append(QDir::separator()).append(fileName);
                storagePath = QDir::toNativeSeparators(storagePath);
    #ifdef QT_DEBUG
                qDebug() << storagePath;
    #endif

                QEventLoop dlLoop;

                QNetworkRequest request(fileUrl);

                QNetworkReply *replyGetImage = network.get(request);

                connect(replyGetImage, SIGNAL(finished()), &dlLoop, SLOT(quit()));
                dlLoop.exec();

                if (replyGetImage->error() == QNetworkReply::NoError)
                {

                    QImage image;
                    if (image.loadFromData(replyGetImage->readAll()))
                    {

                        imageInfo["width"] = image.width();
                        imageInfo["height"] = image.height();

#ifdef QT_DEBUG
                        qDebug() << "Image Seize: " << imageInfo["width"].toInt() << " x " << imageInfo["height"].toInt();
#endif

                        if (image.save(storagePath))
                        {
                            qDebug() << "Replacing body text.";
                            newBodyText.replace(imageInfo["src"].toString(), storagePath, Qt::CaseSensitive);
                            imageInfo["src"] = storagePath;
                            replyGetImage->deleteLater();
                        }
                        else
                        {

                            qDebug() << "Try to save image with QFile method.";

                            QFile file(storagePath);
                            file.open(QIODevice::WriteOnly);

                            if (file.write(replyGetImage->readAll()) != -1)
                            {

                                newBodyText.replace(imageInfo["src"].toString(), storagePath, Qt::CaseSensitive);
                                imageInfo["src"] = storagePath;
                                replyGetImage->deleteLater();

                            } else {

                                qDebug() << "Failed to save image";
                                imageInfo = extractImgData(foundImages.at(i), false);
                                replyGetImage->deleteLater();

                            }

                        }

                    }
                    else
                    {
                        qDebug() << "Can not load image data.";
                        imageInfo = extractImgData(foundImages.at(i), false);
                        replyGetImage->deleteLater();
                    }
                }
                else
                {
                    qDebug() << "Can not download image file.";
                    imageInfo = extractImgData(foundImages.at(i), false);
                    replyGetImage->deleteLater();
                }
            }

            query.prepare("INSERT INTO images (parentId, path, width, height) VALUES (:parentId, :path, :width, :height);");
            query.bindValue(":parentId", id);
            query.bindValue(":path", imageInfo["src"]);
            query.bindValue(":width", imageInfo["width"]);
            query.bindValue(":height", imageInfo["height"]);
            query.exec();
        }
    }

    return newBodyText;
}


QVariantMap OcItems::extractImgData(const QString &imgStr, bool srcOnly)
{
    QVariantMap result;

    QRegExp findSrc("(http|ftp)[^\"]*");
    imgStr.contains(findSrc);
    result["src"] =  findSrc.cap(0);

    if (!srcOnly)
    {
        QRegExp findWidth("width=\"\\d+\"");
        QRegExp findHeight("height=\"\\d+\"");

        imgStr.contains(findWidth);
        imgStr.contains(findHeight);

        QString widthString = findWidth.cap(0);
        QString heightString = findHeight.cap(0);

        widthString.chop(1);
        widthString.remove(0, 7);
        heightString.chop(1);
        heightString.remove(0, 8);

        result["width"] = widthString.toInt();
        result["height"] = heightString.toInt();
    }

#ifdef QT_DEBUG
    qDebug() << "Image information: " << result;
#endif

    return result;
}


void OcItems::deleteCachedImages(const QList<int> &idsToDelte)
{
    QSqlQuery query;
    QSqlDatabase::database().transaction();
    for (int i = 0; i < idsToDelte.size(); ++i)
    {
        query.exec(QString("DELETE FROM images WHERE parentId = %1").arg(idsToDelte.at(i)));
    }
    QSqlDatabase::database().commit();

    for (int d = 0; d < idsToDelte.size(); ++d)
    {

        QStringList nameFilter;
        QString fileFilter("_*");
        fileFilter.prepend(QString::number(idsToDelte.at(d)));
        nameFilter << fileFilter;
        QString storagePath(QDir::homePath());
        storagePath.append(IMAGE_CACHE);
        QDir directory(storagePath);
        QStringList imgs = directory.entryList(nameFilter);

    #ifdef QT_DEBUG
        qDebug() << "Images to delete: " << imgs;
    #endif

        for (int i = 0; i < imgs.size(); ++i)
        {
            directory.remove(imgs.at(i));
        }
    }
}


int OcItems::isFetchImagesRunning()
{
    qDebug() << "Items is fetching images: " << itemsToFetchImages;
    return itemsToFetchImages;
}
