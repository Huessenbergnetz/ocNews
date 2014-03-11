#include "occombinedmodelsql.h"
#include "../../../common/globals.h"

const char* OcCombinedModelSql::COLUMN_NAMES[] = {
    "title",
    "id",
    "unreadCount",
    "type",
    "iconSource",
    "iconWidth",
    "iconHeight",
    "folderId",
    "folderName",
    NULL
};

OcCombinedModelSql::OcCombinedModelSql(QObject *parent) :
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
QHash<int,QByteArray> OcCombinedModelSql::roleNames() const
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



QVariant OcCombinedModelSql::data(const QModelIndex &index, int role) const
{
    QVariant value = QSqlQueryModel::data(index, role);
    if(role < Qt::UserRole)
    {
        value = QSqlQueryModel::data(index, role);
    } else {
        int columnIdx = role - Qt::UserRole - 1;
        QModelIndex modelIndex = this->index(index.row(), columnIdx);
        value = QSqlQueryModel::data(modelIndex, Qt::DisplayRole);
        if (columnIdx == 4 && value != "") {
            QString iconSrcPath = QDir::homePath() + BASE_PATH + "/favicons/" + value.toString();
            value = iconSrcPath;
        }
    }
    return value;
}



void OcCombinedModelSql::refresh()
{
#ifdef QT_DEBUG
    qDebug() << "Refresh Combined Model";
#endif

    bool hideReadFeeds = dbus.getSetting("display/hidereadfeeds", false).toBool();

    QSqlQuery query;

    QString querystring("SELECT fe.title, fe.id AS id, fe.localUnreadCount AS unreadCount, '0' AS type, fe.iconSource, fe.iconWidth, fe.iconHeight, fe.folderId, (SELECT name FROM folders WHERE id = fe.folderId) AS folderName FROM feeds fe WHERE fe.folderId > 0 ");

    if (hideReadFeeds)
        querystring.append("AND fe.localUnreadCount > 0 ");

    querystring.append("UNION ");

    querystring.append(QString("SELECT fe.title, fe.id AS id, fe.localUnreadCount AS unreadCount, '0' AS type, fe.iconSource, fe.iconWidth, fe.iconHeight, fe.folderId, '%1' AS folderName FROM feeds fe WHERE fe.folderId = 0 ").arg(tr("Uncategorized")));

    if (hideReadFeeds)
        querystring.append("AND fe.localUnreadCount > 0 ");

    querystring.append("UNION ");

    querystring.append(QString("SELECT '%1' AS title, '0' AS id, ((SELECT IFNULL(SUM(localUnreadCount),0) FROM feeds WHERE folderId = 0) + (SELECT SUM(localUnreadCount) FROM folders)) AS unreadCount, '-1' AS type, '' AS iconSource, '' AS iconWidth, '' AS iconHeight, 0 AS folderId, '' AS folderName ").arg(tr("All posts")));

    query.exec(QString("SELECT id FROM items WHERE starred = %1 LIMIT 1").arg(SQL_TRUE));
    if (query.next())
    {

        querystring.append("UNION ");

        querystring.append(QString("SELECT '%1' AS title, '1' AS id, (SELECT COUNT(id) FROM items WHERE starred = %2) AS unreadCount, '-1' AS type, '' AS iconSource, '' AS iconWidth, '' AS iconHeight, 0 AS folderId, '' AS folderName ").arg(tr("Favourite posts")).arg(SQL_TRUE));
    }

    QString itemOrder = dbus.getSetting("display/orderby", "id").toString();

    if (itemOrder == "id") {
        querystring.append("ORDER BY type ASC, folderId ASC, id ASC;");
    } else if (itemOrder == "title") {
        querystring.append("ORDER BY type ASC, folderName COLLATE NOCASE ASC, title COLLATE NOCASE ASC;");
    } else {
        querystring.append("ORDER BY type ASC, folderName COLLATE NOCASE ASC, unreadCount DESC");
    }

    this->setQuery(querystring);
}
