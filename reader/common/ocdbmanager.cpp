#include <QDebug>
#include "ocdbmanager.h"

OcDbManager::OcDbManager(QObject *parent) :
    QObject(parent)
{
}

bool OcDbManager::openDB()
{
    db = QSqlDatabase::addDatabase("QSQLITE");

    QString path(QDir::homePath());
#if defined(MEEGO_EDITION_HARMATTAN)
    path.append(QDir::separator()).append(".local").append(QDir::separator()).append("share").append(QDir::separator()).append("data").append(QDir::separator()).append("ocNews").append(QDir::separator()).append("database.sqlite");
#else
    path.append(QDir::separator()).append(".local").append(QDir::separator()).append("share").append(QDir::separator()).append("data").append(QDir::separator()).append("harbour-ocnews").append(QDir::separator()).append("database.sqlite");
#endif
    path = QDir::toNativeSeparators(path);

    // check if database file exists before database will be opened
    QFile dbfile(path);

    while(!dbfile.exists()) {
        QEventLoop loop;
        QTimer::singleShot(1000, &loop, SLOT(quit()));
        loop.exec();
    }

    db.setDatabaseName(path);
    db.setConnectOptions("QSQLITE_OPEN_READONLY");

    return db.open();
}

bool OcDbManager::closeDB()
{
    db.close();
    return true;
}
