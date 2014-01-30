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
    void requestedItems(QVariantMap requestItemsResult, QString type, QString fid);
    void updatedItems(QVariantMap updateItemsResult, QString type, QString fid);
    void markedItems(QStringList ids);
    void markedAllItemsRead(QString newestItemId);
    void starredItems(QStringList hashes);

    // item queue
    void dequeueFinished();

public slots:
    void cleanItems(int id);

Q_SIGNALS: // SIGNALS
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

private slots:
    void itemsRequested();
    void itemsRequestedUpdateDb(QVariantMap requestItemsResult, QString type, QString fId);
    void itemsUpdated();
    void itemsUpdatedUpdateDb(QVariantMap updateItemsResult, QString type, QString fid);
    void itemsMarked(QString joinedIds);
    void itemsMarkedUpdateDb(QStringList ids);
    void itemsStarred(QString joinedHashes);
    void itemsStarredUpdateDb(QStringList hashes);
    void itemsMarkedAllRead(QString newestItemid);
    void itemsMarkedAllReadUpdateDb(QString newestItemId);

    // item queue
    void dequeueRead();
    void dequeueUnread();
    void dequeueStarred();
    void dequeueUnstarred();
    void dequeueFinish();

private:
    QUrl urlRequestItems, urlUpdateItems, urlMarkItems, urlStarItems, urlMarkAllItemsRead;
    QNetworkReply *replyRequestItems, *replyUpdateItems, *replyMarkItems, *replyStarItems, *replyMarkAllItemsRead;
    OcHelper helper;
    OcDbManager database;
    OcNetwork network;
    Action action;
    OcConfiguration config;

    // item queue
    void queueItems(int action, QStringList &itemIds);
    void dequeueItems();

    // add items to event feed
    void updateEventFeed(const QList<int> &newsFeedItems);

    // cache images
    QString cacheImages(const QString &bodyText, int id);
};

#endif // OCITEMS_H
