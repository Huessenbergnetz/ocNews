#ifndef OCCOMBINEDMODELFILTER_H
#define OCCOMBINEDMODELFILTER_H

#include <QSortFilterProxyModel>
#include <QList>
#include <QPair>
#include "occombinedmodelnew.h"

class OcCombinedModelFilter : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(bool hideRead READ hideRead WRITE setHideRead NOTIFY hideReadChanged)
    Q_PROPERTY(QString orderBy READ orderBy WRITE setOrderBy NOTIFY orderByChanged)
public:
    explicit OcCombinedModelFilter(QObject *parent = 0);

    bool hideRead() const;
    QString orderBy() const;

signals:
    void hideReadChanged(const bool &nHideRead);
    void orderByChanged(const QString &nOrderBy);

public slots:
    void setHideRead(const bool &nHideRead);
    void setOrderBy(const QString &nOrderBy);

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;

private:
    bool m_hideRead;
    QString m_orderBy;
    QList<QPair<int, Qt::SortOrder> > m_orders;

    void setSorting();

};

#endif // OCCOMBINEDMODELFILTER_H
