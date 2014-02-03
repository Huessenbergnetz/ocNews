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
    int isFetchImagesRunning();

signals:
    void markedAllItemsReadError(const QString &markedAllItemsReadErrorString);
    void markedAllItemsReadSuccess();
    void markedItemsError(const QString &markedItemsErrorString);
    void markedItemsSuccess();
    void requestedItemsError(const QString &requestedItemsErrorString);
    void requestedItemsSuccess();
    void starredItemsError(const QString &starredItemsErrorString);
    void starredItemsSuccess();
    void updatedItemsError(const QString &updateItemsErrorString);
    void updatedItemsSuccess();
    void startedFetchingImages(const int &numberOfItems);
    void finishedFetchingImages();
    void fetchingImages(const int &currentItem);

private slots:
    void dbusMarkedAllItemsReadError(const QString &markedAllItemsReadErrorString);
    void dbusMarkedAllItemsReadSuccess();
    void dbusMarkedItemsError(const QString &markedItemsErrorString);
    void dbusMarkedItemsSuccess();
    void dbusRequestedItemsError(const QString &requestedItemsErrorString);
    void dbusRequestedItemsSuccess();
    void dbusStarredItemsError(const QString &starredItemsErrorString);
    void dbusStarredItemsSuccess();
    void dbusUpdatedItemsError(const QString &updateItemsErrorString);
    void dbusUpdatedItemsSuccess();
    void dbusStartedFetchingImages(const int &numberOfItems);
    void dbusFinishedFetchingImages();
    void dbusFetchingImages(const int &currentItem);

private:
    de::buschmann23::ocNewsEngine::Items *items;

};

#endif // OCDBUSFOLDERS_H
