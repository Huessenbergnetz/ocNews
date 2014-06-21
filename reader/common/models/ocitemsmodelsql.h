#ifndef OCITEMSMODELSQL_H
#define OCITEMSMODELSQL_H

#include <QSqlQueryModel>
#include <QSqlQuery>
#include "../common/dbus/interfaces/ocdbusinterface.h"

class OcItemsModelSql : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit OcItemsModelSql(QObject *parent = 0);

    QVariant data(const QModelIndex &item, int role) const;

#if !defined(MEEGO_EDITION_HARMATTAN)
    QHash<int,QByteArray> roleNames() const;
#endif
    
signals:
    
public slots:
    void refresh(const QString &feedId, int handleRead, bool sortAsc, const QString &search = "");

private:
    const static char* COLUMN_NAMES[];
    OcDBusInterface config;
    
};

#endif // OCITEMSMODELSQL_H
