#ifndef OCDBMANAGER_H
#define OCDBMANAGER_H

#include <QObject>
#include <QtSql>
#include <QFile>
#include "../common/globals.h"

class OcDbManager : public QObject
{
    Q_OBJECT
public:
    explicit OcDbManager(QObject *parent = 0);

    bool openDB();
    bool createTables();
    bool closeDB();

signals:

public slots:

private:
    QSqlDatabase db;

};

#endif // OCDBMANAGER_H
