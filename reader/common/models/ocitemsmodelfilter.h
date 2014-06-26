#ifndef OCITEMSMODELFILTER_H
#define OCITEMSMODELFILTER_H

#include <QSortFilterProxyModel>
#include "ocitemsmodelnew.h"

class OcItemsModelFilter : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(QString search READ search WRITE setSearch NOTIFY searchChanged)
    Q_PROPERTY(bool sortAsc READ sortAsc WRITE setSortAsc NOTIFY sortAscChanged)
    Q_PROPERTY(int handleRead READ handleRead WRITE setHandleRead NOTIFY handleReadChanged)

public:
    explicit OcItemsModelFilter(QObject *parent = 0);

    QString search() const;
    bool sortAsc() const;
    int handleRead() const;

signals:
    void searchChanged(const QString &nSearch);
    void sortAscChanged(const bool &nSortAsc);
    void handleReadChanged(const int &nHandleRead);

public slots:
    void setSearch(const QString &nSearch);
    void setSortAsc(const bool &nSortAsc);
    void setHandleRead(const int &nHandleRead);

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;

private:
    QString m_search;
    bool m_sortAsc;
    int m_handleRead;

    void setSortOrder();
};

#endif // OCITEMSMODELFILTER_H
