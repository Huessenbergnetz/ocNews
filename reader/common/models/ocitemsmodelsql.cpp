#include <QDebug>
#include <QDateTime>
#include "ocitemsmodelsql.h"


const char* OcItemsModelSql::COLUMN_NAMES[] = {
    "title",
    "itemId",
    "pubDate",
    "pubDateInt",
    "enclosureLink",
    "enclosureMime",
    "unread",
    "starred",
    "url",
    "guidHash",
    NULL
};

OcItemsModelSql::OcItemsModelSql(QObject *parent) :
    QSqlQueryModel(parent)
{

#if defined(MEEGO_EDITION_HARMATTAN)
    int idx = 0;
        QHash<int, QByteArray> roles;
        while(COLUMN_NAMES[idx]) {
            roles[Qt::UserRole + idx + 1] = COLUMN_NAMES[idx];
            idx++;
        }
        setRoleNames(roles);
#endif

}

#if !defined(MEEGO_EDITION_HARMATTAN)
QHash<int,QByteArray> OcItemsModelSql::roleNames() const
{
    int idx = 0;
    QHash<int, QByteArray> roles;
    while (COLUMN_NAMES[idx]) {
        roles[Qt::UserRole + idx + 1] = COLUMN_NAMES[idx];
        idx++;
    }
    return roles;
}
#endif


QVariant OcItemsModelSql::data(const QModelIndex &index, int role) const
{
    QVariant value = QSqlQueryModel::data(index, role);
        if(role < Qt::UserRole)
        {
            value = QSqlQueryModel::data(index, role);
        } else {
            int columnIdx = role - Qt::UserRole - 1;
            QModelIndex modelIndex = this->index(index.row(), columnIdx);
            value = QSqlQueryModel::data(modelIndex, Qt::DisplayRole);
            if (columnIdx == 2) {
                QString date = (QDateTime::currentDateTimeUtc().date() == QDateTime::fromTime_t(value.toUInt()).date()) ?
                                tr("Today") :
                                QDateTime::fromTime_t(value.toUInt()).toLocalTime().toString(tr("d. MMMM"));
                QString time = QDateTime::fromTime_t(value.toUInt()).toLocalTime().toString(tr("hh:mm"));

                value = date + " | " + time;
            }
        }
        return value;
}


void OcItemsModelSql::refresh(const QString &feedId, int handleRead, bool sortAsc, const QString &search)
{
#ifdef QT_DEBUG
    qDebug() << "Refresh Items Model";
#endif

    QString queryString;

//    if (search == "")
//    {
//        this->setQuery(QString("SELECT it.title, "
//                                      "it.id AS itemId, "
//                                      "it.pubDate, "
//                                      "it.pubDate AS pubDateInt, "
//                                      "it.enclosureLink, "
//                                      "it.enclosureMime, "
//                                      "it.unread, "
//                                      "it.starred, "
//                                      "it.url, "
//                                      "it.guidHash "
//                               "FROM items it WHERE feedId = %1 ORDER BY pubDate DESC;").arg(feedId.toInt()));
//    } else {
//        QString t_search = search;
//        t_search.prepend("%");
//        t_search.append("%");
//        this->setQuery(QString("SELECT it.title, "
//                                      "it.id AS itemId, "
//                                      "it.pubDate, "
//                                      "it.pubDate AS pubDateInt, "
//                                      "it.enclosureLink, "
//                                      "it.enclosureMime, "
//                                      "it.unread, "
//                                      "it.starred, "
//                                      "it.url, "
//                                      "it.guidHash "
//                               "FROM items it WHERE feedId = %1 AND title LIKE \"%2%\" ORDER BY pubDate DESC;").arg(feedId.toInt()).arg(t_search));
//    }

    // handleRead 0: show read, 1: hide read, 2: show after unread
    queryString = QString("SELECT it.title, "
                                 "it.id AS itemId, "
                                 "it.pubDate, "
                                 "it.pubDate AS pubDateInt, "
                                 "it.enclosureLink, "
                                 "it.enclosureMime, "
                                 "it.unread, "
                                 "it.starred, "
                                 "it.url, "
                                 "it.guidHash "
                          "FROM items it WHERE feedId = %1").arg(feedId.toInt());

#if defined(MEEGO_EDITION_HARMATTAN)
    if (handleRead == 1) queryString.append(" AND it.unread = \"true\"");
#else
    if (handleRead == 1) queryString.append(" AND it.unread = 1");
#endif

    if (search != "")
    {
        QString t_search = search;
        t_search.prepend("%");
        t_search.append("%");
        queryString.append(QString(" AND it.title LIKE \"%1\"").arg(t_search));
    }

    queryString.append(" ORDER BY");

    if (handleRead == 2)
        queryString.append(" unread DESC,");

    if (sortAsc)
    {
        queryString.append(" pubDate ASC;");
    } else {
        queryString.append(" pubDate DESC;");
    }

    this->setQuery(queryString);


    while(this->canFetchMore())
        this->fetchMore();
}
