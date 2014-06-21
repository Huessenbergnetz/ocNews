#ifndef OCITEMSMODELFILTER_H
#define OCITEMSMODELFILTER_H

#include <QSortFilterProxyModel>
#include "ocitemsmodelnew.h"

class OcItemsModelFilter : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(QString search READ search WRITE setSearch NOTIFY searchChanged)
    Q_PROPERTY(bool sortAsc READ sortAsc WRITE setSortAsc NOTIFY sortAscChanged)
public:
    explicit OcItemsModelFilter(QObject *parent = 0);

    QString search() const;
    bool sortAsc() const;

signals:
    void searchChanged(const QString &nSearch);
    void sortAscChanged(const bool &nSortAsc);

public slots:
    void setSearch(const QString &nSearch);
    void setSortAsc(const bool &nSortAsc);

private:
    QString m_search;
    bool m_sortAsc;

};

#endif // OCITEMSMODELFILTER_H
