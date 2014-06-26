#include "occombinedmodelfilter.h"
#include <QDebug>

OcCombinedModelFilter::OcCombinedModelFilter(QObject *parent) :
    QSortFilterProxyModel(parent)
{
    m_hideRead = false;
    m_orderBy = "id";
    this->setDynamicSortFilter(true);
    this->setSortCaseSensitivity(Qt::CaseInsensitive);
    this->setSortLocaleAware(true);
    this->setSorting();
}



bool OcCombinedModelFilter::hideRead() const { return m_hideRead; }

void OcCombinedModelFilter::setHideRead(const bool &nHideRead)
{
    if (nHideRead != m_hideRead) {
        m_hideRead = nHideRead;
        this->invalidateFilter();
        emit hideReadChanged(hideRead());
    }
}


QString OcCombinedModelFilter::orderBy() const { return m_orderBy; }

void OcCombinedModelFilter::setOrderBy(const QString &nOrderBy)
{
    if (nOrderBy != m_orderBy) {
        m_orderBy = nOrderBy;
        setSorting();
        emit orderByChanged(orderBy());
    }
}


bool OcCombinedModelFilter::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    QModelIndex index = sourceModel()->index(source_row, 0, source_parent);

    if (hideRead()) {
        return (sourceModel()->data(index, OcCombinedModelNew::UnreadCountRole).toInt() > 0);
    } else {
        return true;
    }
}



bool OcCombinedModelFilter::lessThan(const QModelIndex &left, const QModelIndex &right) const
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


void OcCombinedModelFilter::setSorting()
{
    if (orderBy() == "id") {
        m_orders.clear();
        m_orders.append(qMakePair(OcCombinedModelNew::TypeRole, Qt::DescendingOrder));
        m_orders.append(qMakePair(OcCombinedModelNew::FolderIdRole, Qt::DescendingOrder));
        m_orders.append(qMakePair(OcCombinedModelNew::IdRole, Qt::DescendingOrder));
    } else if (orderBy() == "title") {
        m_orders.clear();
        m_orders.append(qMakePair(OcCombinedModelNew::TypeRole, Qt::DescendingOrder));
        m_orders.append(qMakePair(OcCombinedModelNew::FolderNameRole, Qt::AscendingOrder));
        m_orders.append(qMakePair(OcCombinedModelNew::TitleRole, Qt::AscendingOrder));
    } else {
        m_orders.clear();
        m_orders.append(qMakePair(OcCombinedModelNew::TypeRole, Qt::DescendingOrder));
        m_orders.append(qMakePair(OcCombinedModelNew::FolderNameRole, Qt::AscendingOrder));
        m_orders.append(qMakePair(OcCombinedModelNew::UnreadCountRole, Qt::DescendingOrder));
    }

    this->sort(0);
}
