#include "ocfoldersmodelnew.h"
#include "objects/ocfolerdobject.h"
#include <QDebug>

const int OcFoldersModelNew::IdRole = Qt::UserRole + 1;
const int OcFoldersModelNew::TypeRole = Qt::UserRole + 2;
const int OcFoldersModelNew::TitleRole = Qt::UserRole + 3;
const int OcFoldersModelNew::UnreadCountRole = Qt::UserRole + 4;
const int OcFoldersModelNew::IconSourceRole = Qt::UserRole + 5;
const int OcFoldersModelNew::IconWidthRole = Qt::UserRole + 6;
const int OcFoldersModelNew::IconHeightRole = Qt::UserRole + 7;
const int OcFoldersModelNew::FeedCountRole = Qt::UserRole + 8;

OcFoldersModelNew::OcFoldersModelNew(QObject *parent) :
    QAbstractListModel(parent)
{
    m_items = QList<OcFolderObject*>();
    m_active = false;
    m_totalUnread = 0;
}


bool OcFoldersModelNew::active() const { return m_active; }

void OcFoldersModelNew::setActive(const bool &nActive)
{
    if (nActive != m_active) {
        m_active = nActive;

        if (active()) {
            init();
        } else {
            clear();
        }

        emit activeChanged(active());
    }
}


int OcFoldersModelNew::totalUnread() const { return m_totalUnread; }

void OcFoldersModelNew::setTotalUnread(const int &nTotalUnread)
{
    if (nTotalUnread != m_totalUnread) {
        m_totalUnread = nTotalUnread;
        emit totalUnreadChanged(totalUnread());
    }
}


int OcFoldersModelNew::rowCount(const QModelIndex &parent) const
{
    return m_items.size();
}


QHash<int, QByteArray> OcFoldersModelNew::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();
    roles.insert(IdRole, QByteArray("id"));
    roles.insert(TypeRole, QByteArray("type"));
    roles.insert(TitleRole, QByteArray("title"));
    roles.insert(UnreadCountRole, QByteArray("unreadCount"));
    roles.insert(IconSourceRole, QByteArray("iconSource"));
    roles.insert(IconWidthRole, QByteArray("iconWidth"));
    roles.insert(IconHeightRole, QByteArray("iconHeight"));
    roles.insert(FeedCountRole, QByteArray("feedCount"));
    return roles;
}


QVariant OcFoldersModelNew::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() > (m_items.size()-1))
        return QVariant();

    OcFolderObject *fobj = m_items.at(index.row());

    switch(role) {
    case IdRole:
        return QVariant::fromValue(fobj->id);
    case TypeRole:
        return QVariant::fromValue(fobj->type);
    case TitleRole:
        return QVariant::fromValue(fobj->title);
    case UnreadCountRole:
        return QVariant::fromValue(fobj->unreadCount);
    case IconSourceRole:
        if (!fobj->iconSource.isEmpty()) {
            return QVariant::fromValue(QDir::homePath() + BASE_PATH + "/favicons/" + fobj->iconSource);
        } else {
            return QVariant();
        }
    case IconWidthRole:
        return QVariant::fromValue(fobj->iconWidth);
    case IconHeightRole:
        return QVariant::fromValue(fobj->iconHeight);
    case FeedCountRole:
        return QVariant::fromValue(fobj->feedCount);
    default:
        return QVariant();
    }
}


QModelIndex OcFoldersModelNew::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    return createIndex(row, column);
}



void OcFoldersModelNew::init()
{
#ifdef QT_DEBUG
    qDebug() << "Init Folder Model";
#endif

    clear();

    QSqlQuery query;

    int length = 0;

    QString querystring("SELECT ((SELECT COUNT(id) FROM feeds WHERE folderId = 0) + (SELECT COUNT(id) FROM folders))");

    query.exec(querystring);

    query.next();

    length = query.value(0).toInt();

    if (length > 0)
    {

        querystring = "SELECT fo.id AS id, 1 AS type, fo.name AS title, localUnreadCount AS unreadCount, '' AS iconSource, '' AS iconWidth, '' AS iconHeight, (SELECT COUNT(id) FROM feeds WHERE folderId = fo.id) AS feedCount FROM folders fo ";

        querystring.append("UNION ");
        querystring.append("SELECT fe.id AS id, 0 AS type, fe.title, fe.localUnreadCount AS unreadCount, fe.iconSource, fe.iconWidth, fe.iconHeight, 0 AS feedCount FROM feeds fe WHERE folderId = 0 ");

        querystring.append("UNION ");
        querystring.append(QString("SELECT -1 AS id, 3 AS type, '%1' AS title, ((SELECT IFNULL(SUM(localUnreadCount),0) FROM feeds WHERE folderId = 0) + (SELECT SUM(localUnreadCount) FROM folders)) AS unreadCount, '' AS iconSource, '' AS iconWidth, '' AS iconHeight, 0 AS feedCount ").arg(tr("All posts")));

        querystring.append("UNION ");
        querystring.append(QString("SELECT -2 AS id, 2 AS type, '%1' AS title, (SELECT COUNT(id) FROM items WHERE starred = %2) AS unreadCount, '' AS iconSource, '' AS iconWidth, '' AS iconHeight, 0 AS feedCount ").arg(tr("Favourite posts")).arg(SQL_TRUE));

        querystring.append("ORDER BY TYPE DESC");

        query.exec(querystring);

        beginInsertRows(QModelIndex(), 0, length-1);

        while(query.next()) {

            OcFolderObject *fobj = new OcFolderObject(query.value(0).toInt(),
                                                      query.value(1).toInt(),
                                                      query.value(2).toString(),
                                                      query.value(3).toInt(),
                                                      query.value(4).toString(),
                                                      query.value(5).toInt(),
                                                      query.value(6).toInt(),
                                                      query.value(7).toInt());

            m_items.append(fobj);

            if (fobj->type == 3)
                setTotalUnread(fobj->unreadCount);

        }

        endInsertRows();
    }
}



void OcFoldersModelNew::clear()
{
    beginRemoveRows(QModelIndex(), 0, rowCount()-1);

    while (!m_items.isEmpty()) {
        delete m_items.takeLast();
    }

    m_items.clear();

    endRemoveRows();
}



int OcFoldersModelNew::findIndex(const int &id) const
{
    for (int i = 0; i < rowCount(); ++i) {
        if (m_items.at(i)->id == id)
            return i;
    }

    return -999;
}


void OcFoldersModelNew::queryAndSetTotalCount()
{
    QSqlQuery query;

    query.exec("SELECT ((SELECT IFNULL(SUM(localUnreadCount),0) FROM feeds WHERE folderId = 0) + (SELECT SUM(localUnreadCount) FROM folders))");

    query.next();

    int totalCount = query.value(0).toInt();

    int idx = findIndex(-1);

    m_items.at(idx)->unreadCount = totalCount;

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    emit dataChanged(index(idx), index(idx), QVector<int>(1, UnreadCountRole));
#else
    emit dataChanged(index(idx), index(idx));
#endif

    setTotalUnread(totalCount);
}
