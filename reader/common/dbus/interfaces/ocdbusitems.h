#ifndef OCDBUSITEMS_H
#define OCDBUSITEMS_H

#include <QObject>

#include "ocitemsif.h"

class OcDBusItems : public QObject
{
    Q_OBJECT
public:
    explicit OcDBusItems(QObject *parent = 0);

    enum Action { MarkRead, MarkUnread, Star, Unstar };

public slots:
    void markAllItemsRead();
    void markItems(const QString &action, const QVariantList &ids);
    void markItemsTillThis(const QString &action, const QVariant &pubDate, const QVariant &feedId);
    void requestItems(const QString &batchSize, const QString &offset, const QString &type, const QString &id, const QString &getRead); // default: "1000", "0", "3", "0", "true"
    void starItems(const QString &action, const QVariantMap &itemIds);
    void updateItems(const QString &lastModified, const QString &type, const QString &id); // default: "0", "3", "0"

signals:
    void markedAllItemsReadError(const QString &markedAllItemsReadErrorString);
    void markedAllItemsReadSuccess();
    void markedItemsError(const QString &markedItemsErrorString);
    void markedItemsSuccess(const QStringList &ids, const QString &action);
    void requestedItemsError(const QString &requestedItemsErrorString);
    void requestedItemsSuccess(const QList<int> &updated, const QList<int> &newItems, const QList<int> &deleted);
    void starredItemsError(const QString &starredItemsErrorString);
    void starredItemsSuccess(const QStringList &hashes, const QString &action);
    void updatedItemsError(const QString &updateItemsErrorString);
    void updatedItemsSuccess(const QList<int> &updated, const QList<int> &newItems, const QList<int> &deleted);

private slots:
    void dbusMarkedAllItemsReadError(const QString &markedAllItemsReadErrorString);
    void dbusMarkedAllItemsReadSuccess();
    void dbusMarkedItemsError(const QString &markedItemsErrorString);
    void dbusMarkedItemsSuccess(const QStringList &ids, const QString &action);
    void dbusRequestedItemsError(const QString &requestedItemsErrorString);
    void dbusRequestedItemsSuccess(const QList<int> &updated, const QList<int> &newItems, const QList<int> &deleted);
    void dbusStarredItemsError(const QString &starredItemsErrorString);
    void dbusStarredItemsSuccess(const QStringList &hashes, const QString &action);
    void dbusUpdatedItemsError(const QString &updateItemsErrorString);
    void dbusUpdatedItemsSuccess(const QList<int> &updated, const QList<int> &newItems, const QList<int> &deleted);

private:
    de::buschmann23::ocNewsEngine::Items *items;

};

#endif // OCDBUSFOLDERS_H
