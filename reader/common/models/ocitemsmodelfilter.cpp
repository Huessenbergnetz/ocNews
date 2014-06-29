#include "ocitemsmodelfilter.h"

OcItemsModelFilter::OcItemsModelFilter(QObject *parent) :
    QSortFilterProxyModel(parent)
{
    m_search = "";
    m_sortAsc = false;
    m_handleRead = 0;

//    this->setSortRole(OcItemsModelNew::PubDateIntRole);
//    this->sort(0, Qt::DescendingOrder);
    this->setDynamicSortFilter(true);
    this->setSortCaseSensitivity(Qt::CaseInsensitive);
    this->setSortLocaleAware(true);
    this->setSortOrder();
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
        if (handleRead() == 2) {
            setSortOrder();
        } else {
            this->invalidateFilter();
            this->invalidate();
        }
//        this->invalidateFilter();
//        this->invalidate();
//        setSortOrder();
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



bool OcItemsModelFilter::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    QPair<int, Qt::SortOrder> roleOrder;
    int role, c;
    QVariant l, r;
    QModelIndex idxLeft, idxRight;

    foreach(roleOrder, m_orders)
    {
        role = roleOrder.first;
        idxLeft = left.model()->index(left.row(), 0, left.parent());
        idxRight = right.model()->index(right.row(), 0, right.parent());

        if (roleOrder.second == Qt::AscendingOrder) {
            l = idxLeft.data(role);
            r = idxRight.data(role);
        } else {
            r = idxLeft.data(role);
            l = idxRight.data(role);
        }

        switch (l.userType())
        {
        case QVariant::Invalid:
            return(r.type() == QVariant::Invalid);
        case QVariant::Int:
            if (l.toInt() < r.toInt())
                return true;
            else if (l.toInt() > r.toInt())
                return false;
            break;
        case QVariant::Bool:
            if (l.toBool() < r.toBool())
                return true;
            else if (l.toBool() > r.toBool())
                return false;
            break;
        default:
            if (isSortLocaleAware())
            {
                c = l.toString().localeAwareCompare(r.toString());
                if (c<0)
                    return true;
                else if (c>0)
                    return false;
            } else {
                c = l.toString().compare(r.toString(), sortCaseSensitivity());
                if (c<0)
                    return true;
                else if (c>0)
                    return false;
            }
        }
    }

    return true;
}


void OcItemsModelFilter::setSortOrder()
{
//    if (sortAsc()) {
//        this->setSortRole(OcItemsModelNew::PubDateIntRole);
//        this->sort(0, Qt::AscendingOrder);
//    } else {
//        this->setSortRole(OcItemsModelNew::PubDateIntRole);
//        this->sort(0, Qt::DescendingOrder);
//    }

//    if (handleRead() == 2) {
//        this->setSortRole(OcItemsModelNew::UnreadRole);
//        this->sort(0, Qt::DescendingOrder);
//    }

    if (sortAsc() && handleRead() == 2) {
        m_orders.clear();
        m_orders.append(qMakePair(OcItemsModelNew::UnreadRole, Qt::DescendingOrder));
        m_orders.append(qMakePair(OcItemsModelNew::PubDateIntRole, Qt::AscendingOrder));
    } else if (sortAsc() && handleRead() != 2) {
        m_orders.clear();
        m_orders.append(qMakePair(OcItemsModelNew::PubDateIntRole, Qt::AscendingOrder));
    } else if (!sortAsc() && handleRead() == 2) {
        m_orders.clear();
        m_orders.append(qMakePair(OcItemsModelNew::UnreadRole, Qt::DescendingOrder));
        m_orders.append(qMakePair(OcItemsModelNew::PubDateIntRole, Qt::DescendingOrder));
    } else if (!sortAsc() && handleRead() != 2) {
        m_orders.clear();
        m_orders.append(qMakePair(OcItemsModelNew::PubDateIntRole, Qt::DescendingOrder));
    }

    this->sort(0);
    this->invalidate();
    this->invalidateFilter();
}
