#include "ocitemsmodelnew.h"
#include "objects/ocitemobject.h"

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
    m_feedId = "";
    m_items = QList<OcItemObject*>();
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
        return QVariant::fromValue(niceTime(iobj->pubDate));
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
#ifdef QT_DEBUG
    qDebug() << "Refresh Items Model";
#endif

    clear();

    QString queryString;
    QSqlQuery query;

    int length = 0;

    queryString = QString("SELECT COUNT(id) FROM items WHERE feedId = %1").arg(feedId().toInt());

    query.exec(queryString);

    query.next();

    length = query.value(0).toInt();

    query.clear();

    // handleRead 0: show read, 1: hide read, 2: show after unread
    queryString = QString("SELECT it.id, "
                                 "it.title, "
                                 "it.pubDate, "
                                 "it.enclosureLink, "
                                 "it.enclosureMime, "
                                 "it.unread, "
                                 "it.starred, "
                                 "it.url, "
                                 "it.guidHash, ");

    if (config.getSetting("display/excerpts", false).toBool()) {
        queryString.append("it.body AS excerpt, ");
    } else {
        queryString.append("'' AS excerpt, ");
    }

    if (config.getSetting("display/picturesInList", false).toBool()) {
        queryString.append("(SELECT DISTINCT path FROM images WHERE parentId = it.id AND height > 50 ORDER BY width, height LIMIT 1) AS image ");
    } else {
        queryString.append("'' AS image ");
    }

    queryString.append(QString("FROM items it WHERE feedId = %1").arg(feedId().toInt()));

    queryString.append(" ORDER BY pubDate DESC");

    query.exec(queryString);

    beginInsertRows(QModelIndex(), 0, length-1);

    while(query.next())
    {

        OcItemObject *iobj = new OcItemObject(query.value(0).toInt(),
                                              query.value(1).toString(),
                                              query.value(2).toUInt(),
                                              query.value(3).toString(),
                                              query.value(4).toString(),
                                              query.value(5).toBool(),
                                              query.value(6).toBool(),
                                              query.value(7).toString(),
                                              query.value(8).toString(),
                                              prepareBody(query.value(9).toString()),
                                              query.value(10).toString(),
                                              "",
                                              -1);
        m_items.append(iobj);
    }

    endInsertRows();

    query.clear();


//    if (handleRead == 1) queryString.append(" AND it.unread = ").append(SQL_TRUE);

//    if (!search.isEmpty())
//    {
//        QString t_search = search;
//        t_search.prepend("%");
//        t_search.append("%");
//        queryString.append(QString(" AND it.title LIKE \"%1\"").arg(t_search));
//    }

//    queryString.append(" ORDER BY");

//    if (handleRead == 2)
//        queryString.append(" unread DESC,");

//    if (sortAsc)
//    {
//        queryString.append(" pubDate ASC;");
//    } else {
//        queryString.append(" pubDate DESC;");
//    }
}


QString OcItemsModelNew::niceTime(const uint &t) const
{
    QString date = (QDateTime::currentDateTimeUtc().date() == QDateTime::fromTime_t(t).date()) ?
                                    tr("Today") :
                                    QDateTime::fromTime_t(t).toLocalTime().toString(tr("d. MMMM"));
    QString time = QDateTime::fromTime_t(t).toLocalTime().toString(tr("hh:mm"));

    return date + " | " + time;
}


QString OcItemsModelNew::prepareBody(const QString &b) const
{
    QString newBody = b;

#if defined(MEEGO_EDITION_HARMATTAN)
    return newBody.remove(QRegExp("<[^>]*>")).trimmed().left(200);
#else
    return newBody.remove(QRegularExpression("<[^>]*>")).trimmed().left(200);
#endif
}


QModelIndex OcItemsModelNew::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    return createIndex(row, column);
}



void OcItemsModelNew::clear()
{
    beginRemoveRows(QModelIndex(), 0, rowCount()-1);

    m_items.clear();

    endRemoveRows();
}



QString OcItemsModelNew::feedId() const
{
    return m_feedId;
}

void OcItemsModelNew::setFeedId(const QString &nFeedId)
{
    if (nFeedId != m_feedId) {
        m_feedId = nFeedId;
        emit feedIdChanged(feedId());
        init();
    }
}



void OcItemsModelNew::itemsMarked(const QStringList &ids, const QString &action)
{
    if (!ids.isEmpty()) {

        for (int i = 0; i < rowCount(); ++i) {
            if (ids.contains(QString::number(m_items.at(i)->itemId))) {
                OcItemObject *iobj = m_items.at(i);

                if (action == "read") {
                    iobj->unread = false;
                } else {
                    iobj->unread = true;
                }

                m_items[i] = iobj;

                emit dataChanged(index(i, 0), index(i, 0));
            }
        }
    }
}



void OcItemsModelNew::itemsStarred(const QStringList &hashes, const QString &action)
{
    if (!hashes.isEmpty()) {

        for (int i = 0; i < rowCount(); ++i) {
            if (hashes.contains(m_items.at(i)->guidHash)) {
                OcItemObject *iobj = m_items.at(i);

                if (action == "star") {
                    iobj->starred = true;
                } else {
                    iobj->starred = false;
                }

                m_items[i] = iobj;

                emit dataChanged(index(i, 0), index(i, 0));
            }
        }
    }
}



void OcItemsModelNew::feedMarkedRead(const QString &markedFeedId)
{
    if (markedFeedId == feedId()) {

        for (int i = 0; i < rowCount(); ++i) {

            OcItemObject *iobj = m_items.at(i);

            iobj->unread = false;

            m_items[i] = iobj;

        }

        emit dataChanged(index(0, 0), index(rowCount()-1, 0));
    }
}


void OcItemsModelNew::itemsUpdated(const QList<int> &updated, const QList<int> &newItems, const QList<int> &deleted)
{
    if (m_items.isEmpty())
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

        qDebug() << itemList;

        int length = 0;

        queryString = QString("SELECT COUNT(id) FROM items WHERE feedId = %1 AND id IN %2").arg(feedId().toInt()).arg(itemList);

        q.exec(queryString);

        q.next();

        length = q.value(0).toInt();

        q.clear();

        if (length > 0) {

            queryString = QString("SELECT it.id, "
                                         "it.title, "
                                         "it.pubDate, "
                                         "it.enclosureLink, "
                                         "it.enclosureMime, "
                                         "it.unread, "
                                         "it.starred, "
                                         "it.url, "
                                         "it.guidHash, ");

            if (config.getSetting("display/excerpts", false).toBool()) {
                queryString.append("it.body AS excerpt, ");
            } else {
                queryString.append("'' AS excerpt, ");
            }

            if (config.getSetting("display/picturesInList", false).toBool()) {
                queryString.append("(SELECT DISTINCT path FROM images WHERE parentId = it.id AND height > 50 ORDER BY width, height LIMIT 1) AS image ");
            } else {
                queryString.append("'' AS image ");
            }

            queryString.append(QString("FROM items it WHERE feedId = %1 AND id IN %2").arg(feedId().toInt()).arg(itemList));

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
                                                      prepareBody(q.value(9).toString()),
                                                      q.value(10).toString(),
                                                      "",
                                                      -1);
                m_items.prepend(iobj);

                qDebug() << "Adding item: " << iobj->itemId;
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

                qDebug() << "Update item: " << iobj->itemId;

                emit dataChanged(index(i, 0), index(i, 0));
            }
        }
    }



    if (!deleted.isEmpty())
    {
        QList<int> deleteIdxs;

        for (int i = 0; i < rowCount(); ++i)
        {
            if (updated.contains(m_items.at(i)->itemId))
                deleteIdxs << i;
        }

        for (int ii = 0; ii < deleteIdxs.size(); ++ii)
        {
            beginRemoveRows(QModelIndex(), ii, ii);

            qDebug() << "Remove item row: " << ii;

            m_items.removeAt(ii);

            endRemoveRows();
        }
    }
}
