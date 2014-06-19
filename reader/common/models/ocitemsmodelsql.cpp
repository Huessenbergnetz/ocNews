#include <QDebug>
#include <QDateTime>
#include "ocitemsmodelsql.h"
#include "../../../common/globals.h"


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
    "excerpt",
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
            } else if (columnIdx == 10) {
#if defined(MEEGO_EDITION_HARMATTAN)
                value = value.toString().remove(QRegExp("<[^>]*>"));
#else
                value = value.toString().remove(QRegularExpression("<[^>]*>"));
#endif
            }
#if !defined(MEEGO_EDITION_HARMATTAN)
            else if (columnIdx == 0) {
                value = value.toString().toHtmlEscaped();
            }
#endif
        }
        return value;
}


void OcItemsModelSql::refresh(const QString &feedId, int handleRead, bool sortAsc, const QString &search)
{
#ifdef QT_DEBUG
    qDebug() << "Refresh Items Model";
#endif

    QString queryString;

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
                                 "it.guidHash, ");

    if (config.getSetting("display/excerpts", false).toBool()) {
        queryString.append("it.body AS excerpt ");
    } else {
        queryString.append("'' AS excerpt ");
    }

    queryString.append(QString("FROM items it WHERE feedId = %1").arg(feedId.toInt()));


    if (handleRead == 1) queryString.append(" AND it.unread = ").append(SQL_TRUE);

    if (!search.isEmpty())
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
