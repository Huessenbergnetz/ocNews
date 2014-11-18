#include "QsLog.h"
#include "ocfoldermodelsql.h"
#include "../../../common/globals.h"

const char* OcFolderModelSql::COLUMN_NAMES[] = {
    "title",
    "id",
    "feedCount",
    "unreadCount",
    "type",
    "iconSource",
    "iconWidth",
    "iconHeight",
    NULL
};

OcFolderModelSql::OcFolderModelSql(QObject *parent) :
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

    refresh();

}

#if !defined(MEEGO_EDITION_HARMATTAN)
QHash<int,QByteArray> OcFolderModelSql::roleNames() const
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

QVariant OcFolderModelSql::data(const QModelIndex &index, int role) const
{
    QVariant value = QSqlQueryModel::data(index, role);
    if(role < Qt::UserRole)
    {
        value = QSqlQueryModel::data(index, role);
    } else {
        int columnIdx = role - Qt::UserRole - 1;
        QModelIndex modelIndex = this->index(index.row(), columnIdx);
        value = QSqlQueryModel::data(modelIndex, Qt::DisplayRole);
        if (columnIdx == 5 && value != "") {
            QString iconSrcPath = QDir::homePath() + BASE_PATH + "/favicons/" + value.toString();
            value = iconSrcPath;
        }
    }
    return value;
}

void OcFolderModelSql::refresh()
{
#ifdef QT_DEBUG
    qDebug() << "Refresh Folder Model";
#endif

    QSqlQuery query;

    bool hideReadFeeds = dbus.getSetting("display/hidereadfeeds", false).toBool();

    QString querystring("SELECT fo.name AS title, fo.id AS id, (SELECT COUNT(id) FROM feeds WHERE folderId = fo.id) AS feedCount, localUnreadCount AS unreadCount, '1' AS type, '' AS iconSource, '' AS iconWidth, '' AS iconHeight FROM folders fo ");

    if (hideReadFeeds)
        querystring.append("WHERE localUnreadCount > 0 ");

    querystring.append("UNION ");
    querystring.append("SELECT fe.title, fe.id AS id, '0' AS feedCount, fe.localUnreadCount AS unreadCount, '0' AS type, fe.iconSource, fe.iconWidth, fe.iconHeight FROM feeds fe WHERE folderId = 0 ");

    if (hideReadFeeds)
        querystring.append("AND fe.localUnreadCount > 0 ");

    querystring.append("UNION ");
    querystring.append(QString("SELECT '%1' AS title, '0' AS id, '0' AS feedCount, ((SELECT IFNULL(SUM(localUnreadCount),0) FROM feeds WHERE folderId = 0) + (SELECT SUM(localUnreadCount) FROM folders)) AS unreadCount, '-1' AS type, '' AS iconSource, '' AS iconWidth, '' AS iconHeight ").arg(tr("All posts")));

    query.exec(QString("SELECT id FROM items WHERE starred = %1 LIMIT 1;").arg(SQL_TRUE));
    if (query.next())
    {
        querystring.append("UNION ");
        querystring.append(QString("SELECT '%1' AS title, '1' AS id, '0' AS feedCount, (SELECT COUNT(id) FROM items WHERE starred = %2) AS unreadCount, '-1' AS type, '' AS iconSource, '' AS iconWidth, '' AS iconHeight ").arg(tr("Favourite posts")).arg(SQL_TRUE));

    }

    QString itemOrder = dbus.getSetting("display/orderby", "id").toString();

    if (itemOrder == "id") {
        querystring.append("ORDER BY type ASC, id ASC;");
    } else if (itemOrder == "title") {
        querystring.append("ORDER BY type ASC, title COLLATE NOCASE ASC;");
    } else {
        querystring.append("ORDER BY type ASC, unreadCount DESC");
    }

    this->setQuery(querystring);
}

