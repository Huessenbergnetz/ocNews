#include "ocitemsmodelfilter.h"

OcItemsModelFilter::OcItemsModelFilter(QObject *parent) :
    QSortFilterProxyModel(parent)
{
    m_search = "";
    this->setFilterRole(OcItemsModelNew::TitleRole);
    this->setFilterCaseSensitivity(Qt::CaseInsensitive);
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
        this->setFilterFixedString(search());
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
        if (sortAsc()) {
            this->sort(0, Qt::AscendingOrder);
        } else {
            this->sort(0, Qt::DescendingOrder);
        }
        emit sortAscChanged(sortAsc());
    }
}
