#include "QsLog.h"
#include "ocdbmanager.h"
#include "../../common/globals.h"

OcDbManager::OcDbManager(QObject *parent) :
    QObject(parent)
{
}

bool OcDbManager::openDB()
{
    db = QSqlDatabase::addDatabase("QSQLITE");

    QString path(QDir::homePath());
    path.append(BASE_PATH).append("/database.sqlite");
    path = QDir::toNativeSeparators(path);

    QLOG_DEBUG() << "Database file path: " << path;

    // check if database file exists before database will be opened
    QFile dbfile(path);

    while(!dbfile.exists()) {
        QLOG_WARN() << "Database file does not exist. Waiting for it's creation by the engine...";
        QEventLoop loop;
        QTimer::singleShot(1000, &loop, SLOT(quit()));
        loop.exec();
    }

    db.setDatabaseName(path);
    db.setConnectOptions("QSQLITE_OPEN_READONLY");

    bool dbOpen = db.open();

    if (!dbOpen) {
        QLOG_FATAL() << "Can not open sqlite database";
    } else {
        QLOG_INFO() << "Opened sqlite database";
    }

    return dbOpen;
}

bool OcDbManager::closeDB()
{
    QLOG_INFO() << "Closing database";
    db.close();
    return true;
}
