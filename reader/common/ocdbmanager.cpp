#include <QDebug>
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
