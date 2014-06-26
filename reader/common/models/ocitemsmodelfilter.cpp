#include "ocitemsmodelfilter.h"

OcItemsModelFilter::OcItemsModelFilter(QObject *parent) :
    QSortFilterProxyModel(parent)
{
    m_search = "";
    m_sortAsc = false;
    m_handleRead = 0;

    this->setSortRole(OcItemsModelNew::PubDateIntRole);
    this->sort(0, Qt::DescendingOrder);
}

QString OcItemsModelFilter::search() const
{
    return m_search;
}

void OcItemsModelFilter::setSearch(const QString &nSearch)
{
    if (nSearch != m_search) {
        m_search = nSearch;
        this->invalidateFilter();
        this->invalidate();
        emit searchChanged(search());
    }
}


bool OcItemsModelFilter::sortAsc() const
{
    return m_sortAsc;
}


void OcItemsModelFilter::setSortAsc(const bool &nSortAsc)
{
    if (nSortAsc != m_sortAsc) {
        m_sortAsc = nSortAsc;
        setSortOrder();
        emit sortAscChanged(sortAsc());
    }
}



int OcItemsModelFilter::handleRead() const
{
    return m_handleRead;
}


void OcItemsModelFilter::setHandleRead(const int &nHandleRead)
{
    if (nHandleRead != m_handleRead) {
        m_handleRead = nHandleRead;
        this->invalidateFilter();
        this->invalidate();
        setSortOrder();
        emit handleReadChanged(handleRead());
    }
}



bool OcItemsModelFilter::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    QModelIndex index = sourceModel()->index(source_row, 0, source_parent);

    if (handleRead() == 1) {
        return (sourceModel()->data(index, OcItemsModelNew::TitleRole).toString().contains(search(), Qt::CaseInsensitive) && sourceModel()->data(index, OcItemsModelNew::UnreadRole).toBool());
    } else {
        return sourceModel()->data(index, OcItemsModelNew::TitleRole).toString().contains(search(), Qt::CaseInsensitive);
    }
}


void OcItemsModelFilter::setSortOrder()
{
    if (sortAsc()) {
        this->setSortRole(OcItemsModelNew::PubDateIntRole);
        this->sort(0, Qt::AscendingOrder);
    } else {
        this->setSortRole(OcItemsModelNew::PubDateIntRole);
        this->sort(0, Qt::DescendingOrder);
    }

    if (handleRead() == 2) {
        this->setSortRole(OcItemsModelNew::UnreadRole);
        this->sort(0, Qt::DescendingOrder);
    }
}
