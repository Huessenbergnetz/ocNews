#include "ocspecialitemsmodelnew.h"

#include <QDebug>

const int OcSpecialItemsModelNew::TitleRole = Qt::UserRole + 1;
const int OcSpecialItemsModelNew::ItemIdRole = Qt::UserRole + 2;
const int OcSpecialItemsModelNew::PubDateRole = Qt::UserRole + 3;
const int OcSpecialItemsModelNew::PubDateIntRole = Qt::UserRole + 4;
const int OcSpecialItemsModelNew::EnclosureLinkRole = Qt::UserRole + 5;
const int OcSpecialItemsModelNew::EnclosureMimeRole = Qt::UserRole + 6;
const int OcSpecialItemsModelNew::UnreadRole = Qt::UserRole + 7;
const int OcSpecialItemsModelNew::StarredRole = Qt::UserRole + 8;
const int OcSpecialItemsModelNew::UrlRole = Qt::UserRole + 9;
const int OcSpecialItemsModelNew::GuidHashRole = Qt::UserRole + 10;
const int OcSpecialItemsModelNew::ExcerptRole = Qt::UserRole + 11;
const int OcSpecialItemsModelNew::ImageRole = Qt::UserRole + 12;
const int OcSpecialItemsModelNew::FeedNameRole = Qt::UserRole + 13;
const int OcSpecialItemsModelNew::FeedIdRole = Qt::UserRole + 14;

OcSpecialItemsModelNew::OcSpecialItemsModelNew(QObject *parent) :
    QAbstractListModel(parent)
{
    m_type = -99;
    m_id = -99;
    m_showExcerpts = false;
    m_showImages = false;
    m_populating = false;
    m_items = QList<OcItemObject*>();

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    setRoleNames(roleNames());
#endif
}


int OcSpecialItemsModelNew::type() const { return m_type; }

void OcSpecialItemsModelNew::setType(const int &nType)
{
    if (nType != m_type) {
        m_type = nType;
        emit typeChanged(type());
    }
}


int OcSpecialItemsModelNew::id() const { return m_id; }

void OcSpecialItemsModelNew::setId(const int &nId)
{
    if (nId != m_id) {
        m_id = nId;
        init();
        emit idChanged(id());
    }
}


bool OcSpecialItemsModelNew::showExcerpts() const { return m_showExcerpts; }

void OcSpecialItemsModelNew::setShowExcerpts(const bool &nShowExcerpts)
{
    if (nShowExcerpts != m_showExcerpts) {
        m_showExcerpts = nShowExcerpts;
        emit showExcerptsChanged(showExcerpts());
    }
}


bool OcSpecialItemsModelNew::showImages() const { return m_showImages; }

void OcSpecialItemsModelNew::setShowImages(const bool &nShowImages)
{
    if (nShowImages != m_showImages) {
        m_showImages = nShowImages;
        emit showImagesChanged(showImages());
    }
}


bool OcSpecialItemsModelNew::populating() const { return m_populating; }

void OcSpecialItemsModelNew::setPopulating(const bool &nPopulating)
{
    if (nPopulating != m_populating) {
        m_populating = nPopulating;
        emit populatingChanged(populating());
    }
}


QHash<int, QByteArray> OcSpecialItemsModelNew::roleNames() const
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
    roles.insert(FeedNameRole, QByteArray("feedName"));
    roles.insert(FeedIdRole, QByteArray("feedId"));
    return roles;
}


int OcSpecialItemsModelNew::rowCount(const QModelIndex &parent) const
{
    return m_items.size();
}


QVariant OcSpecialItemsModelNew::data(const QModelIndex &index, int role) const
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
    case FeedNameRole:
        return QVariant::fromValue(iobj->feedName);
    case FeedIdRole:
        return QVariant::fromValue(iobj->feedId);
    default:
        return QVariant();

    }
}


QModelIndex OcSpecialItemsModelNew::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    return createIndex(row, column);
}




void OcSpecialItemsModelNew::init()
{
    // type: 0: Feed, 1: Folder, 2: Starred, 3: All

#ifdef QT_DEBUG
    qDebug() << "Init special items model";
#endif

    setPopulating(true);

    if (!m_items.isEmpty())
        clear();

    QString querystring;
    QSqlQuery query;

    int length = 0;

    switch (type()) {
    case 1:
        querystring = QString("SELECT count (id) FROM items WHERE feedId IN (SELECT id FROM feeds WHERE folderId = %1)").arg(id());
        break;
    case 2:
        querystring = QString("SELECT count (id) FROM items WHERE starred = %1").arg(SQL_TRUE);
        break;
    case 3:
    default:
        querystring = "SELECT count (id) FROM items";
        break;
    }

    query.exec(querystring);

    query.next();

    length = query.value(0).toInt();

    if (length > 0) {

        querystring = "SELECT it.id, it.title, it.pubDate, it.enclosureLink, it.enclosureMime, it.unread, it.starred, it.url, it.guidHash, ";

        if (showExcerpts()) {
            querystring.append("it.body AS excerpt, ");
        } else {
            querystring.append("'' AS excerpt, ");
        }

        if (showImages()) {
            querystring.append("(SELECT DISTINCT path FROM images WHERE parentId = it.id AND height > 50 ORDER BY width, height LIMIT 1) AS image, ");
        } else {
            querystring.append("'' AS image, ");
        }

         querystring.append("(SELECT title FROM feeds where id = it.feedId) as feedName, it.feedId ");


        switch (type()) {
        case 1:
            querystring.append(QString("FROM items it WHERE feedId IN (SELECT id FROM feeds WHERE folderId = %1)").arg(id()));
            break;
        case 2:
            querystring.append("FROM items it WHERE starred = ").append(SQL_TRUE);
            break;
        case 3:
        default:
            querystring.append("FROM items it");
            break;
        }

        querystring.append(" ORDER BY pubDate DESC");

        beginInsertRows(QModelIndex(), 0, length-1);

        OcItemsQuery *queryThread = new OcItemsQuery(this);
        connect(queryThread, SIGNAL(gotRecord(OcItemObject*)), this, SLOT(gotItem(OcItemObject*)));
        connect(queryThread, SIGNAL(finished()), this, SLOT(queryFinished()));
        connect(queryThread, SIGNAL(finished()), queryThread, SLOT(deleteLater()));
        queryThread->startQuery(querystring, true);

    } else {
        setPopulating(false);
    }
}



void OcSpecialItemsModelNew::gotItem(OcItemObject *item)
{
    m_items.append(item);
}


void OcSpecialItemsModelNew::queryFinished()
{
    setPopulating(false);
    endInsertRows();
}


void OcSpecialItemsModelNew::clear()
{
    beginRemoveRows(QModelIndex(), 0, rowCount()-1);

    while (!m_items.isEmpty()) {
        delete m_items.takeLast();
    }

    m_items.clear();

    endRemoveRows();

    m_type = -99;
    m_id = -99;
}



void OcSpecialItemsModelNew::itemsMarked(const QStringList &ids, const QString &action)
{
    if (!ids.isEmpty() && !m_items.isEmpty() && !m_populating)
    {
        for (int i = 0; i < rowCount(); ++i)
        {
            if (ids.contains(QString::number(m_items.at(i)->itemId)))
            {
                OcItemObject *iobj = m_items.at(i);

                if (action == "read") {
                    iobj->unread = false;
                } else {
                    iobj->unread = true;
                }

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



void OcSpecialItemsModelNew::itemsStarred(const QStringList &hashes, const QString &action)
{
    if (!hashes.isEmpty() && !m_items.isEmpty() && !m_populating) {

        for (int i = 0; i < rowCount(); ++i) {
            if (hashes.contains(m_items.at(i)->guidHash)) {
                OcItemObject *iobj = m_items.at(i);

                if (action == "star") {
                    iobj->starred = true;
                } else {
                    iobj->starred = false;
                }

                m_items[i] = iobj;

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
                emit dataChanged(index(i), index(i), QVector<int>(1, StarredRole));
#else
                emit dataChanged(index(i), index(i));
#endif
            }
        }
    }
}


void OcSpecialItemsModelNew::folderMarkedRead(const int &markedFolderId)
{
    if (type() == 1 && id() == markedFolderId && !m_items.isEmpty() && !m_populating)
    {
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


void OcSpecialItemsModelNew::allMarkedRead()
{
    if (!m_items.isEmpty() && !m_populating)
    {
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


void OcSpecialItemsModelNew::itemsUpdated(const QList<int> &updated, const QList<int> &newItems, const QList<int> &deleted)
{
    if (m_items.isEmpty() || m_populating)
        return;

    QSqlQuery q;

    QString queryString;

    if (!newItems.isEmpty())
    {
        QString itemList("(");

        for (int i = 0; i < newItems.size(); ++i)
        {
            itemList.append(QString::number(newItems.at(i)));
            itemList.append(", ");
        }

        itemList.chop(2);
        itemList.append(")");

        int length = 0;

        switch (type())
        {
        case 1:
            queryString = QString("SELECT COUNT(id) FROM items WHERE feedId IN (SELECT id FROM feeds WHERE folderId = %1) AND id IN %2").arg(id()).arg(itemList);
            break;
        case 2:
            queryString = QString("SELECT COUNT(id) FROM items WHERE starred = %1 AND id IN %2").arg(SQL_TRUE).arg(itemList);
            break;
        case 3:
        default:
            queryString = QString("SELECT COUNT (id) FROM items WHERE id IN %2").arg(itemList);
            break;
        }

        q.exec(queryString);

        q.next();

        length = q.value(0).toInt();

        q.clear();

        if (length > 0) {

            queryString = "SELECT it.id, it.title, it.pubDate, it.enclosureLink, it.enclosureMime, it.unread, it.starred, it.url, it.guidHash, ";

            if (showExcerpts()) {
                queryString.append("it.body AS excerpt, ");
            } else {
                queryString.append("'' AS excerpt, ");
            }

            if (showImages()) {
                queryString.append("(SELECT DISTINCT path FROM images WHERE parentId = it.id AND height > 50 ORDER BY width, height LIMIT 1) AS image ");
            } else {
                queryString.append("'' AS image, ");
            }

             queryString.append("(SELECT title FROM feeds where id = it.feedId) as feedName, it.feedId ");


            switch (type()) {
            case 1:
                queryString.append(QString("FROM items it WHERE feedId IN (SELECT id FROM feeds WHERE folderId = %1) AND id IN %2").arg(id()).arg(itemList));
                break;
            case 2:
                queryString.append(QString("FROM items it WHERE starred = %1 AND id IN %2").arg(SQL_TRUE).arg(itemList));
                break;
            case 3:
            default:
                queryString.append(QString("FROM items it WHERE id IN %1").arg(itemList));
                break;
            }

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
                                                      q.value(11).toString(),
                                                      q.value(12).toInt());
                m_items.prepend(iobj);
            }

            endInsertRows();

            q.clear();
        }
    }


    if (!updated.isEmpty())
    {
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
                                             "it.guidHash ");

                queryString.append(QString("FROM items it WHERE id = %1").arg(m_items.at(i)->itemId));

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

                m_items[i] = iobj;

                emit dataChanged(index(i, 0), index(i, 0));
            }
        }

        q.clear();
    }


    if (!deleted.isEmpty())
    {
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

            delete m_items.takeAt(idx);

            endRemoveRows();
        }
    }
}
