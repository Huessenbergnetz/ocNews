#include <QtNetwork>
#include "ocfolders.h"
#include "QsLog.h"

OcFolders::OcFolders(QObject *parent) :
    QObject(parent)
{
    newFolderName = "";
}

void OcFolders::requestFolders()
{
    if (network.isFlightMode())
    {
        QLOG_INFO() << "Can not request folders: Device is in flight mode.";
        emit requestedFoldersError(tr("Device is in flight mode."));
    } else {
        QLOG_INFO() << "Starting request to fetch folders.";

        replyRequestFolders = network.get(helper.buildRequest("folders"));

        connect(replyRequestFolders,SIGNAL(finished()),this,SLOT(foldersRequested()));
    }
}

void OcFolders::foldersRequested()
{

    if (replyRequestFolders->error() == QNetworkReply::NoError)
    {
        QVariantMap foldersresult = helper.jsonToQt(replyRequestFolders->readAll());



        if (foldersresult.isEmpty())
        {
            QLOG_ERROR() << "Failed to request foders: Server reply was empty.";
            notify.showNotification(tr("Server reply was empty."), tr("Failed to request folders"), OcNotifications::Error);
            emit requestedFoldersError(tr("Server reply was empty."));

        } else {

            QLOG_INFO() << "Successfully requested folders from the server.";

            foldersRequestedUpdateDb(foldersresult);

        }

    } else {

        QLOG_ERROR() << "Failed to request folders: " << replyRequestFolders->errorString();

        notify.showNotification(replyRequestFolders->errorString(), tr("Failed to request folders"), OcNotifications::Error);
        emit requestedFoldersError(replyRequestFolders->errorString());
    }

    replyRequestFolders->deleteLater();

}

void OcFolders::foldersRequestedUpdateDb(const QVariantMap &foldersresult)
{
    QLOG_INFO() << "Updating database after requestinf folders.";

    QSqlQuery query;
    QStringList newFolders = QStringList();

    QList<int> newFolderIds = QList<int>();
    QList<int> updatedFolderIds = QList<int>();

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

                updatedFolderIds << map["id"].toInt();

                QLOG_DEBUG() << "Updated folder: " << map["name"].toString();
            }

        } else { // if folder is not in database, create it there
            query.prepare("INSERT INTO folders (id, name) VALUES (:id, :name);");
            query.bindValue(":id", map["id"].toInt());
            query.bindValue(":name", map["name"].toString());
            query.exec();

            QLOG_DEBUG() << "Created folder: " << map["name"].toString();

            newFolders << map["name"].toString();
            newFolderIds << map["id"].toInt();
        }


    }


    // now check if folders were deleted on server
    QList<int> idList;
    QList<int> idListDeleted;
    QStringList deleteFolderNames = QStringList();

    // put all the ids into a list
    foreach (QVariant folder, foldersresult["folders"].toList())
    {
        QMap<QString, QVariant> map = folder.value<QVariantMap>();

        idList << map["id"].toInt();
    }

    // compare the ids and put the deleted ids into a list
    query.exec(QString("SELECT id, name FROM folders;"));
    while (query.next())
    {
        if (!idList.contains(query.value(0).toInt())) {
            idListDeleted << query.value(0).toInt();
            deleteFolderNames << query.value(1).toString();
        }
    }

    // delete the serverside deleted ids in the database
    if (!idListDeleted.isEmpty())
    {
        QSqlDatabase::database().transaction();
        for (int i = 0; i < idListDeleted.size(); ++i) {
            query.exec(QString("DELETE FROM folders WHERE id = %1").arg(idListDeleted.at(i)));

            QLOG_DEBUG() << "Deleted Folder ID: " << idListDeleted.at(i);

        }
        QSqlDatabase::database().commit();
    }

    if (query.lastError().type() != QSqlError::NoError) {
        QLOG_ERROR() << "Database error while deleting folder(s): " << query.lastError().text();
    }

    if ((!deleteFolderNames.isEmpty() || !newFolders.isEmpty()) && config.value("notifications/feedsFolders", false).toBool())
    {
        QString summary = "";
        QString body = "";

        if (!newFolders.isEmpty()) {
            summary = tr("%n folder(s) added", "", newFolders.count());
            body = tr("Added:").append(" ");
            body.append(newFolders.join(", "));
        }

        if (summary != "") {
            summary.append(", ");
            body.append("; ");
        }

        if (!deleteFolderNames.isEmpty())
        {
            summary.append(tr("%n folder(s) removed", "", deleteFolderNames.count()));
            body.append(tr("Removed:")).append(" ");
            body.append(deleteFolderNames.join(", "));
        }

        notify.showNotification(body, summary);
    }

    emit requestedFoldersSuccess(updatedFolderIds, newFolderIds, idListDeleted);
}



void OcFolders::createFolder(const QString &name)
{
    if (network.isFlightMode())
    {
        QLOG_INFO() << "Can not create folder: Device is in flight mode.";
        emit createdFolderError(tr("Device is in flight mode."));
    } else {

        QLOG_INFO() << "Start request to create new folder: " << name;
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
            QLOG_ERROR() << "Failed to create new folder: Server reply was empty.";
            notify.showNotification(tr("Server reply was empty."), tr("Failed to create folder"), OcNotifications::Error);
            emit createdFolderError(tr("Server reply was empty."));
        } else {

            QLOG_INFO() << "Successfully requested the creation of new folder.";
            folderCreatedUpdateDb(createresult);
        }

    } else {
        QVariantMap createresult = helper.jsonToQt(replyCreateFolder->readAll());

        QString createresulterror = createresult["message"].toString();

        if (createresulterror.isEmpty())
            createresulterror = replyCreateFolder->errorString();

        replyCreateFolder->deleteLater();

        QLOG_ERROR() << "Failed to request the creation of a new folder: " << createresulterror;

        notify.showNotification(createresulterror, tr("Failed to create folder"), OcNotifications::Error);

        emit createdFolderError(createresulterror);
    }
}


void OcFolders::folderCreatedUpdateDb(const QVariantMap &createresult)
{
    QLOG_INFO() << "Update database for newly created folder.";
    QLOG_TRACE() << createresult;
    QSqlQuery query;
    foreach (QVariant folder, createresult["folders"].toList())
    {
        QMap<QString, QVariant> map = folder.value<QVariantMap>();
        query.prepare("INSERT INTO folders (id, name) VALUES (:id, :name);");
        query.bindValue(":id", map["id"].toInt());
        query.bindValue(":name", map["name"].toString());
        if (!query.exec()) {
            QLOG_ERROR() << "Database error while adding newly created folder: " << query.lastError().text();
        }
        emit createdFolderSuccess(map["name"].toString(), map["id"].toInt());
//        notify.showNotification(tr("Successfully created folder \"%1\"").arg(map["name"].toString()), tr("Created folder"), OcNotifications::Success);
    }
}


void OcFolders::deleteFolder(const QString &id)
{
    if (network.isFlightMode())
    {
        QLOG_INFO() << "Can not request to delete folder ID " << id << ": Device is in flight mode.";
        emit deletedFolderError(tr("Device is in flight mode."));
    } else {
        // Create the API URL
        QLOG_INFO() << "Start to request the deletion of folder ID: " << id;

        QString folder = "folders/";
        folder.append(id);

        replyDeleteFolder = network.deleteResource(helper.buildRequest(folder));

        connect(replyDeleteFolder,SIGNAL(finished()),this,SLOT(folderDeleted()));
    }
}

void OcFolders::folderDeleted()
{

    if (replyDeleteFolder->error() == QNetworkReply::NoError)
    {
        // extract folder id out of URL
        int id = replyDeleteFolder->url().toString().split("/").last().toInt();

        QLOG_INFO() << "Successfully requested the delition of folder ID: " << id;

        replyDeleteFolder->deleteLater();

        folderDeletedUpdateDb(id);

    } else {
        QVariantMap deleteresult;
        deleteresult = helper.jsonToQt(replyDeleteFolder->readAll());

        QString deleteresulterror = deleteresult["message"].toString();

        if (deleteresulterror.isEmpty())
            deleteresulterror = replyDeleteFolder->errorString();

        replyDeleteFolder->deleteLater();

        QLOG_ERROR() << "Failed to request the deletion of a folder: " << deleteresulterror;

        notify.showNotification(deleteresulterror, tr("Failed to delete folder"), OcNotifications::Error);

        emit deletedFolderError(deleteresulterror);
    }
}


void OcFolders::folderDeletedUpdateDb(const int &id)
{
    QLOG_INFO() << "Updating the databse after deleting folder ID: " << id;

    QSqlQuery query;

    // query the ids of the affected feeds, used for deleting items
    QList<int> idListFeeds;
    query.exec(QString("SELECT id FROM feeds WHERE folderId = %1;").arg(id));
    while (query.next())
    {
        idListFeeds << query.value(0).toInt();
    }

    // delete the folder identified by id
    if (!query.exec(QString("DELETE FROM folders WHERE id = %1;").arg(id))) {
        QLOG_ERROR() << "Database error while deleting folder ID " << id << ": " << query.lastError().text();
    }

    // delete the feeds that are part of the deleted folder
    if (!query.exec(QString("DELETE FROM feeds WHERE folderId = %1;").arg(id))) {
        QLOG_ERROR() << "Database error while deleting feeds of folder ID: " << id << ": " << query.lastError().text();
    }

    QLOG_INFO() << "Deleted Folder ID: " << id;

    emit deletedFolderCleanItems(idListFeeds);
    emit deletedFolderSuccess(id);
}



void OcFolders::renameFolder(const QString &id, const QString &name)
{
    if (network.isFlightMode())
    {
        QLOG_INFO() << "Can not request to rename folder ID " << id << ": Device is in flight mode.";
        emit renamedFolderError(tr("Device is in flight mode."));
    } else {
        QLOG_INFO() << "Start request to rename folder ID " << id << " to " << name;

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

        QLOG_INFO() << "Successfully requested to delete folder ID " << id;

        folderRenamedUpdateDb(id, newFolderName);

    } else {
        QVariantMap renameresult = helper.jsonToQt(replyRenameFolder->readAll());

        QString renameresulterror = renameresult["message"].toString();

        if (renameresulterror.isEmpty())
            renameresulterror = replyRenameFolder->errorString();

        replyRenameFolder->deleteLater();

        QLOG_ERROR() << "Failed to request the deletion of a folder: " << renameresulterror;

        notify.showNotification(renameresulterror, tr("Failed to rename folder"), OcNotifications::Error);

        emit renamedFolderError(renameresulterror);
    }
}


void OcFolders::folderRenamedUpdateDb(const int &id, const QString &name)
{
    QLOG_INFO() << "Updating databse after renaming folder ID " << id << " to " << name;
    QSqlQuery query;

    query.prepare("UPDATE folders SET name = :name WHERE id = :id;");
    query.bindValue(":name", name);
    query.bindValue(":id", id);
    if (!query.exec()) {
        QLOG_ERROR() << "Database error while renaming a folder: " << query.lastError().text();
    }

    emit renamedFolderSuccess(name, id);
}


void OcFolders::markFolderRead(const QString &id)
{
    if (network.isFlightMode())
    {
        QLOG_INFO() << "Can not mark folder ID " << id << " as read: Device is in flight mode.";
        emit markedReadFolderError(tr("Device is in flight mode."));
    } else {
        QLOG_INFO() << "Start request to mark folder ID " << id << " as read.";

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

        QLOG_DEBUG() << "Mark folder ID " << id << " as read. Newest ID: " << newestItemId;

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

        QLOG_INFO() << "Successfully requested the marking of the folder ID " << id << " as read.";

        replyMarkFolderRead->deleteLater();

        folderMarkedReadUpdateDb(id);

    } else {
        QVariantMap markreadresult = helper.jsonToQt(replyMarkFolderRead->readAll());

        QString markreaderror = markreadresult["message"].toString();

        if (markreaderror.isEmpty())
            markreaderror = replyMarkFolderRead->errorString();

        replyMarkFolderRead->deleteLater();

        QLOG_ERROR() << "Failed to mark folder as read: " << markreaderror;

        notify.showNotification(markreaderror, tr("Failed to mark folder as read"), OcNotifications::Error);

        emit markedReadFolderError(markreaderror);
    }
}

void OcFolders::folderMarkedReadUpdateDb(const int &id)
{
    QLOG_INFO() << "Updating database after marking folder ID " << id << " as read.";

    QDateTime ts;
    QSqlQuery query;
    QList<int> feedIds;

    // get feed ids
    if (!query.exec(QString("SELECT id FROM feeds WHERE folderId = %1;").arg(id))) {
        QLOG_ERROR() << "Database error while selecting feed IDs for marking folder ID " << id << " as read: " << query.lastError().text();
    }

    while(query.next())
    {
        feedIds << query.value(0).toInt();
    }

    QLOG_DEBUG() << "Feed IDs belonging to the folder ID " << id << " that should be marked as read: " << feedIds;

    QSqlDatabase::database().transaction();
    for (int i = 0; i < feedIds.size(); ++i) {
        query.exec(QString("UPDATE items SET unread = %3, lastModified = %2 WHERE unread = %4 AND feedId = %1").arg(feedIds.at(i)).arg(ts.currentDateTimeUtc().toTime_t()).arg(SQL_FALSE).arg(SQL_TRUE));
    }
    QSqlDatabase::database().commit();

    if (query.lastError().type() != QSqlError::NoError) {
        QLOG_ERROR() << "Database error while marking items belonging to folder ID " << id << " as read: " << query.lastError().text();
    }

    emit markedReadFolderSuccess(id);

}


QVariantMap OcFolders::getFolders()
{
    QSqlQuery query;
    QVariantMap folders;

    if (!query.exec("SELECT id, name FROM folders;")) {
        QLOG_ERROR() << "Database error while selecting folde IDs and names: " << query.lastError().text();
    }

    while (query.next())
        folders[query.value(0).toString()] = query.value(1).toString();

    return folders;
}
