#include "QsLog.h"
#include "ocdbmanager.h"



/*!
 * \class OcConfiguration
 * \brief The OcDbManager class provides base functions for database access and creation.
 *
 * This class is a helper class that can open and create the SQLite database.
*/

OcDbManager::OcDbManager(QObject *parent) :
    QObject(parent)
{
}



/*!
 * \fn bool OcDbManager::openDB()
 * \brief Opens the database
 *
 * This function opens the database file and returns true if it was successful
 *
 * \return true if successfull
 */

bool OcDbManager::openDB()
{
    db = QSqlDatabase::addDatabase("QSQLITE");

    QString path(QDir::homePath());
    path.append(BASE_PATH).append("/database.sqlite");
    db.setDatabaseName(path);

    QLOG_DEBUG() << "Database path: " << db.databaseName();

    bool dbOpened = db.open();

    if (dbOpened) {
        QLOG_INFO() << "Opening Sqlite database: success";
    } else {
        QLOG_FATAL() << "Opening Sqlite database: failed";
        QLOG_ERROR() << "Error opening databse: " << db.lastError().text();
    }

    return dbOpened;
}




/*!
 * \fn bool OcDbManager::createTables()
 * \brief Creates tables, triggers and indexes
 *
 * This function creates the necessary tables, triggers and indexes and
 * returns true if it was successful.
 *
 * \return true if successful
 */

bool OcDbManager::createTables()
{
    bool success = false;
    if (db.isOpen())
    {
        QSqlQuery build;
        QSqlDatabase::database().transaction();
        build.exec(QString("CREATE TABLE IF NOT EXISTS folders "
                           "(id INTEGER PRIMARY KEY NOT NULL, "
                           "parent_id INTEGER, "
                           "name VARCHAR, "
                           "localUnreadCount INT DEFAULT 0);"));

        QLOG_INFO() << "Creating table folders if not exists";


        build.exec(QString("CREATE TABLE IF NOT EXISTS feeds "
                           "(id INTEGER PRIMARY KEY NOT NULL, "
                           "url TEXT NOT NULL, "
                           "title TEXT NOT NULL, "
                           "faviconLink TEXT, "
                           "added INTEGER NOT NULL, "
                           "folderId INTEGER NOT NULL, "
                           "unreadCount INTEGER, "
                           "link TEXT, "
                           "iconSource TEXT, "
                           "iconWidth INT, "
                           "iconHeight INT, "
                           "localUnreadCount INT DEFAULT 0);"));

        QLOG_INFO() << "Creating table feeds if not exists";


        build.exec(QString("CREATE TABLE IF NOT EXISTS items "
                           "(id INTEGER PRIMARY KEY NOT NULL, "
                           "guid TEXT NOT NULL, "
                           "guidHash VARCHAR, "
                           "url TEXT, "
                           "title TEXT, "
                           "author TEXT, "
                           "pubDate INTEGER, "
                           "body TEXT, "
                           "enclosureMime TEXT, "
                           "enclosureLink TEXT, "
                           "feedId INTEGER NOT NULL, "
                           "unread BOOLEAN, "
                           "starred BOOLEAN, "
                           "lastModified INTEGER);"));

        QLOG_INFO() << "Creating table items if not exists";


        build.exec(QString("CREATE TABLE IF NOT EXISTS queue "
                           "(id INTEGER PRIMARY KEY AUTOINCREMENT, "
                           "action INTEGER NOT NULL, "
                           "itemId INTEGER, "
                           "guidHash VARCHAR, "
                           "entryDate INTEGER NOT NULL);"));

        QLOG_INFO() << "Creating table queue if not exists";


        build.exec(QString("CREATE TABLE IF NOT EXISTS images "
                           "(id INTEGER PRIMARY KEY AUTOINCREMENT, "
                           "parentId INTEGER NOT NULL, "
                           "path TEXT, "
                           "width INT, "
                           "height INT);"));


        QLOG_INFO() << "Creating database indexes if not exists";

        build.exec(QString("CREATE INDEX IF NOT EXISTS feeds_folder_id_index ON feeds (folderId)"));

        build.exec(QString("CREATE INDEX IF NOT EXISTS folders_parent_id_index ON folders (parent_id)"));

        build.exec(QString("CREATE INDEX IF NOT EXISTS items_item_guid ON items (guidHash, feedId)"));

        build.exec(QString("CREATE INDEX IF NOT EXISTS items_feed_id_index ON items (feedId)"));


        QLOG_INFO() << "Creating database triggers if not exists";

        build.exec(QString("CREATE TRIGGER IF NOT EXISTS feeds_localUnread_update_item AFTER UPDATE OF unread ON items "
                           "BEGIN "
                           "UPDATE feeds SET localUnreadCount = (SELECT COUNT(id) FROM items WHERE unread = %1 AND feedId = old.feedId) WHERE id = old.feedId; "
                           "END;").arg(SQL_TRUE));

        build.exec(QString("CREATE TRIGGER IF NOT EXISTS feeds_localUnread_delete_item AFTER DELETE ON items "
                           "BEGIN "
                           "UPDATE feeds SET localUnreadCount = (SELECT COUNT(id) FROM items WHERE unread = %1 AND feedId = old.feedId) WHERE id = old.feedId; "
                           "END;").arg(SQL_TRUE));

        build.exec(QString("CREATE TRIGGER IF NOT EXISTS feeds_localUnread_add_item AFTER INSERT ON items "
                           "BEGIN "
                           "UPDATE feeds SET localUnreadCount = (SELECT COUNT(id) FROM items WHERE unread = %1 AND feedId = new.feedId) WHERE id = new.feedId; "
                           "END;").arg(SQL_TRUE));

        build.exec(QString("CREATE TRIGGER IF NOT EXISTS folders_localUnread_update_feed AFTER UPDATE OF localUnreadCount ON feeds "
                           "BEGIN "
                           "UPDATE folders SET localUnreadCount = (SELECT SUM(localUnreadCount) FROM feeds WHERE folderId = old.folderId) WHERE id = old.folderId; "
                           "END;"));

        build.exec(QString("CREATE TRIGGER IF NOT EXISTS folders_localUnread_add_feed AFTER INSERT ON feeds "
                           "BEGIN "
                           "UPDATE folders SET localUnreadCount = (SELECT SUM(localUnreadCount) FROM feeds WHERE folderId = new.folderId) WHERE id = new.folderId; "
                           "END;"));

        build.exec(QString("CREATE TRIGGER IF NOT EXISTS folders_localUnread_delete_feed AFTER DELETE ON feeds "
                           "BEGIN "
                           "UPDATE folders SET localUnreadCount = (SELECT SUM(localUnreadCount) FROM feeds WHERE folderId = old.folderId) WHERE id = old.folderId; "
                           "END;"));

        build.exec(QString("CREATE TRIGGER IF NOT EXISTS folders_localUnread_move_feed AFTER UPDATE OF folderId ON feeds "
                           "BEGIN "
                           "UPDATE folders SET localUnreadCount = (SELECT SUM(localUnreadCount) FROM feeds WHERE folderId = new.folderId) WHERE id = new.folderId; "
                           "UPDATE folders SET localUnreadCount = (SELECT SUM(localUnreadCount) FROM feeds WHERE folderId = old.folderId) WHERE id = old.folderId; "
                           "END;"));


        success = QSqlDatabase::database().commit();

        if (!success) {
            QLOG_ERROR() << "Database Error: " << db.lastError().text();
        };

    }
    return success;
}




/*!
 * \fn bool OcDbManager::closeDB()
 * \brief Closes the database
 *
 * This function closes the database connection.
 *
 * \return true if successful
 */

bool OcDbManager::closeDB()
{
    QLOG_INFO() << "Closing database";
    db.close();
    return true;
}
