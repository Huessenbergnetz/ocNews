#include "ocfeedsmodelnew.h"
#include "objects/ocfeedobjec.h"
#include "QsLog.h"

const int OcFeedsModelNew::IdRole = Qt::UserRole + 1;
const int OcFeedsModelNew::TypeRole = Qt::UserRole + 2;
const int OcFeedsModelNew::TitleRole = Qt::UserRole + 3;
const int OcFeedsModelNew::UnreadCountRole = Qt::UserRole + 4;
const int OcFeedsModelNew::IconSourceRole = Qt::UserRole + 5;
const int OcFeedsModelNew::IconWidthRole = Qt::UserRole + 6;
const int OcFeedsModelNew::IconHeightRole = Qt::UserRole + 7;

OcFeedsModelNew::OcFeedsModelNew(QObject *parent) :
    QAbstractListModel(parent)
{
    m_folderId = -999;

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    setRoleNames(roleNames());
#endif
}


int OcFeedsModelNew::folderId() const { return m_folderId; }

void OcFeedsModelNew::setFolderId(const int &nFolderId)
{
    if (nFolderId != m_folderId) {
        m_folderId = nFolderId;
        QLOG_DEBUG() << "Feeds model: setting folder ID to " << folderId();
        init();
        emit folderIdChanged(folderId());
    }
}


QHash<int, QByteArray> OcFeedsModelNew::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();
    roles.insert(IdRole, QByteArray("id"));
    roles.insert(TypeRole, QByteArray("type"));
    roles.insert(TitleRole, QByteArray("title"));
    roles.insert(UnreadCountRole, QByteArray("unreadCount"));
    roles.insert(IconSourceRole, QByteArray("iconSource"));
    roles.insert(IconWidthRole, QByteArray("iconWidth"));
    roles.insert(IconHeightRole, QByteArray("iconHeight"));
    return roles;
}


int OcFeedsModelNew::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_items.size();
}


QVariant OcFeedsModelNew::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() > (m_items.size()-1))
        return QVariant();

    OcFeedObject *fobj = m_items.at(index.row());

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
            return QVariant::fromValue(QString());
        }
    case IconWidthRole:
        return QVariant::fromValue(fobj->iconWidth);
    case IconHeightRole:
        return QVariant::fromValue(fobj->iconHeight);
    default:
        return QVariant();
    }
}


QModelIndex OcFeedsModelNew::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    return createIndex(row, column);
}



void OcFeedsModelNew::init()
{
    QLOG_INFO() << "Initializing feeds model";

    if (!m_items.isEmpty())
        clear(false);

    QSqlQuery query;

    int length = 1;

    QString querystring(QString("SELECT COUNT(id) FROM feeds WHERE folderId = %1").arg(folderId()));

    if (!query.exec(querystring)) {
        QLOG_ERROR() << "Feeds mode: failed to select feeds count from database: " << query.lastError().text();
    }

    query.next();

    length += query.value(0).toInt();

    querystring = QString("SELECT %1 AS id, 1 AS type, '%2' AS title, (SELECT localUnreadCount FROM folders WHERE id = %1) AS unreadCount, '' AS iconSource, '' AS iconWidth, '' AS iconHeight ").arg(folderId()).arg(tr("All posts"));

    if (length > 1) {

        querystring.append("UNION ");

        querystring.append(QString("SELECT id, 0 AS type, title, localUnreadCount AS unreadCount, iconSource, iconWidth, iconHeight FROM feeds WHERE folderId = %1 ").arg(folderId()));

    }

    querystring.append("ORDER BY type DESC");

    if (!query.exec(querystring)) {
        QLOG_ERROR() << "Feeds mode: failed to select feeds from database: " << query.lastError().text();
    }

    beginInsertRows(QModelIndex(), 0, length-1);

    while(query.next())
    {
        OcFeedObject *fobj = new OcFeedObject(query.value(0).toInt(),
                                              query.value(1).toInt(),
                                              query.value(2).toString(),
                                              query.value(3).toInt(),
                                              query.value(4).toString(),
                                              query.value(5).toInt(),
                                              query.value(6).toInt());
        m_items.append(fobj);
    }

    endInsertRows();
}



void OcFeedsModelNew::clear(const bool &resetId)
{
    QLOG_INFO() << "Clearing feeds model";

    beginRemoveRows(QModelIndex(), 0, rowCount()-1);

    while (!m_items.isEmpty()) {
        delete m_items.takeLast();
    }

    m_items.clear();

    if (resetId)
        m_folderId = -999;

    endRemoveRows();
}



void OcFeedsModelNew::itemsUpdated(const QList<int> &updated, const QList<int> &newItems, const QList<int> &deleted)
{
    if (rowCount() < 2)
        return;

    if (updated.isEmpty() && newItems.isEmpty() && deleted.isEmpty())
        return;

    itemsMarked();
}


void OcFeedsModelNew::itemsMarked()
{
    if (rowCount() < 2)
        return;

    QLOG_INFO() << "Feeds model: update unread count";

    QHash<int, int> idsAndUnread;

    QSqlQuery query;

    if (!query.exec(QString("SELECT id, localUnreadCount FROM feeds WHERE folderId = %1").arg(folderId()))) {
        QLOG_ERROR() << "Feeds model: failed to select feeds data from database: " << query.lastError().text();
    }

    while(query.next()) {
        idsAndUnread[query.value(0).toInt()] = query.value(1).toInt();
    }

    for (int i = 0; i < rowCount(); ++i) {

        if (m_items.at(i)->type == 0) {
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

    queryAndSetTotalUnread();
}



void OcFeedsModelNew::itemsMarkedAllRead()
{
    if (m_items.isEmpty())
        return;

    QLOG_INFO() << "Feeds model: marking all items as read";

    for (int i = 0; i < rowCount(); ++i)
    {
        m_items.at(i)->unreadCount = 0;
    }

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    emit dataChanged(index(0), index(rowCount()-1), QVector<int>(1, UnreadCountRole));
#else
    emit dataChanged(index(0), index(rowCount()-1));
#endif
}



void OcFeedsModelNew::feedsRequested(const QList<int> &updated, const QList<int> &newFeeds, const QList<int> &deleted)
{
    if (updated.isEmpty() && newFeeds.isEmpty() && deleted.isEmpty())
        return;

    QSqlQuery query;

    if (!updated.isEmpty()) {

        QLOG_INFO() << "Feeds model: updating changed feeds";

        for (int i = 0; i < updated.size(); ++i)
        {
            int idx = findIndex(updated.at(i), 0);

            if (!query.exec(QString("SELECT title, iconSource, iconWidth, iconHeight, folderId, localUnreadCount, FROM feeds WHERE id = %1").arg(updated.at(i)))) {
                QLOG_ERROR() << "Feeds model: failed to select data for changed feeds from database: " << query.lastError().text();
            }

            query.next();

            int fId = query.value(4).toInt();

            // check if updated feed is still child of these folder
            if ((idx != -999) && (fId == folderId())) {

                QLOG_DEBUG() << "Feeds model: updating feed at index " << idx;

                m_items.at(i)->title = query.value(0).toString();
                m_items.at(i)->iconSource = query.value(1).toString();
                m_items.at(i)->iconWidth = query.value(2).toInt();
                m_items.at(i)->iconHeight = query.value(3).toInt();

                QVector<int> roles(1, TitleRole);
                roles.append(IconSourceRole);
                roles.append(IconWidthRole);
                roles.append(IconHeightRole);

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
                emit dataChanged(index(idx), index(idx), roles);
#else
                emit dataChanged(index(idx), index(idx));
#endif
            } else if ((idx != -999) && (fId != folderId())) {

                // the updated feed is no longer child of these folder

                QLOG_DEBUG() << "Feeds model: removing feed at index " << idx << " because it is no longer child of this folder";

                beginRemoveRows(QModelIndex(), idx, idx);

                delete m_items.takeAt(i);

                endRemoveRows();
            } else if ((idx == -999) && (fId == folderId())) {

                // the feed has been moved to this folder

                QLOG_DEBUG() << "Feeds model: adding feed that has been moved to this folder";

                beginInsertRows(QModelIndex(), rowCount(), rowCount());

                OcFeedObject *fobj = new OcFeedObject(updated.at(i),
                                                      0,
                                                      query.value(0).toString(),
                                                      query.value(5).toInt(),
                                                      query.value(1).toString(),
                                                      query.value(2).toInt(),
                                                      query.value(3).toInt());
                m_items.append(fobj);

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

        int length = 0;

        if (!query.exec(QString("SELECT COUNT(id) FROM feeds WHERE folderId = %1 AND id IN %2").arg(folderId()).arg(feedList))) {
            QLOG_ERROR() << "Feeds model: failed to select count of new feeds in this folder from database: " << query.lastError().text();
        }

        query.next();

        length = query.value(0).toInt();

        if (length > 0) {

            QLOG_INFO() << "Feeds model: adding new feeds";

            if (!query.exec(QString("SELECT id, title, localUnreadCount, iconSource, iconWidth, iconHeight WHERE folderId = %1 AND id IN %2").arg(folderId()).arg(feedList))) {
                QLOG_ERROR() << "Feeds model: failed to select data for newly added feeds from database: " << query.lastError().text();
            }

            beginInsertRows(QModelIndex(), rowCount(), (rowCount() + length - 1));

            while(query.next()) {

                OcFeedObject *fobj = new OcFeedObject(query.value(0).toInt(),
                                                      0,
                                                      query.value(1).toString(),
                                                      query.value(2).toInt(),
                                                      query.value(3).toString(),
                                                      query.value(4).toInt(),
                                                      query.value(5).toInt());
                m_items.append(fobj);
            }

            endInsertRows();

            endInsertRows();
        }
    }


    if (!deleted.isEmpty()) {

        for (int i = 0; i < deleted.size(); ++i) {

            int idx = findIndex(deleted.at(i), 0);

            if (idx != -999) {

                QLOG_INFO() << "Feeds model: removing deleted feed at index " << idx;

                beginRemoveRows(QModelIndex(), idx, idx);

                delete m_items.takeAt(idx);

                endRemoveRows();
            }
        }
    }

    queryAndSetTotalUnread();
}



void OcFeedsModelNew::feedDeleted(const int &id)
{
    int idx = findIndex(id, 0);

    if (idx != -999) {

        QLOG_INFO() << "Feeds model: removing deleted feed at index " << idx;

        beginRemoveRows(QModelIndex(), idx, idx);

        delete m_items.takeAt(idx);

        endRemoveRows();

        queryAndSetTotalUnread();
    }
}


void OcFeedsModelNew::feedCreated(const QString &name, const int &id)
{
    QSqlQuery query;

    if (!query.exec(QString("SELECT id, localUnreadCount, iconSource, iconWidth, iconHeight, folderId FROM feeds WHERE id = %1").arg(id))) {
        QLOG_ERROR() << "Feeds model: failed to select data of newly created feed from database: " << query.lastError().text();
    }

    query.next();

    if (query.value(5).toInt() == folderId()) {

        QLOG_INFO() << "Feeds model: adding newly created feed";

        beginInsertRows(QModelIndex(), rowCount(), rowCount());

        OcFeedObject *fobj = new OcFeedObject(query.value(0).toInt(),
                                              0,
                                              name,
                                              query.value(1).toInt(),
                                              query.value(2).toString(),
                                              query.value(3).toInt(),
                                              query.value(4).toInt());

        m_items.append(fobj);

        endInsertRows();

        queryAndSetTotalUnread();
    }
}


void OcFeedsModelNew::feedMoved(const int &feedId, const int &fId)
{
    int idx = findIndex(feedId, 0);

    if ((idx == -999) && (fId == folderId())) {

        // feed was added to this folder, so create it

        QLOG_INFO() << "Feeds model: add feed that was moved to this folder";

        QSqlQuery query;

        if (!query.exec(QString("SELECT id, title, localUnreadCount, iconSource, iconWidth, iconHeight, folderId FROM feeds WHERE id = %1").arg(feedId))) {
            QLOG_ERROR() << "Feeds model: failed to select feed data from database: " << query.lastError().text();
        }

        query.next();

        beginInsertRows(QModelIndex(), rowCount(), rowCount());

        OcFeedObject *fobj = new OcFeedObject(query.value(0).toInt(),
                                              0,
                                              query.value(1).toString(),
                                              query.value(2).toInt(),
                                              query.value(3).toString(),
                                              query.value(4).toInt(),
                                              query.value(5).toInt());
        m_items.append(fobj);

        endInsertRows();

        queryAndSetTotalUnread();

    } else if ((idx != -999) && (fId != folderId())) {

        // feed was removed from this folder, so delete it

        QLOG_INFO() << "Feeds model: remove feed at index " << idx << " that was moved to another folder";

        beginRemoveRows(QModelIndex(), idx, idx);

        delete m_items.takeAt(idx);

        endRemoveRows();

        queryAndSetTotalUnread();
    }
}


void OcFeedsModelNew::feedMarkedRead(const int &id)
{
    int idx = findIndex(id, 0);

    if (idx != -999) {

        QLOG_INFO() << "Combined model: feed at index " << idx << " was marked as read";

        m_items.at(idx)->unreadCount = 0;

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
        emit dataChanged(index(idx), index(idx), QVector<int>(1, UnreadCountRole));
#else
        emit dataChanged(index(idx), index(idx));
#endif

        queryAndSetTotalUnread();
    }
}



void OcFeedsModelNew::feedRenamed(const QString &newName, const int &feedId)
{
    if (m_items.isEmpty())
        return;

     int idx = findIndex(feedId, 0);

     if (idx != -999) {

        QLOG_INFO() << "Feeds model: feed at index " << idx  << " was renamed to " << newName;

        m_items.at(idx)->title = newName;

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
        emit dataChanged(index(idx), index(idx), QVector<int>(1, TitleRole));
#else
        emit dataChanged(index(idx), index(idx));
#endif

     }
}



void OcFeedsModelNew::folderMarkedRead(const int &id)
{
    if (id == folderId()) {

        QLOG_INFO() << "Feeds model: marking whole folder as read";

        for (int i = 0; i < rowCount(); ++i) {

            m_items.at(i)->unreadCount = 0;
        }

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
        emit dataChanged(index(0), index(rowCount()-1), QVector<int>(1, UnreadCountRole));
#else
        emit dataChanged(index(0), index(rowCount()-1));
#endif
    }
}



int OcFeedsModelNew::findIndex(const int &id, const int &type) const
{
    if (!m_items.isEmpty()) {
        for (int i = 0; i < rowCount(); ++i) {
            if ((m_items.at(i)->id == id) && (m_items.at(i)->type == type))
                return i;
        }
    }

    return -999;
}


void OcFeedsModelNew::queryAndSetTotalUnread()
{
    int idx = findIndex(folderId(), 1);

    if (idx == -999)
        return;

    QSqlQuery query;

    if (!query.exec(QString("SELECT localUnreadCount FROM folders WHERE id = %1").arg(folderId()))) {
        QLOG_ERROR() << "Feeds model: failed to select folder unread count from database: " << query.lastError().text();
    }

    query.next();

    m_items.at(idx)->unreadCount = query.value(0).toInt();

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    emit dataChanged(index(idx), index(idx), QVector<int>(1, UnreadCountRole));
#else
    emit dataChanged(index(idx), index(idx));
#endif

}
