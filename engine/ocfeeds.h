#ifndef OCFEEDS_H
#define OCFEEDS_H

#include <QObject>
#include <QUrl>
#include <QImage>

#include "ochelper.h"
#include "ocdbmanager.h"
#include "ocnetwork.h"
#include "ocnotifications.h"
#include "../common/globals.h"

class OcFeeds : public QObject
{
    Q_OBJECT
public:
    explicit OcFeeds(QObject *parent = 0);

    void getFavicon(QString feedId, QString faviconLink);

public Q_SLOTS: // METHODS
    void createFeed(const QString &url, const QString &folderId, const bool &eventView);
    void deleteFeed(const QString &id);
    QVariantMap getFeeds();
    void markFeedRead(const QString &feedId);
    void moveFeed(const QString &id, const QString &folderId);
    void renameFeed(const QString &id, const QString &newName);
    void requestFeeds();

public slots:
    void feedDeletedCleanItems(int id);

Q_SIGNALS: // SIGNALS
    void createdFeedError(const QString &createFeedResultError);
    void createdFeedSuccess(const QString &feedname);
    void deletedFeedError(const QString &deleteFeedResultError);
    void deletedFeedSuccess(const int &id);
    void markedReadFeedError(const QString &markedReadFeedErrorResult);
    void markedReadFeedSuccess(const QString &feedId);
    void movedFeedError(const QString &moveFeedResultError);
    void movedFeedSuccess();
    void requestedFeedsError(const QString &requestedFeedsErrorString);
    void requestedFeedsSuccess(const QList<int> &updated, const QList<int> &newFeeds, const QList<int> &deleted);
    void renamedFeedSuccess(const QString &newName);
    void renamedFeedError(const QString &renamedFeedErrorString);

signals:
    void feedCreatedFetchItems(const QString &batchSize, const QString &offset, const QString &type, const QString &id, const QString &getRead);

private slots:
    void feedsRequested();
    void feedCreated();
    void feedDeleted();
    void feedMoved();
    void feedMarkedRead();
    void feedRenamed();

private:
    void feedsRequestedUpdateDb(const QVariantMap &feedsresult);
    void feedCreatedUpdateDb(const QVariantMap &createFeedResult);
    void feedDeletedUpdateDb(const int &id);
    void feedMovedUpdateDb(const int &id, const QString &folderId);
    void feedMarkedReadUpdateDb(const int &id);
    void feedRenamedUpdateDb(const int &id, const QString &name);

    QNetworkReply *replyRequestFeeds, *replyCreateFeed, *replyDeleteFeed, *replyMoveFeed, *replyGetFavicon, *replyMarkFeedRead, *replyRenameFeed;
    OcHelper helper;
    OcDbManager database;
    OcNetwork network;
    OcConfiguration config;
    OcNotifications notify;

    bool addFeedToEventView;
    QString folderIdToMoveTo;
    QString newFeedName;

};

#endif // OCFEEDS_H
