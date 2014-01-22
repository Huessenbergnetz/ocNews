#ifndef OCSPECIALITEMSMODELSQL_H
#define OCSPECIALITEMSMODELSQL_H

#include <QSqlQueryModel>

class OcSpecialItemsModelSql : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit OcSpecialItemsModelSql(QObject *parent = 0);

    QVariant data(const QModelIndex &item, int role) const;

#if !defined(MEEGO_EDITION_HARMATTAN)
    QHash<int,QByteArray> roleNames() const;
#endif
    
signals:
    
public slots:
    void refresh(const QString &type = "all", const QString &folderId = "0", int handleRead = 0, bool sortAsc = false, const QString &search = "");

private:
    const static char* COLUMN_NAMES[];
    
};

#endif // OCSPECIALITEMSMODELSQL_H
