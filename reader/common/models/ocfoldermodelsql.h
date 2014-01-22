#ifndef OCFOLDERMODELSQL_H
#define OCFOLDERMODELSQL_H

#include <QSqlQueryModel>
#include <QSqlQuery>
#include "../common/dbus/interfaces/ocdbusinterface.h"

class OcFolderModelSql : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit OcFolderModelSql(QObject *parent = 0);

    QVariant data(const QModelIndex &item, int role) const;

#if !defined(MEEGO_EDITION_HARMATTAN)
    QHash<int,QByteArray> roleNames() const;
#endif
    
signals:
    
public slots:
    void refresh();

private:
    const static char* COLUMN_NAMES[];
    OcDBusInterface dbus;
    
};

#endif // OCFOLDERMODELSQL_H
