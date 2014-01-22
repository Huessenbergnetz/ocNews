#include <QDebug>
#include "ocfoldermodelsql.h"

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
    }
    return value;
}

void OcFolderModelSql::refresh()
{
#ifdef QT_DEBUG
    qDebug() << "Refresh Folder Model";
#endif

    QSqlQuery query;

    QString querystring("SELECT fo.name AS title, fo.id AS id, (SELECT COUNT(id) FROM feeds WHERE folderId = fo.id) AS feedCount, localUnreadCount AS unreadCount, '1' AS type, '' AS iconSource, '' AS iconWidth, '' AS iconHeight FROM folders fo ");
    querystring.append("UNION ");
    querystring.append("SELECT fe.title, fe.id AS id, '0' AS feedCount, fe.localUnreadCount AS unreadCount, '0' AS type, fe.iconSource, fe.iconWidth, fe.iconHeight FROM feeds fe WHERE folderId = 0 ");
    querystring.append("UNION ");
    querystring.append(QString("SELECT '%1' AS title, '0' AS id, '0' AS feedCount, ((SELECT IFNULL(SUM(localUnreadCount),0) FROM feeds WHERE folderId = 0) + (SELECT SUM(localUnreadCount) FROM folders)) AS unreadCount, '-1' AS type, '' AS iconSource, '' AS iconWidth, '' AS iconHeight ").arg(tr("All posts")));

#if defined(MEEGO_EDITION_HARMATTAN)
    query.exec("SELECT id FROM items WHERE starred = \"true\" LIMIT 1;");
#else
    query.exec("SELECT id FROM items WHERE starred = 1 LIMIT 1;");
#endif
    if (query.next())
    {
#if defined(MEEGO_EDITION_HARMATTAN)
        QString iconSource = (dbus.getSetting("display/themecolor", "white").toString() == "white") ? "image://theme/icon-m-content-favourites" : "image://theme/icon-m-content-favourites-inverse";
#else
        QString iconSource = "image://theme/icon-m-favorite-selected";
#endif

        querystring.append("UNION ");
#if defined(MEEGO_EDITION_HARMATTAN)
        querystring.append(QString("SELECT '%1' AS title, '1' AS id, '0' AS feedCount, (SELECT COUNT(id) FROM items WHERE starred = \"true\") AS unreadCount, '-1' AS type, '%2' AS iconSource, '64' AS iconWidth, '64' AS iconHeight ").arg(tr("Favourite posts")).arg(iconSource));
#else
        querystring.append(QString("SELECT '%1' AS title, '1' AS id, '0' AS feedCount, (SELECT COUNT(id) FROM items WHERE starred = 1) AS unreadCount, '-1' AS type, '%2' AS iconSource, '64' AS iconWidth, '64' AS iconHeight ").arg(tr("Favourite posts")).arg(iconSource));
#endif
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

