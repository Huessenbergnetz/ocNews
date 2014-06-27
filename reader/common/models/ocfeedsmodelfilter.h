#ifndef OCFEEDSMODELFILTER_H
#define OCFEEDSMODELFILTER_H

#include <QSortFilterProxyModel>
#include <QList>
#include <QPair>
#include "ocfeedsmodelnew.h"

class OcFeedsModelFilter : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(QString orderBy READ orderBy WRITE setOrderBy NOTIFY orderByChanged)
    Q_PROPERTY(bool hideRead READ hideRead WRITE setHideRead NOTIFY hideReadChanged)
public:
    explicit OcFeedsModelFilter(QObject *parent = 0);

    QString orderBy() const;
    bool hideRead() const;

signals:
    void orderByChanged(const QString &nOrderBy);
    void hideReadChanged(const bool &nHideRead);

public slots:
    void setOrderBy(const QString &nOrderBy);
    void setHideRead(const bool &nHideRead);

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;

private:
    QString m_orderBy;
    bool m_hideRead;
    QList<QPair<int, Qt::SortOrder> > m_orders;

    void setSorting();
};

#endif // OCFEEDSMODELFILTER_H
