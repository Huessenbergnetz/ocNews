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

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    setRoleNames(roleNames());
#endif
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

    int length = 2;

    QString querystring("SELECT ((SELECT COUNT(id) FROM feeds WHERE folderId = 0) + (SELECT COUNT(id) FROM folders))");

    query.exec(querystring);

    query.next();

    length += query.value(0).toInt();

//    if (length > 0)
//    {
        querystring = "SELECT fo.id AS id, 1 AS type, fo.name AS title, localUnreadCount AS unreadCount, '' AS iconSource, '' AS iconWidth, '' AS iconHeight, (SELECT COUNT(id) FROM feeds WHERE folderId = fo.id) AS feedCount FROM folders fo ";

        querystring.append("UNION ");
        querystring.append("SELECT fe.id AS id, 0 AS type, fe.title, fe.localUnreadCount AS unreadCount, fe.iconSource, fe.iconWidth, fe.iconHeight, 0 AS feedCount FROM feeds fe WHERE folderId = 0 ");

        querystring.append("UNION ");
        querystring.append(QString("SELECT -1 AS id, 3 AS type, '%1' AS title, ((SELECT IFNULL(SUM(localUnreadCount),0) FROM feeds WHERE folderId = 0) + (SELECT IFNULL(SUM(localUnreadCount),0) FROM folders)) AS unreadCount, '' AS iconSource, '' AS iconWidth, '' AS iconHeight, 0 AS feedCount ").arg(tr("All posts")));

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
//    }
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




void OcFoldersModelNew::itemsUpdated(const QList<int> &updated, const QList<int> &newItems, const QList<int> &deleted)
{

    if (!updated.isEmpty() || !newItems.isEmpty() || !deleted.isEmpty())
    {
        itemsMarked();

        itemsStarred();
    }

}


void OcFoldersModelNew::itemsMarked()
{
    if (!active())
        return;

    QHash<int, int> idsAndUnread;

    QSqlQuery query;

    QString queryString("SELECT id, localUnreadCount FROM feeds WHERE folderId = 0");

    query.exec(queryString);

    while(query.next()) {
        idsAndUnread[query.value(0).toInt()] = query.value(1).toInt();
    }

    queryString = "SELECT id, localUnreadCount FROM folders";

    query.exec(queryString);

    while(query.next()) {
        idsAndUnread[query.value(0).toInt()] = query.value(1).toInt();
    }

    queryString = "SELECT ((SELECT IFNULL(SUM(localUnreadCount),0) FROM feeds WHERE folderId = 0) + (SELECT IFNULL(SUM(localUnreadCount),0) FROM folders))";

    query.exec(queryString);

    query.next();

    idsAndUnread[-1] = query.value(0).toInt();

    setTotalUnread(query.value(0).toInt());

    for (int i = 0; i < rowCount(); ++i)
    {
        if (m_items.at(i)->id != -2) {

            if (m_items.at(i)->unreadCount != idsAndUnread[m_items.at(i)->id]) {
                m_items.at(i)->unreadCount = idsAndUnread[m_items.at(i)->id];

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
                emit dataChanged(index(i), index(i), QVector<int>(1, UnreadCountRole));
#else
                emit dataChanged(index(i), index(i));
#endif

            }
        }
    }
}


void OcFoldersModelNew::itemsStarred()
{
    if (!active())
        return;

    QSqlQuery query;

    query.exec(QString("SELECT COUNT(id) FROM items WHERE starred = %1").arg(SQL_TRUE));

    query.next();

    int uc = query.value(0).toInt();

    int idx = findIndex(-2, 2);

    if (m_items.at(idx)->unreadCount != uc) {

        m_items.at(idx)->unreadCount = uc;

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
        emit dataChanged(index(idx), index(idx), QVector<int>(1, UnreadCountRole));
#else
        emit dataChanged(index(idx), index(idx));
#endif
    }
}



void OcFoldersModelNew::itemsMarkedAllRead()
{
    if (!active())
        return;

    for (int i = 0; i < rowCount(); ++i)
    {
        if (m_items.at(i)->id != -2)
            m_items.at(i)->unreadCount = 0;
    }

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    emit dataChanged(index(0), index(rowCount()-1), QVector<int>(1, UnreadCountRole));
#else
    emit dataChanged(index(0), index(rowCount()-1));
#endif

    setTotalUnread(0);

}



void OcFoldersModelNew::feedsRequested(const QList<int> &updated, const QList<int> &newFeeds, const QList<int> &deleted)
{
    if (!active())
        return;

    if (updated.isEmpty() && newFeeds.isEmpty() && deleted.isEmpty())
        return;

    QSqlQuery query;

    if (!updated.isEmpty()) {

        for (int i = 0; i < updated.size(); ++i) {

            int idx = findIndex(updated.at(i), 0);

            query.exec(QString("SELECT title, iconSource, iconWidth, iconHeight, folderId, localUnreadCount FROM feeds WHERE id = %1").arg(updated.at(i)));

            query.next();

            int folderId = query.value(4).toInt();

            // check if updated feed is still child of root folder
            if ((idx != -999) && (folderId == 0)) {

                //update the feed, because it is still children of root
                m_items.at(idx)->title = query.value(0).toString();
                m_items.at(idx)->iconSource = query.value(1).toString();
                m_items.at(idx)->iconWidth = query.value(2).toInt();
                m_items.at(idx)->iconHeight = query.value(3).toInt();

                emit dataChanged(index(idx), index(idx));

            } else if ((idx != -999) && (folderId != 0)) {

                //the updated feed is no longer child of root, remove it
                beginRemoveRows(QModelIndex(), idx, idx);

                delete m_items.takeAt(idx);

                endRemoveRows();

            } else if ((idx == -999) && (folderId == 0)) {

                // the feed has been moved to the root folder

                beginInsertRows(QModelIndex(), 0, 0);

                OcFolderObject *fobj = new OcFolderObject(updated.at(i),
                                                          0,
                                                          query.value(0).toString(),
                                                          query.value(5).toInt(),
                                                          query.value(1).toString(),
                                                          query.value(2).toInt(),
                                                          query.value(3).toInt(),
                                                          0);

                m_items.prepend(fobj);

                endInsertRows();
            }
        }
    }


    if (!newFeeds.isEmpty()) {

        QString feedList("(");

        for (int i = 0; i < newFeeds.size(); ++i) {
            feedList.append(QString::number(newFeeds.at(i)));
            feedList.append(", ");
        }

        feedList.chop(2);
        feedList.append(")");

        query.exec(QString("SELECT COUNT(id) FROM feeds WHERE folderId = 0 AND id IN %1").arg(feedList));

        query.next();

        int length = query.value(0).toInt();

        if (length > 0) {

            query.exec(QString("SELECT id, title, localUnreadCount, iconSource, iconWidth, iconHeight FROM feeds WHERE folderId = 0 AND id IN %1").arg(feedList));

            beginInsertRows(QModelIndex(), rowCount(), (rowCount() + length - 1));

            while(query.next()) {

                OcFolderObject *fobj = new OcFolderObject(query.value(0).toInt(),
                                                          0,
                                                          query.value(1).toString(),
                                                          query.value(2).toInt(),
                                                          query.value(3).toString(),
                                                          query.value(4).toInt(),
                                                          query.value(5).toInt(),
                                                          0);
                m_items.append(fobj);
            }

            endInsertRows();
        }
    }


    if (!deleted.isEmpty()) {

        QList<int> idxs = QList<int>();

        for (int i = 0; i < deleted.size(); ++i) {
            int idx = findIndex(deleted.at(i), 0);
            if (idx != -999)
                idxs << idx;
        }

        if (!idxs.isEmpty()) {

            for (int j = 0; j < idxs.size(); ++j) {

                int idx = idxs.at(j);

                beginRemoveRows(QModelIndex(), idx, idx);

                delete m_items.takeAt(idx);

                endRemoveRows();
            }
        }
    }
}



void OcFoldersModelNew::feedCreated(const QString &name, const int &id)
{
    if (!active())
        return;

    QSqlQuery query;

    query.exec(QString("SELECT localUnreadCount, iconSource, iconWidth, iconHeight, folderId FROM feeds WHERE id = %1").arg(id));

    query.next();

    if (query.value(4).toInt() == 0) {

        beginInsertRows(QModelIndex(), 0 , 0);

        OcFolderObject *fobj = new OcFolderObject(id,
                                                  0,
                                                  name,
                                                  query.value(0).toInt(),
                                                  query.value(1).toString(),
                                                  query.value(2).toInt(),
                                                  query.value(3).toInt(),
                                                  0);
        m_items.prepend(fobj);

        endInsertRows();
    }

    queryAndSetTotalCount();
}



void OcFoldersModelNew::feedDeleted(const int &id)
{
    if (!active())
        return;

    int idx = findIndex(id, 0);

    if (idx != -999) {

        beginRemoveRows(QModelIndex(), idx, idx);

        delete m_items.takeAt(idx);

        endRemoveRows();
    }

    queryAndSetTotalCount();
}


void OcFoldersModelNew::feedMarkedRead(const int &id)
{
    if (!active())
        return;

    int idx = findIndex(id, 0);

    if (idx != -999) {

        m_items.at(idx)->unreadCount = 0;

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
        emit dataChanged(index(idx), index(idx), QVector<int>(1, UnreadCountRole));
#else
        emit dataChanged(index(idx), index(idx));
#endif

    }

    queryAndSetTotalCount();
}


void OcFoldersModelNew::feedMoved(const int &feedId, const int &folderId)
{
    if (!active())
        return;

    int idx = findIndex(feedId, 0);

    if ((idx == -999) && (folderId == 0)) {

        QSqlQuery query;

        query.exec(QString("SELECT title, localUnreadCount, iconSource, iconWidth, iconHeight FROM feeds WHERE id = %1").arg(feedId));

        query.next();

        beginInsertRows(QModelIndex(), 0, 0);

        OcFolderObject *fobj = new OcFolderObject(feedId,
                                                  0,
                                                  query.value(0).toString(),
                                                  query.value(1).toInt(),
                                                  query.value(2).toString(),
                                                  query.value(3).toInt(),
                                                  query.value(4).toInt(),
                                                  0);
        m_items.prepend(fobj);

        endInsertRows();

    } else if ((idx != -999) && (folderId != 0)) {

        beginRemoveRows(QModelIndex(), idx, idx);

        delete m_items.takeAt(idx);

        endRemoveRows();
    }

    itemsMarked();

}



void OcFoldersModelNew::feedRenamed(const QString &newName, const int &feedId)
{
    if (!active())
        return;

    int idx = findIndex(feedId, 0);

    if (idx != -999)
    {
        m_items.at(idx)->title = newName;

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
        emit dataChanged(index(idx), index(idx), QVector<int>(1, TitleRole));
#else
        emit dataChanged(index(idx), index(idx));
#endif
    }
}


void OcFoldersModelNew::foldersRequested(const QList<int> &updated, const QList<int> &newFolders, const QList<int> &deleted)
{
    if (!active())
        return;

    if (updated.isEmpty() && newFolders.isEmpty() && deleted.isEmpty())
        return;

    QSqlQuery query;

    if (!updated.isEmpty()) {

        for (int i = 0; i < updated.size(); ++i) {

            int idx = findIndex(updated.at(i), 1);

            query.exec(QString("SELECT name FROM folders WHERE id = %1").arg(updated.at(i)));

            query.next();

            m_items.at(idx)->title = query.value(0).toString();

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
            emit dataChanged(index(idx), index(idx), QVector<int>(1, TitleRole));
#else
            emit dataChanged(index(idx), index(idx));
#endif
        }
    }


    if (!newFolders.isEmpty()) {

        for (int i = 0; i < newFolders.size(); ++i) {

            query.exec(QString("SELECT fo.name, fo.localUnreadCount, (SELECT COUNT(id) FROM feeds WHERE folderId = fo.id) AS feedCount FROM folders fo WHERE id = %1").arg(newFolders.at(i)));

            query.next();

            beginInsertRows(QModelIndex(), rowCount(), rowCount());

            OcFolderObject *fobj = new OcFolderObject(newFolders.at(i),
                                                      1,
                                                      query.value(0).toString(),
                                                      query.value(1).toInt(),
                                                      "",
                                                      0,
                                                      0,
                                                      query.value(2).toInt());
            m_items.append(fobj);

            endInsertRows();
        }
    }


    if (!deleted.isEmpty()) {

        QList<int> idxs = QList<int>();

        for (int i = 0; i < deleted.size(); ++i) {
            int idx = findIndex(deleted.at(i), 1);
            if (idx != -999)
                idxs << idx;
        }

        if (!idxs.isEmpty()) {

            for (int j = 0; j < idxs.size(); ++j) {
                int idx = idxs.at(j);
                beginRemoveRows(QModelIndex(), idx, idx);

                delete m_items.takeAt(idx);

                endRemoveRows();
            }
        }
    }
}


void OcFoldersModelNew::folderCreated(const QString &foldername, const int &folderId)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());

    OcFolderObject *fobj = new OcFolderObject(folderId, 1, foldername, 0, "", 0, 0, 0);

    m_items.append(fobj);

    endInsertRows();
}



void OcFoldersModelNew::folderDeleted(const int &id)
{
    int idx = findIndex(id, 1);

    if (idx != -999) {

        beginRemoveRows(QModelIndex(), idx, idx);

        delete m_items.takeAt(idx);

        endRemoveRows();
    }

    queryAndSetTotalCount();
}


void OcFoldersModelNew::folderMarkedRead(const int &id)
{
    int idx = findIndex(id, 1);

    if (idx != -999) {

        m_items.at(idx)->unreadCount = 0;

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
        emit dataChanged(index(idx), index(idx), QVector<int>(1, UnreadCountRole));
#else
        emit dataChanged(index(idx), index(idx));
#endif
    }

    queryAndSetTotalCount();
}


void OcFoldersModelNew::folderRenamed(const QString &newName, const int &id)
{
    int idx = findIndex(id, 1);

    if (idx != -999) {

        m_items.at(idx)->title = newName;

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
        emit dataChanged(index(idx), index(idx), QVector<int>(1, TitleRole));
#else
        emit dataChanged(index(idx), index(idx));
#endif
    }
}

void OcFoldersModelNew::databaseCleaned()
{
    if (!active())
        return;

    init();
}



int OcFoldersModelNew::findIndex(const int &id, const int &type) const
{
    if (!m_items.isEmpty()) {
        for (int i = 0; i < rowCount(); ++i) {
            if ((m_items.at(i)->id == id) && (m_items.at(i)->type == type))
                return i;
        }
    }

    return -999;
}


void OcFoldersModelNew::queryAndSetTotalCount()
{
    QSqlQuery query;

    query.exec("SELECT ((SELECT IFNULL(SUM(localUnreadCount),0) FROM feeds WHERE folderId = 0) + (SELECT IFNULL(SUM(localUnreadCount),0) FROM folders))");

    query.next();

    int totalCount = query.value(0).toInt();

    int idx = findIndex(-1, 3);

    m_items.at(idx)->unreadCount = totalCount;

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    emit dataChanged(index(idx), index(idx), QVector<int>(1, UnreadCountRole));
#else
    emit dataChanged(index(idx), index(idx));
#endif

    setTotalUnread(totalCount);
}

