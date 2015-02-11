#include "occombinedmodelnew.h"
#include "objects/occombinedobject.h"
#include "QsLog.h"

const int OcCombinedModelNew::IdRole = Qt::UserRole + 1;
const int OcCombinedModelNew::TypeRole = Qt::UserRole + 2;
const int OcCombinedModelNew::TitleRole = Qt::UserRole + 3;
const int OcCombinedModelNew::UnreadCountRole = Qt::UserRole + 4;
const int OcCombinedModelNew::IconSourceRole = Qt::UserRole + 5;
const int OcCombinedModelNew::IconWidthRole = Qt::UserRole + 6;
const int OcCombinedModelNew::IconHeightRole = Qt::UserRole + 7;
const int OcCombinedModelNew::FolderIdRole = Qt::UserRole + 8;
const int OcCombinedModelNew::FolderNameRole = Qt::UserRole + 9;

OcCombinedModelNew::OcCombinedModelNew(QObject *parent) :
    QAbstractListModel(parent)
{
    m_items = QList<OcCombinedObject*>();
    m_active = false;
    m_totalUnread = 0;

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    setRoleNames(roleNames());
#endif
}


bool OcCombinedModelNew::active() const { return m_active; }

void OcCombinedModelNew::setActive(const bool &nActive)
{
    if (nActive != m_active) {
        m_active = nActive;

        QLOG_DEBUG() << "Combined model: set active to " << active();

        if (active()) {
            init();
        } else {
            clear();
        }

        emit activeChanged(active());
    }
}


int OcCombinedModelNew::totalUnread() const { return m_totalUnread; }

void OcCombinedModelNew::setTotalUnread(const int &nTotalUnread)
{
    if (nTotalUnread != m_totalUnread) {
        m_totalUnread = nTotalUnread;
        QLOG_DEBUG() << "Combined model: set total unread to " << totalUnread();
        emit totalUnreadChanged(totalUnread());
    }
}



QHash<int, QByteArray> OcCombinedModelNew::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();
    roles.insert(IdRole, QByteArray("id"));
    roles.insert(TypeRole, QByteArray("type"));
    roles.insert(TitleRole, QByteArray("title"));
    roles.insert(UnreadCountRole, QByteArray("unreadCount"));
    roles.insert(IconSourceRole, QByteArray("iconSource"));
    roles.insert(IconWidthRole, QByteArray("iconWidth"));
    roles.insert(IconHeightRole, QByteArray("iconHeight"));
    roles.insert(FolderIdRole, QByteArray("folderId"));
    roles.insert(FolderNameRole, QByteArray("folderName"));
    return roles;
}


int OcCombinedModelNew::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_items.size();
}


QVariant OcCombinedModelNew::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() > (m_items.size()-1))
        return QVariant();

    OcCombinedObject *cobj = m_items.at(index.row());

    switch(role) {
    case IdRole:
        return QVariant::fromValue(cobj->id);
    case TypeRole:
        return QVariant::fromValue(cobj->type);
    case TitleRole:
        return QVariant::fromValue(cobj->title);
    case UnreadCountRole:
        return QVariant::fromValue(cobj->unreadCount);
    case IconSourceRole:
        if (!cobj->iconSource.isEmpty()) {
            return QVariant::fromValue(QDir::homePath() + BASE_PATH + "/favicons/" + cobj->iconSource);
        } else {
            return QVariant::fromValue(QString());
        }
    case IconWidthRole:
        return QVariant::fromValue(cobj->iconWidth);
    case IconHeightRole:
        return QVariant::fromValue(cobj->iconHeight);
    case FolderIdRole:
        return QVariant::fromValue(cobj->folderId);
    case FolderNameRole:
        return QVariant::fromValue(cobj->folderName);
    default:
        return QVariant();
    }
}


QModelIndex OcCombinedModelNew::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    return createIndex(row, column);
}



void OcCombinedModelNew::init()
{
    QLOG_INFO() << "Initialize combined model";

    clear();

    QSqlQuery query;

    int length = 2;

    QString querystring("SELECT COUNT(id) FROM feeds");

    if (!query.exec(querystring)) {
        QLOG_ERROR() << "Combined model: failed to select feed count from database: " << query.lastError().text();
    }

    query.next();

    length += query.value(0).toInt();

//    if (length > 0)
//    {
//        length += 2;

        querystring = "SELECT fe.id AS id, 0 AS type, fe.title, fe.localUnreadCount AS unreadCount, fe.iconSource, fe.iconWidth, fe.iconHeight, fe.folderId, (SELECT name FROM folders WHERE id = fe.folderId) AS folderName FROM feeds fe WHERE fe.folderId > 0 ";

        querystring.append("UNION ");

        querystring.append(QString("SELECT fe.id AS id, 0 AS type, fe.title, fe.localUnreadCount AS unreadCount, fe.iconSource, fe.iconWidth, fe.iconHeight, fe.folderId, '%1' AS folderName FROM feeds fe WHERE fe.folderId = 0 ").arg(tr("Uncategorized")));

        querystring.append("UNION ");

        querystring.append(QString("SELECT -1 AS id, 3 AS type, '%1' AS title, ((SELECT IFNULL(SUM(localUnreadCount),0) FROM feeds WHERE folderId = 0) + (SELECT IFNULL(SUM(localUnreadCount),0) FROM folders)) AS unreadCount, '' AS iconSource, '' AS iconWidth, '' AS iconHeight, 0 AS folderId, '' AS folderName ").arg(tr("All posts")));

        querystring.append("UNION ");

        querystring.append(QString("SELECT -2 AS id, 2 AS type, '%1' AS title, (SELECT COUNT(id) FROM items WHERE starred = %2) AS unreadCount, '' AS iconSource, '' AS iconWidth, '' AS iconHeight, 0 AS folderId, '' AS folderName ").arg(tr("Favourite posts")).arg(SQL_TRUE));

        querystring.append("ORDER BY TYPE DESC");

        if (!query.exec(querystring)) {
            QLOG_ERROR() << "Combined model: failed to select items from database: " << query.lastError().text();
        }

        beginInsertRows(QModelIndex(), 0, length-1);

        while(query.next())
        {
            OcCombinedObject *cobj = new OcCombinedObject(query.value(0).toInt(),
                                                          query.value(1).toInt(),
                                                          query.value(2).toString(),
                                                          query.value(3).toInt(),
                                                          query.value(4).toString(),
                                                          query.value(5).toInt(),
                                                          query.value(6).toInt(),
                                                          query.value(7).toInt(),
                                                          query.value(8).toString());
            m_items.append(cobj);

            if (cobj->type == 3)
                setTotalUnread(cobj->unreadCount);
        }

        endInsertRows();
//    }
}


void OcCombinedModelNew::clear()
{
    QLOG_INFO() << "Clearing combined model";

    beginRemoveRows(QModelIndex(), 0, rowCount()-1);

    while (!m_items.isEmpty()) {
        delete m_items.takeLast();
    }

    m_items.clear();

    endRemoveRows();
}



void OcCombinedModelNew::itemsUpdated(const QList<int> &updated, const QList<int> &newItems, const QList<int> &deleted)
{
    if (!updated.isEmpty() || !newItems.isEmpty() || !deleted.isEmpty())
    {
        itemsMarked();

        itemsStarred();
    }
}



void OcCombinedModelNew::feedsRequested(const QList<int> &updated, const QList<int> &newFeeds, const QList<int> &deleted)
{
    if (!active())
        return;

    if (updated.isEmpty() && newFeeds.isEmpty() && deleted.isEmpty())
        return;

    QSqlQuery query;
    QString queryString;


    if (!updated.isEmpty()) {

        QLOG_INFO() << "Combined model: updating existing items";

        for (int i = 0; i < rowCount(); ++i) {

            if (updated.contains(m_items.at(i)->id)) {

                queryString = QString("SELECT fe.id, fe.title, fe.localUnreadCount AS unreadCount, fe.iconSource, fe.iconWidth, fe.iconHeight, fe.folderId, (SELECT name FROM folders WHERE id = fe.folderId) AS folderName FROM feeds fe WHERE fe.id = %1").arg(m_items.at(i)->id);

                if (!query.exec(queryString)) {
                    QLOG_ERROR() << "Combined model: failed to select feed data from database: " << query.lastError().text();
                }

                if (query.next()) {

                    OcCombinedObject *cobj = m_items.at(i);

                    cobj->id = query.value(0).toInt();
                    cobj->title = query.value(1).toString();
                    cobj->unreadCount = query.value(2).toInt();
                    cobj->iconSource = query.value(3).toString();
                    cobj->iconWidth = query.value(4).toInt();
                    cobj->iconHeight = query.value(5).toInt();
                    cobj->folderId = query.value(6).toInt();
                    if (cobj->folderId == 0) {
                        cobj->folderName = tr("Uncategorized");
                    } else {
                        cobj->folderName = query.value(7).toString();
                    }

                    m_items.replace(i, cobj);

                    emit dataChanged(index(i), index(i));
                }
            }
        }
    }


    if (!newFeeds.isEmpty()) {

        QLOG_INFO() << "Combined model: adding new items";

        QString feedList("(");

        for (int i = 0; i < newFeeds.size(); ++i)
        {
            feedList.append(QString::number(newFeeds.at(i)));
            feedList.append(", ");
        }

        feedList.chop(2);
        feedList.append(")");

        queryString = QString("SELECT fe.id AS id, 0 AS type, fe.title, fe.localUnreadCount AS unreadCount, fe.iconSource, fe.iconWidth, fe.iconHeight, fe.folderId, (SELECT name FROM folders WHERE id = fe.folderId) AS folderName FROM feeds fe WHERE fe.id IN %1 ORDER BY TYPE DESC").arg(feedList);

        if (!query.exec(queryString)) {
            QLOG_ERROR() << "Combined model: Failed to select feed data from database: " << query.lastError().text();
        }

        beginInsertRows(QModelIndex(), rowCount(), (rowCount() + newFeeds.size()-1));

        while(query.next()) {

            OcCombinedObject *cobj = new OcCombinedObject(query.value(0).toInt(),
                                                          query.value(1).toInt(),
                                                          query.value(2).toString(),
                                                          query.value(3).toInt(),
                                                          query.value(4).toString(),
                                                          query.value(5).toInt(),
                                                          query.value(6).toInt(),
                                                          query.value(7).toInt(),
                                                          query.value(8).toString());

            m_items.append(cobj);

        }

        endInsertRows();
    }



    if (!deleted.isEmpty()) {

        QLOG_INFO() << "Combined model: removing deleted items";

        QList<int> deletedIdxs = QList<int>();

        for (int i = 0; i < rowCount(); ++i) {

            if (deleted.contains(m_items.at(i)->id))
                deletedIdxs << i;
        }


        if (!deletedIdxs.isEmpty()) {
            for (int j = 0; j < deletedIdxs.size(); ++j) {

                int idx = deletedIdxs.at(j);

                QLOG_DEBUG() << "Combined model: removing delete item at index " << idx;

                beginRemoveRows(QModelIndex(), idx, idx);

                delete m_items.takeAt(idx);

                endRemoveRows();
            }
        }
    }

    queryAndSetTotalCount();
}


void OcCombinedModelNew::itemsMarkedAllRead()
{
    if (!active())
        return;

    QLOG_INFO() << "Combined model: marking all items as read";

    for (int i = 0; i < rowCount(); ++i) {

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



void OcCombinedModelNew::itemsMarked()
{
    if (!active())
        return;

    QLOG_INFO() << "Combined model: update unread count";

    QHash<int, int> idsAndUnread;

    QSqlQuery query;

    QString queryString("SELECT id, localUnreadCount FROM feeds");

    if (!query.exec(queryString)) {
        QLOG_ERROR() << "Combined model: update unread count: failed to select feed data from database: " << query.lastError().text();
    }

    while(query.next()) {
        idsAndUnread[query.value(0).toInt()] = query.value(1).toInt();
    }

    queryString = "SELECT ((SELECT IFNULL(SUM(localUnreadCount),0) FROM feeds WHERE folderId = 0) + (SELECT IFNULL(SUM(localUnreadCount),0) FROM folders))";

    if (!query.exec(queryString)) {
        QLOG_ERROR() << "Combined model: update unread count: failed to select fedd data from database: " << query.lastError().text();
    }

    query.next();

    idsAndUnread[-1] = query.value(0).toInt();

    setTotalUnread(query.value(0).toInt());

    for (int i = 0; i < rowCount(); ++i)
    {
        OcCombinedObject *cobj = m_items.at(i);

        if (cobj->id != -2)
            if (cobj->unreadCount != idsAndUnread[cobj->id]) {
                cobj->unreadCount = idsAndUnread[cobj->id];
                m_items.replace(i, cobj);

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
                emit dataChanged(index(i), index(i), QVector<int>(1, UnreadCountRole));
#else
                emit dataChanged(index(i), index(i));
#endif
            }
    }
}



void OcCombinedModelNew::itemsStarred()
{
    if (!active())
        return;

    QLOG_INFO() << "Combined model: updating starred items count";

    QSqlQuery query;

    QString queryString;

    queryString = QString("SELECT COUNT(id) FROM items WHERE starred = %1").arg(SQL_TRUE);

    if (!query.exec(queryString)) {
        QLOG_ERROR() << "Combined model: failed to update starred items count from database: " << query.lastError().text();
    }

    query.next();

    int idx = findIndex(-2);

    m_items.at(idx)->unreadCount = query.value(0).toInt();

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    emit dataChanged(index(idx), index(idx), QVector<int>(1, UnreadCountRole));
#else
    emit dataChanged(index(idx), index(idx));
#endif
}



void OcCombinedModelNew::feedCreated(const QString &name, const int &id)
{
    if (!active())
        return;

    QLOG_INFO() << "Combined model: adding newly created feedd";

    QSqlQuery query;

    if (!query.exec(QString("SELECT fe.id AS id, 0 AS type, fe.localUnreadCount AS unreadCount, fe.iconSource, fe.iconWidth, fe.iconHeight, fe.folderId, (SELECT name FROM folders WHERE id = fe.folderId) AS folderName FROM feeds fe WHERE fe.id = %1").arg(id))) {
        QLOG_ERROR() << "Combined model: failed to add newly created feed from database: " << query.lastError();
    }

    if(query.next()) {

        beginInsertRows(QModelIndex(), rowCount(), rowCount());

        OcCombinedObject *cobj = new OcCombinedObject(query.value(0).toInt(),
                                                      query.value(1).toInt(),
                                                      name,
                                                      query.value(2).toInt(),
                                                      query.value(3).toString(),
                                                      query.value(4).toInt(),
                                                      query.value(5).toInt(),
                                                      query.value(6).toInt(),
                                                      query.value(6).toInt() == 0 ? tr("Uncategorized") : query.value(7).toString());

        m_items.append(cobj);

        endInsertRows();
    }
}



void OcCombinedModelNew::feedDeleted(const int &id)
{
    if (!active())
        return;

    int idx = findIndex(id);

    QLOG_INFO() << "Combined model: removing delted feed at index " << idx;

    beginRemoveRows(QModelIndex(), idx, idx);

    delete m_items.takeAt(idx);

    endRemoveRows();

    queryAndSetTotalCount();
}



void OcCombinedModelNew::feedMarkedRead(const int &id)
{
    if (!active())
        return;

    int idx = findIndex(id);

    QLOG_INFO() << "Combined model: marking feed at index " << idx << " as read";

    m_items.at(idx)->unreadCount = 0;

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    emit dataChanged(index(idx), index(idx), QVector<int>(1, UnreadCountRole));
#else
    emit dataChanged(index(idx), index(idx));
#endif

    queryAndSetTotalCount();
}


void OcCombinedModelNew::feedMoved(const int &feedId, const int &folderId)
{
    if (!active())
        return;

    QLOG_INFO() << "Combined model: update moved feed";

    QSqlQuery query;

    if (!query.exec(QString("SELECT name FROM folders WHERE id = %1").arg(folderId))) {
        QLOG_ERROR() << "Combined model: failed to update moved feed: failed to select new folder name from database: " << query.lastError().text();
    }

    query.next();

    int idx = findIndex(feedId);

    m_items.at(idx)->folderId = folderId;
    m_items.at(idx)->folderName = query.value(0).toString();

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    QVector<int> roles(1, FolderIdRole);
    roles.append(FolderNameRole);
    emit dataChanged(index(idx), index(idx), roles);
#else
    emit dataChanged(index(idx), index(idx));
#endif

}


void OcCombinedModelNew::feedRenamed(const QString &newName, const int &feedId)
{
    if (!active())
        return;

    int idx = findIndex(feedId);

    QLOG_INFO() << "Combined model: upadting renamed feed at index " << idx;

    m_items.at(idx)->title = newName;

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    emit dataChanged(index(idx), index(idx), QVector<int>(1, TitleRole));
#else
    emit dataChanged(index(idx), index(idx));
#endif
}


void OcCombinedModelNew::databaseCleaned()
{
    if (!active())
        return;

    init();
}



int OcCombinedModelNew::findIndex(const int &id) const
{
    if (!m_items.isEmpty()) {
        for (int i = 0; i < rowCount(); ++i) {
            if (m_items.at(i)->id == id)
                return i;
        }
    }

    return -999;
}


void OcCombinedModelNew::queryAndSetTotalCount()
{
    QSqlQuery query;

    query.exec("SELECT ((SELECT IFNULL(SUM(localUnreadCount),0) FROM feeds WHERE folderId = 0) + (SELECT IFNULL(SUM(localUnreadCount),0) FROM folders))");

    if (!query.next()) {
        QLOG_ERROR() << "Combined model: failed to query total count fromo database: " << query.lastError().text();
    }

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
