#include <QDebug>
#include <QDateTime>
#include "ocspecialitemsmodelsql.h"
#include "../../../common/globals.h"

const char* OcSpecialItemsModelSql::COLUMN_NAMES[] = {
    "title",
    "itemId",
    "pubDate",
    "enclosureLink",
    "enclosureMime",
    "unread",
    "starred",
    "url",
    "guidHash",
    "feedId",
    "feedName",
    "excerpt",
    NULL
};

OcSpecialItemsModelSql::OcSpecialItemsModelSql(QObject *parent) :
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
QHash<int,QByteArray> OcSpecialItemsModelSql::roleNames() const
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


QVariant OcSpecialItemsModelSql::data(const QModelIndex &index, int role) const
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
                } else if (columnIdx == 11) {
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


void OcSpecialItemsModelSql::refresh(const QString &type, const QString &folderId, int handleRead, bool sortAsc, const QString &search)
{
#ifdef QT_DEBUG
    qDebug() << "Refresh Special Items Model";
#endif

    QString querystring("SELECT it.title, it.id AS itemId, it.pubDate, it.enclosureLink, it.enclosureMime, it.unread, it.starred, it.url, it.guidHash, it.feedId, ");
    querystring.append("(SELECT title FROM feeds where id = it.feedId) as feedName, ");

    if (config.getSetting("display/excerpts", false).toBool()) {
        querystring.append("it.body AS excerpt ");
    } else {
        querystring.append("'' AS excerpt ");
    }


    if (type == "folder")
    {
        querystring.append(QString("FROM items it WHERE feedId IN (SELECT id FROM feeds WHERE folderId = %1)").arg(folderId.toInt()));

        if (handleRead == 1) querystring.append(" AND it.unread = ").append(SQL_TRUE);

        if (search != "")
        {
            QString t_search = search;
            t_search.prepend("%");
            t_search.append("%");
            querystring.append(QString(" AND it.title LIKE \"%1\"").arg(t_search));
        }
    } else if (type == "starred") {

        querystring.append("FROM items it WHERE starred = ").append(SQL_TRUE);
        if (handleRead == 1) querystring.append(" AND it.unread = ").append(SQL_TRUE);

        if (!search.isEmpty())
        {
            QString t_search = search;
            t_search.prepend("%");
            t_search.append("%");
            querystring.append(QString(" AND it.title LIKE \"%1\" ").arg(t_search));
        }
    } else if (type == "all") {

        querystring.append("FROM items it");

        if (search != "" || handleRead == 1)
        {

            querystring.append(" WHERE");

            if (handleRead == 1) querystring.append(" it.unread = ").append(SQL_TRUE);

            if (search != "")
            {
                QString t_search = search;
                t_search.prepend("%");
                t_search.append("%");
                if (handleRead == 1) querystring.append(" AND");
                querystring.append(QString(" it.title LIKE \"%1\" ").arg(t_search));
            }
        }
    }

    querystring.append(" ORDER BY");

    if (handleRead == 2)
        querystring.append(" unread DESC,");

    if (sortAsc)
    {
        querystring.append(" pubDate ASC;");
    } else {
        querystring.append(" pubDate DESC;");
    }

    this->setQuery(querystring);
    while(this->canFetchMore())
        this->fetchMore();
}
