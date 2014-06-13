#include <QtNetwork>
#include "ocfolders.h"

OcFolders::OcFolders(QObject *parent) :
    QObject(parent)
{
    newFolderName = "";
}

void OcFolders::requestFolders()
{
    if (network.isFlightMode())
    {
        emit requestedFoldersError(tr("Device is in flight mode."));
    } else {
#ifdef QT_DEBUG
        qDebug() << "Start to fetch folders from server";
#endif

        replyRequestFolders = network.get(helper.buildRequest("folders"));

        connect(replyRequestFolders,SIGNAL(finished()),this,SLOT(foldersRequested()));
    }
}

void OcFolders::foldersRequested()
{
#ifdef QT_DEBUG
    qDebug() << "Folders requested";
#endif

    if (replyRequestFolders->error() == QNetworkReply::NoError)
    {
        QVariantMap foldersresult = helper.jsonToQt(replyRequestFolders->readAll());

#ifdef QT_DEBUG
        qDebug() << foldersresult;
#endif
        if (foldersresult.isEmpty())
        {
            emit requestedFoldersError(tr("Server reply was empty."));

        } else {

#ifdef QT_DEBUG
            qDebug() << "Start updating DB with requested folders";
#endif
            foldersRequestedUpdateDb(foldersresult);

        }

    } else {
#ifdef QT_DEBUG
        qDebug() << "HTTP-Error:" << replyRequestFolders->errorString();
#endif
        emit requestedFoldersError(replyRequestFolders->errorString());
    }

    replyRequestFolders->deleteLater();

}

void OcFolders::foldersRequestedUpdateDb(const QVariantMap &foldersresult)
{
#ifdef QT_DEBUG
    qDebug() << "Update folder db";
#endif
    QSqlQuery query;

    foreach (QVariant folder, foldersresult["folders"].toList())
    {

        // remap QVariantMap
        QMap<QString, QVariant> map = folder.value<QVariantMap>();

        // check if folder is already in database
        query.exec(QString("SELECT id, name FROM folders WHERE id = %1").arg(map["id"].toInt()));
        if (query.next())
        {
            // check if folder has changed it's name
            if (query.value(1).toString() != map["name"].toString())
            {
                query.prepare("UPDATE folders SET name = :name WHERE id = :id;");
                query.bindValue(":name", map["name"].toString());
                query.bindValue(":id", map["id"].toInt());
                query.exec();
#ifdef QT_DEBUG
                qDebug() << "Updated folder: " << map["name"].toString();
#endif
            }

        } else { // if folder is not in database, create it there
            query.prepare("INSERT INTO folders (id, name) VALUES (:id, :name);");
            query.bindValue(":id", map["id"].toInt());
            query.bindValue(":name", map["name"].toString());
            query.exec();
#ifdef QT_DEBUG
            qDebug() << "Created folder: " << map["name"].toString();
#endif
        }


    }


    // now check if folders were deleted on server
    QList<int> idList;
    QList<int> idListDeleted;

    // put all the ids into a list
    foreach (QVariant folder, foldersresult["folders"].toList())
    {
        QMap<QString, QVariant> map = folder.value<QVariantMap>();

        idList << map["id"].toInt();
    }

    // compare the ids and put the deleted ids into a list
    query.exec(QString("SELECT id FROM folders;"));
    while (query.next())
    {
        if (!idList.contains(query.value(0).toInt()))
            idListDeleted << query.value(0).toInt();
    }

    // delete the serverside deleted ids in the database
    if (!idListDeleted.isEmpty())
    {
        QSqlDatabase::database().transaction();
        for (int i = 0; i < idListDeleted.size(); ++i) {
            query.exec(QString("DELETE FROM folders WHERE id = %1").arg(idListDeleted.at(i)));
    #ifdef QT_DEBUG
            qDebug() << "Deleted Folder ID: " << idListDeleted.at(i);
    #endif
        }
        QSqlDatabase::database().commit();
    }

#ifdef QT_DEBUG
    qDebug() << "emit requestedFolderSuccess";
#endif
    emit requestedFoldersSuccess();

}



void OcFolders::createFolder(const QString &name)
{
    if (network.isFlightMode())
    {
        emit createdFolderError(tr("Device is in flight mode."));
    } else {

        // Create the JSON string
        QByteArray parameters("{\"name\": \"");
        parameters.append(name);
        parameters.append("\"}");

        replyCreateFolder = network.post(helper.buildRequest("folders", parameters.length()), parameters);

        connect(replyCreateFolder,SIGNAL(finished()),this,SLOT(folderCreated()));
    }

}


void OcFolders::folderCreated()
{

    if (replyCreateFolder->error() == QNetworkReply::NoError)
    {
        QVariantMap createresult = helper.jsonToQt(replyCreateFolder->readAll());

        replyCreateFolder->deleteLater();

        if (createresult.isEmpty())
        {
            emit createdFolderError(tr("Server reply was empty."));
        } else {

            folderCreatedUpdateDb(createresult);
        }

    } else {
        QVariantMap createresult = helper.jsonToQt(replyCreateFolder->readAll());

        QString createresulterror = createresult["message"].toString();

        replyCreateFolder->deleteLater();
#ifdef QT_DEBUG
        qDebug() << createresulterror;
#endif

        emit createdFolderError(createresulterror);
    }
}


void OcFolders::folderCreatedUpdateDb(const QVariantMap &createresult)
{
    QSqlQuery query;
    foreach (QVariant folder, createresult["folders"].toList())
    {
        QMap<QString, QVariant> map = folder.value<QVariantMap>();
        query.prepare("INSERT INTO folders (id, name) VALUES (:id, :name);");
        query.bindValue(":id", map["id"].toInt());
        query.bindValue(":name", map["name"].toString());
        query.exec();
        emit createdFolderSuccess(map["name"].toString());
    }
}


void OcFolders::deleteFolder(const QString &id)
{
    if (network.isFlightMode())
    {
        emit deletedFolderError(tr("Device is in flight mode."));
    } else {
        // Create the API URL
        QString folder = "folders/";
        folder.append(id);

        replyDeleteFolder = network.deleteResource(helper.buildRequest(folder));

        connect(replyDeleteFolder,SIGNAL(finished()),this,SLOT(folderDeleted()));
    }
}

void OcFolders::folderDeleted()
{
//    connect(this,SIGNAL(deletedFolder(int)),this,SLOT(folderDeletedUpdateDb(int)), Qt::UniqueConnection);

    if (replyDeleteFolder->error() == QNetworkReply::NoError)
    {
        // extract folder id out of URL
        int id = replyDeleteFolder->url().toString().split("/").last().toInt();

        replyDeleteFolder->deleteLater();

        folderDeletedUpdateDb(id);

    } else {
        QVariantMap deleteresult;
        deleteresult = helper.jsonToQt(replyDeleteFolder->readAll());

        QString deleteresulterror = deleteresult["message"].toString();

        replyDeleteFolder->deleteLater();
#ifdef QT_DEBUG
        qDebug() << deleteresulterror;
#endif

        emit deletedFolderError(deleteresulterror);
    }
}


void OcFolders::folderDeletedUpdateDb(const int &id)
{
    QSqlQuery query;

    // query the ids of the affected feeds, used for deleting items
    QList<int> idListFeeds;
    query.exec(QString("SELECT id FROM feeds WHERE folderId = %1;").arg(id));
    while (query.next())
    {
        idListFeeds << query.value(0).toInt();
    }

    // delete the folder identified by id
    query.exec(QString("DELETE FROM folders WHERE id = %1;").arg(id));

    // delete the feeds that are part of the deleted folder
    query.exec(QString("DELETE FROM feeds WHERE folderId = %1;").arg(id));

    // delete the serverside deleted item ids in the database
    for (int i = 0; i < idListFeeds.size(); ++i) {
        feeds.feedDeletedCleanItems(idListFeeds.at(i));
    }

#ifdef QT_DEBUG
    qDebug() << "Delted Folder ID:" << id;
#endif
    emit deletedFolderSuccess();
}



void OcFolders::renameFolder(const QString &id, const QString &name)
{
#ifdef QT_DEBUG
    qDebug() << "New folder name: " << name;
#endif

    if (network.isFlightMode())
    {
        emit renamedFolderError(tr("Device is in flight mode."));
    } else {
        // Create the API URL
        QString folder = "folders/";
        folder.append(id);

        // Create the JSON string
        QByteArray parameters("{\"name\": \"");
        parameters.append(name);
        parameters.append("\"}");

        newFolderName = name;

        replyRenameFolder = network.put(helper.buildRequest(folder, parameters.length()), parameters);

        connect(replyRenameFolder, SIGNAL(finished()), this, SLOT(folderRenamed()));
    }
}

void OcFolders::folderRenamed()
{

    if (replyRenameFolder->error() == QNetworkReply::NoError)
    {
        // extract folder id out of URL
        int id = replyRenameFolder->url().toString().split("/").last().toInt();

        replyRenameFolder->deleteLater();

        folderRenamedUpdateDb(id, newFolderName);

    } else {
        QVariantMap renameresult = helper.jsonToQt(replyRenameFolder->readAll());

        QString renameresulterror = renameresult["message"].toString();

        replyRenameFolder->deleteLater();

#ifdef QT_DEBUG
        qDebug() << renameresulterror;
        qDebug() << replyRenameFolder->error();
        qDebug() << replyRenameFolder->errorString();
#endif

        emit renamedFolderError(renameresulterror);
    }
}


void OcFolders::folderRenamedUpdateDb(const int &id, const QString &name)
{
    QSqlQuery query;

    query.prepare("UPDATE folders SET name = :name WHERE id = :id;");
    query.bindValue(":name", name);
    query.bindValue(":id", id);
    query.exec();

    emit renamedFolderSuccess(name);
}


void OcFolders::markFolderRead(const QString &id)
{
    if (network.isFlightMode())
    {
        emit markedReadFolderError(tr("Device is in flight mode."));
    } else {
        // Create the API URL
        QString folder = "folders/";
        folder.append(id);
        folder.append("/read");

        // get newest item id
        QString newestItemId;
        QSqlQuery query;
        query.exec(QString("SELECT MAX(id) FROM items WHERE feedId IN (SELECT id FROM feeds WHERE folderId = %1);").arg(id.toInt()));
        if (query.next())
            newestItemId = query.value(0).toString();

    #ifdef QT_DEBUG
        qDebug() << "Newest ID: " << newestItemId;
    #endif

        // Create the JSON string
        QByteArray parameters("{\"newestItemId\": ");
        parameters.append(newestItemId);
        parameters.append("}");

        replyMarkFolderRead = network.put(helper.buildRequest(folder, parameters.length()), parameters);

        connect(replyMarkFolderRead, SIGNAL(finished()), this, SLOT(folderMarkedRead()));
    }
}


void OcFolders::folderMarkedRead()
{
    if (replyMarkFolderRead->error() == QNetworkReply::NoError)
    {
        // extract folder id out of URL
        QString url = replyMarkFolderRead->url().toString();
        QStringList urlParts = url.split("/");
        urlParts.removeLast();
        int id = urlParts.last().toInt();

#ifdef QT_DEBUG
        qDebug() << "Folder ID: " << id;
#endif

        replyMarkFolderRead->deleteLater();

        folderMarkedReadUpdateDb(id);

    } else {
        QVariantMap markreadresult = helper.jsonToQt(replyMarkFolderRead->readAll());

        QString markreaderror = markreadresult["message"].toString();

        replyMarkFolderRead->deleteLater();
#ifdef QT_DEBUG
        qDebug() << markreaderror;
#endif

        emit markedReadFolderError(markreaderror);
    }
}

void OcFolders::folderMarkedReadUpdateDb(const int &id)
{
    QDateTime ts;
    QSqlQuery query;
    QList<int> feedIds;

    // get feed ids
    query.exec(QString("SELECT id FROM feeds WHERE folderId = %1;").arg(id));
    while(query.next())
    {
        feedIds << query.value(0).toInt();
    }
#ifdef QT_DEBUG
    qDebug() << "Feed IDs: " << feedIds;
#endif

    QSqlDatabase::database().transaction();
    for (int i = 0; i < feedIds.size(); ++i) {
        query.exec(QString("UPDATE items SET unread = %3, lastModified = %2 WHERE unread = %4 AND feedId = %1").arg(feedIds.at(i)).arg(ts.currentDateTimeUtc().toTime_t()).arg(SQL_FALSE).arg(SQL_TRUE));
    }
    QSqlDatabase::database().commit();

    emit markedReadFolderSuccess();

}


QVariantMap OcFolders::getFolders()
{
    QSqlQuery query;
    QVariantMap folders;
    query.exec("SELECT id, name FROM folders;");
    while (query.next())
        folders[query.value(0).toString()] = query.value(1).toString();

    return folders;
}
