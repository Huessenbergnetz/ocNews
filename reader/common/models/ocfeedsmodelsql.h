#ifndef OCFEEDSMODELSQL_H
#define OCFEEDSMODELSQL_H

#include <QSqlQueryModel>
#include "../dbus/interfaces/ocdbusinterface.h"

class OcFeedsModelSql : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit OcFeedsModelSql(QObject *parent = 0);

    QVariant data(const QModelIndex &item, int role) const;

#if !defined(MEEGO_EDITION_HARMATTAN)
    QHash<int,QByteArray> roleNames() const;
#endif
    
signals:
    
public slots:
    void refresh(const QString &folderId);

private:
    const static char* COLUMN_NAMES[];
    OcDBusInterface dbus;
    
};

#endif // OCFEEDSMODELSQL_H
