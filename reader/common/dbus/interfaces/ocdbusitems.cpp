#include "ocdbusitems.h"

OcDBusItems::OcDBusItems(QObject *parent) :
    QObject(parent)
{
    items = new de::buschmann23::ocNewsEngine::Items("de.buschmann23.ocNewsEngine", "/Items", QDBusConnection::sessionBus(), this);
    connect(items, SIGNAL(markedAllItemsReadError(QString)), this, SLOT(dbusMarkedAllItemsReadError(QString)));
    connect(items, SIGNAL(markedAllItemsReadSuccess()), this, SLOT(dbusMarkedAllItemsReadSuccess()));
    connect(items, SIGNAL(markedItemsError(QString)), this, SLOT(dbusMarkedItemsError(QString)));
    connect(items, SIGNAL(markedItemsSuccess(QStringList, QString)), this, SLOT(dbusMarkedItemsSuccess(QStringList, QString)));
    connect(items, SIGNAL(requestedItemsError(QString)), this, SLOT(dbusRequestedItemsError(QString)));
    connect(items, SIGNAL(requestedItemsSuccess(QList<int>, QList<int>, QList<int>)), this, SLOT(dbusRequestedItemsSuccess(QList<int>, QList<int>, QList<int>)));
    connect(items, SIGNAL(starredItemsError(QString)), this, SLOT(dbusStarredItemsError(QString)));
    connect(items, SIGNAL(starredItemsSuccess(QStringList, QString)), this, SLOT(dbusStarredItemsSuccess(QStringList, QString)));
    connect(items, SIGNAL(updatedItemsError(QString)), this, SLOT(dbusUpdatedItemsError(QString)));
    connect(items, SIGNAL(updatedItemsSuccess(QList<int>, QList<int>, QList<int>)), this, SLOT(dbusUpdatedItemsSuccess(QList<int>, QList<int>, QList<int>)));
}

void OcDBusItems::markAllItemsRead()
{
    items->markAllItemsRead();
}

void OcDBusItems::markItems(const QString &action, const QVariantList &ids)
{
    items->markItems(action, ids);
}

void OcDBusItems::markItemsTillThis(const QString &action, const QVariant &pubDate, const QVariant &feedId)
{
    QDBusVariant t_pubDate(pubDate);
    QDBusVariant t_feedId(feedId);
    items->markItemsTillThis(action, t_pubDate, t_feedId);
}

void OcDBusItems::requestItems(const QString &batchSize, const QString &offset, const QString &type, const QString &id, const QString &getRead)
{
    items->requestItems(batchSize, offset, type, id, getRead);
}

void OcDBusItems::starItems(const QString &action, const QVariantMap &itemIds)
{
    items->starItems(action, itemIds);
}

void OcDBusItems::updateItems(const QString &lastModified, const QString &type, const QString &id)
{
    items->updateItems(lastModified, type, id);
}

void OcDBusItems::dbusMarkedAllItemsReadError(const QString &markedAllItemsReadErrorString)
{
    emit markedAllItemsReadError(markedAllItemsReadErrorString);
}

void OcDBusItems::dbusMarkedAllItemsReadSuccess()
{
    emit markedAllItemsReadSuccess();
}

void OcDBusItems::dbusMarkedItemsError(const QString &markedItemsErrorString)
{
    emit markedItemsError(markedItemsErrorString);
}

void OcDBusItems::dbusMarkedItemsSuccess(const QStringList &ids, const QString &action)
{
    emit markedItemsSuccess(ids, action);
}

void OcDBusItems::dbusRequestedItemsError(const QString &requestedItemsErrorString)
{
    emit requestedItemsError(requestedItemsErrorString);
}

void OcDBusItems::dbusRequestedItemsSuccess(const QList<int> &updated, const QList<int> &newItems, const QList<int> &deleted)
{
    emit requestedItemsSuccess(updated, newItems, deleted);
}

void OcDBusItems::dbusStarredItemsError(const QString &starredItemsErrorString)
{
    emit starredItemsError(starredItemsErrorString);
}

void OcDBusItems::dbusStarredItemsSuccess(const QStringList &hashes, const QString &action)
{
    emit starredItemsSuccess(hashes, action);
}

void OcDBusItems::dbusUpdatedItemsError(const QString &updateItemsErrorString)
{
    emit updatedItemsError(updateItemsErrorString);
}

void OcDBusItems::dbusUpdatedItemsSuccess(const QList<int> &updated, const QList<int> &newItems, const QList<int> &deleted)
{
    emit updatedItemsSuccess(updated, newItems, deleted);
}
