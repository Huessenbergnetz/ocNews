#include "ocitemsmodelnew.h"
#include "QsLog.h"

const int OcItemsModelNew::TitleRole = Qt::UserRole + 1;
const int OcItemsModelNew::ItemIdRole = Qt::UserRole + 2;
const int OcItemsModelNew::PubDateRole = Qt::UserRole + 3;
const int OcItemsModelNew::PubDateIntRole = Qt::UserRole + 4;
const int OcItemsModelNew::EnclosureLinkRole = Qt::UserRole + 5;
const int OcItemsModelNew::EnclosureMimeRole = Qt::UserRole + 6;
const int OcItemsModelNew::UnreadRole = Qt::UserRole + 7;
const int OcItemsModelNew::StarredRole = Qt::UserRole + 8;
const int OcItemsModelNew::UrlRole = Qt::UserRole + 9;
const int OcItemsModelNew::GuidHashRole = Qt::UserRole + 10;
const int OcItemsModelNew::ExcerptRole = Qt::UserRole + 11;
const int OcItemsModelNew::ImageRole = Qt::UserRole + 12;

OcItemsModelNew::OcItemsModelNew(QObject *parent) :
    QAbstractListModel(parent)
{
    m_feedId = -999;
    m_items = QList<OcItemObject*>();
    m_populating = false;
    m_showExcerpts = false;
    m_showImages = false;

    timer = new QTimer(this);
    timer->setSingleShot(true);
    connect(timer, SIGNAL(timeout()), this, SLOT(clearByTimer()));

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    setRoleNames(roleNames());
#endif
}


QHash<int, QByteArray> OcItemsModelNew::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();
    roles.insert(TitleRole, QByteArray("title"));
    roles.insert(ItemIdRole, QByteArray("itemId"));
    roles.insert(PubDateRole, QByteArray("pubDate"));
    roles.insert(PubDateIntRole, QByteArray("pubDateInt"));
    roles.insert(EnclosureLinkRole, QByteArray("enclosureLink"));
    roles.insert(EnclosureMimeRole, QByteArray("enclosureMime"));
    roles.insert(UnreadRole, QByteArray("unread"));
    roles.insert(StarredRole, QByteArray("starred"));
    roles.insert(UrlRole, QByteArray("url"));
    roles.insert(GuidHashRole, QByteArray("guidHash"));
    roles.insert(ExcerptRole, QByteArray("excerpt"));
    roles.insert(ImageRole, QByteArray("image"));
    return roles;
}


int OcItemsModelNew::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_items.size();
}


QVariant OcItemsModelNew::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() > (m_items.size()-1))
        return QVariant();

    OcItemObject *iobj = m_items.at(index.row());

    switch(role) {
    case TitleRole:
        return QVariant::fromValue(iobj->title);
    case ItemIdRole:
        return QVariant::fromValue(iobj->itemId);
    case PubDateRole:
        return QVariant::fromValue(helper.niceTime(iobj->pubDate));
    case PubDateIntRole:
        return QVariant::fromValue(iobj->pubDate);
    case EnclosureLinkRole:
        return QVariant::fromValue(iobj->enclosureLink);
    case EnclosureMimeRole:
        return QVariant::fromValue(iobj->enclosureMime);
    case UnreadRole:
        return QVariant::fromValue(iobj->unread);
    case StarredRole:
        return QVariant::fromValue(iobj->starred);
    case UrlRole:
        return QVariant::fromValue(iobj->url);
    case GuidHashRole:
        return QVariant::fromValue(iobj->guidHash);
    case ExcerptRole:
        return QVariant::fromValue(iobj->excerpt);
    case ImageRole:
        return QVariant::fromValue(iobj->image);
    default:
        return QVariant();

    }
}


void OcItemsModelNew::init()
{
    QLOG_INFO() << "Initialize items model";

    setPopulating(true);

    if (!m_items.isEmpty())
        clear();

    QString queryString;
    QSqlQuery query;

    int length = 0;

    queryString = QString("SELECT COUNT(id) FROM items WHERE feedId = %1").arg(feedId());

    if (!query.exec(queryString)) {
        QLOG_ERROR() << "Items model: failed to count database items in this feed: " << query.lastError().text();
    }

    query.next();

    length = query.value(0).toInt();

    if (length > 0) {

        query.clear();

        queryString = QString("SELECT it.id, "
                                     "it.title, "
                                     "it.pubDate, "
                                     "it.enclosureLink, "
                                     "it.enclosureMime, "
                                     "it.unread, "
                                     "it.starred, "
                                     "it.url, "
                                     "it.guidHash, ");

        if (showExcerpts()) {
            queryString.append("it.body AS excerpt, ");
        } else {
            queryString.append("'' AS excerpt, ");
        }

        if (showImages()) {
            queryString.append("(SELECT DISTINCT path FROM images WHERE parentId = it.id AND height > 50 ORDER BY width, height LIMIT 1) AS image ");
        } else {
            queryString.append("'' AS image ");
        }

        queryString.append(QString("FROM items it WHERE feedId = %1").arg(feedId()));

        queryString.append(" ORDER BY pubDate DESC");

        if (!query.exec(queryString)) {
            QLOG_ERROR() << "Items model: failed to query items from database: " << query.lastError().text();
        }


        beginInsertRows(QModelIndex(), 0, length-1);

        OcItemsQuery *queryThread = new OcItemsQuery(this);
        connect(queryThread, SIGNAL(gotRecord(OcItemObject*)), this, SLOT(gotItem(OcItemObject*)));
        connect(queryThread, SIGNAL(finished()), this, SLOT(queryFinished()));
        connect(queryThread, SIGNAL(finished()), queryThread, SLOT(deleteLater()));
        queryThread->startQuery(queryString, false);

    } else  {
        setPopulating(false);
    }
}


void OcItemsModelNew::gotItem(OcItemObject *item)
{
    m_items.append(item);
}


void OcItemsModelNew::queryFinished()
{
    setPopulating(false);
    endInsertRows();
}


QModelIndex OcItemsModelNew::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    return createIndex(row, column);
}



void OcItemsModelNew::clear()
{
    QLOG_INFO() << "Clearing items model";

    beginRemoveRows(QModelIndex(), 0, rowCount()-1);

    while (!m_items.isEmpty()) {
        delete m_items.takeLast();
    }

    m_items.clear();

    endRemoveRows();
}



int OcItemsModelNew::feedId() const
{
    return m_feedId;
}

void OcItemsModelNew::setFeedId(const int &nFeedId)
{
    timer->stop();
    if (nFeedId != m_feedId) {
        m_feedId = nFeedId;
        QLOG_DEBUG() << "Items model: changing feed ID to " << feedId();
        init();
        emit feedIdChanged(feedId());
    }
}



bool OcItemsModelNew::populating() const { return m_populating; }

void OcItemsModelNew::setPopulating(const bool &nPopulating)
{
    if (nPopulating != m_populating) {
        m_populating = nPopulating;
        QLOG_DEBUG() << "Items model: changing populating to " << populating();
        emit populatingChanged(populating());
    }
}



bool OcItemsModelNew::showExcerpts() const { return m_showExcerpts; }

void OcItemsModelNew::setShowExcerpts(const bool &nShowExcerpts)
{
    if (nShowExcerpts != m_showExcerpts) {
        m_showExcerpts = nShowExcerpts;
        QLOG_DEBUG() << "Items model: changing showExcerpts to " << showExcerpts();
        emit showExcerptsChanged(showExcerpts());
    }
}


bool OcItemsModelNew::showImages() const { return m_showImages; }

void OcItemsModelNew::setShowImages(const bool &nShowImages)
{
    if (nShowImages != m_showImages) {
        m_showImages = nShowImages;
        QLOG_DEBUG() << "Items model: changing showImages to " << showImages();
        emit showImagesChanged(showImages());
    }
}



void OcItemsModelNew::itemsMarked(const QStringList &ids, const QString &action)
{
    if (!ids.isEmpty() && !m_populating) {

        QLOG_INFO() << "Items model: updating un/marked items";

        for (int i = 0; i < rowCount(); ++i) {
            if (ids.contains(QString::number(m_items.at(i)->itemId))) {

                m_items.at(i)->unread = (action == "read") ? false : true;

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
                emit dataChanged(index(i), index(i), QVector<int>(1, UnreadRole));
#else
                emit dataChanged(index(i), index(i));
#endif
            }
        }
    }
}



void OcItemsModelNew::itemsStarred(const QStringList &hashes, const QString &action)
{
    if (!hashes.isEmpty() && !m_populating) {

        QLOG_INFO() << "Items model: updating un/starred items";

        for (int i = 0; i < rowCount(); ++i) {
            if (hashes.contains(m_items.at(i)->guidHash)) {

                m_items.at(i)->starred = (action == "star") ? true : false;

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
                emit dataChanged(index(i), index(i), QVector<int>(1, StarredRole));
#else
                emit dataChanged(index(i), index(i));
#endif
            }
        }
    }
}



void OcItemsModelNew::feedMarkedRead(const int &markedFeedId)
{
    if (markedFeedId == feedId() && !m_populating) {

        QLOG_INFO() << "Items model: feed was marked as read, updating items";

        for (int i = 0; i < rowCount(); ++i) {

            m_items.at(i)->unread = false;

        }
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
        emit dataChanged(index(0), index(rowCount()-1), QVector<int>(1, UnreadRole));
#else
        emit dataChanged(index(0), index(rowCount()-1));
#endif
    }
}


void OcItemsModelNew::itemsUpdated(const QList<int> &updated, const QList<int> &newItems, const QList<int> &deleted)
{
    if (m_items.isEmpty() || m_populating)
        return;

    QSqlQuery q;

    QString queryString;

    if (!newItems.isEmpty()) {

        QString itemList("(");

        for (int i = 0; i < newItems.size(); ++i)
        {
            itemList.append(QString::number(newItems.at(i)));
            itemList.append(", ");
        }

        itemList.chop(2);
        itemList.append(")");

        int length = 0;

        queryString = QString("SELECT COUNT(id) FROM items WHERE feedId = %1 AND id IN %2").arg(feedId()).arg(itemList);

        if (!q.exec(queryString)) {
            QLOG_ERROR() << "Items model: failed to count newly added items from this feed in database: " << q.lastError().text();
        }

        q.next();

        length = q.value(0).toInt();

        q.clear();

        if (length > 0) {

            QLOG_INFO() << "Items model: Adding new items";

            queryString = QString("SELECT it.id, "
                                         "it.title, "
                                         "it.pubDate, "
                                         "it.enclosureLink, "
                                         "it.enclosureMime, "
                                         "it.unread, "
                                         "it.starred, "
                                         "it.url, "
                                         "it.guidHash, ");

            if (showExcerpts()) {
                queryString.append("it.body AS excerpt, ");
            } else {
                queryString.append("'' AS excerpt, ");
            }

            if (showImages()) {
                queryString.append("(SELECT DISTINCT path FROM images WHERE parentId = it.id AND height > 50 ORDER BY width, height LIMIT 1) AS image ");
            } else {
                queryString.append("'' AS image ");
            }

            queryString.append(QString("FROM items it WHERE feedId = %1 AND id IN %2").arg(feedId()).arg(itemList));

            queryString.append(" ORDER BY pubDate ASC");

            q.exec(queryString);

            beginInsertRows(QModelIndex(), 0, length-1);

            while(q.next())
            {

                OcItemObject *iobj = new OcItemObject(q.value(0).toInt(),
                                                      q.value(1).toString(),
                                                      q.value(2).toUInt(),
                                                      q.value(3).toString(),
                                                      q.value(4).toString(),
                                                      q.value(5).toBool(),
                                                      q.value(6).toBool(),
                                                      q.value(7).toString(),
                                                      q.value(8).toString(),
                                                      helper.prepareBody(q.value(9).toString()),
                                                      q.value(10).toString(),
                                                      "",
                                                      -1);
                m_items.prepend(iobj);

            }

            endInsertRows();
        }
    }



    if (!updated.isEmpty())
    {
        QLOG_INFO() << "Items model: updating items";

        for (int i = 0; i < rowCount(); ++i) {

            if (updated.contains(m_items.at(i)->itemId)) {

                OcItemObject *iobj = m_items.at(i);

                queryString = QString("SELECT it.title, "
                                             "it.pubDate, "
                                             "it.enclosureLink, "
                                             "it.enclosureMime, "
                                             "it.unread, "
                                             "it.starred, "
                                             "it.url, "
                                             "it.guidHash "
                                      "FROM items it WHERE id = %1").arg(iobj->itemId);

                q.exec(queryString);

                q.next();

                iobj->title = q.value(0).toString();
                iobj->pubDate = q.value(1).toInt();
                iobj->enclosureLink = q.value(2).toString();
                iobj->enclosureMime = q.value(3).toString();
                iobj->unread = q.value(4).toBool();
                iobj->starred = q.value(5).toBool();
                iobj->url = q.value(6).toString();
                iobj->guidHash = q.value(7).toString();

                emit dataChanged(index(i, 0), index(i, 0));
            }
        }
    }



    if (!deleted.isEmpty())
    {
        QLOG_INFO() << "Items model: remove deleted items";

        QList<int> deleteIdxs;

        for (int i = 0; i < rowCount(); ++i)
        {
            if (deleted.contains(m_items.at(i)->itemId))
                deleteIdxs << i;
        }

        for (int ii = 0; ii < deleteIdxs.size(); ++ii)
        {
            int idx = deleteIdxs.at(ii);

            beginRemoveRows(QModelIndex(), idx, idx);

            m_items.removeAt(idx);

            endRemoveRows();
        }
    }
}



void OcItemsModelNew::allMarkedRead()
{
    if (!m_items.isEmpty() && !m_populating)
    {
        QLOG_INFO() << "Items model: marking all items as read";

        for (int i = 0; i < rowCount(); ++i)
        {
            OcItemObject *iobj = m_items.at(i);

            if (iobj->unread) {
                iobj->unread = false;
                m_items[i] = iobj;

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
                emit dataChanged(index(i), index(i), QVector<int>(1, UnreadRole));
#else
                emit dataChanged(index(i), index(i));
#endif
            }
        }
    }
}



void OcItemsModelNew::startCleanUpTimer()
{
    timer->start(10000);
}


void OcItemsModelNew::clearByTimer()
{
    clear();

    m_feedId = -999;
}
