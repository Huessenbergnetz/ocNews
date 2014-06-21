#ifndef OCITEMS_H
#define OCITEMS_H

#include <QObject>
#include <QUrl>
#if defined(MEEGO_EDITION_HARMATTAN)
#include <meventfeed.h>
#endif
#include "occonfiguration.h"
#include "ochelper.h"
#include "ocnetwork.h"
#include "ocdbmanager.h"
#include "ocnotifications.h"
#include "../common/globals.h"

class OcItems : public QObject
{
    Q_OBJECT
public:
    explicit OcItems(QObject *parent = 0);

    enum Action { MarkRead, MarkUnread, Star, Unstar };

public Q_SLOTS: // METHODS
    void markAllItemsRead();
    void markItems(const QString &action, const QVariantList &ids);
    void markItemsTillThis(const QString &action, const QDBusVariant &pubDate, const QDBusVariant &feedId);
    void requestItems(const QString &batchSize, const QString &offset, const QString &type, const QString &id, const QString &getRead); // default: "1000", "0", "3", "0", "true"
    void starItems(const QString &action, const QVariantMap &itemIds);
    void updateItems(const QString &lastModified, const QString &type, const QString &id); // default: "0", "3", "0"

signals:
    // item queue
    void dequeueFinished();
    void cleanedItems(const QList<int> &updated, const QList<int> &newItems, const QList<int> &deleted);

public slots:
    void cleanItems(const QList<int> &updated, const QList<int> &newFeeds, const QList<int> &deleted);
    void cleanItems(const int &feedId);
    void cleanItems(const QList<int> &feedIds);

Q_SIGNALS: // SIGNALS
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
    void itemsRequested();
    void itemsUpdated();
    void itemsMarked();
    void itemsStarred();
    void itemsMarkedAllRead();

    // item queue
    void dequeueRead();
    void dequeueUnread();
    void dequeueStarred();
    void dequeueUnstarred();
    void dequeueFinish();

private:
    void itemsRequestedUpdateDb(const QVariantMap &requestItemsResult, const QString &type, const QString &fId);
    void itemsUpdatedUpdateDb(const QVariantMap &updateItemsResult, const QString &type, const QString &fid);
    void itemsMarkedUpdateDb(const QStringList &ids);
    void itemsStarredUpdateDb(const QStringList &hashes);
    void itemsMarkedAllReadUpdateDb(const QString &newestItemId);

    QUrl urlRequestItems, urlUpdateItems, urlMarkItems, urlStarItems, urlMarkAllItemsRead;
    QNetworkReply *replyRequestItems, *replyUpdateItems, *replyMarkItems, *replyStarItems, *replyMarkAllItemsRead;
    OcHelper helper;
    OcDbManager database;
    OcNetwork network;
    Action action;
    OcConfiguration config;
    OcNotifications notify;

    // item queue
    void queueItems(int action, QStringList &itemIds);
    void dequeueItems();

    // add items to event feed
    void updateEventFeed(const QList<int> &newsFeedItems);

    QStringList itemsToMark, hashesToStar;
    QString markAllReadNewestId;
};

#endif // OCITEMS_H
