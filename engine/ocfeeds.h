#ifndef OCFEEDS_H
#define OCFEEDS_H

#include <QObject>
#include <QUrl>
#include <QImage>

#include "ochelper.h"
#include "ocdbmanager.h"
#include "ocnetwork.h"
#include "ocitems.h"
#include "../common/globals.h"

class OcFeeds : public QObject
{
    Q_OBJECT
public:
    explicit OcFeeds(QObject *parent = 0);

    void getFavicon(QString feedId, QString faviconLink);

public Q_SLOTS: // METHODS
    void createFeed(const QString &url, const QString &folderId, bool eventView);
    void deleteFeed(const QString &id);
    QVariantMap getFeeds();
    void markFeedRead(const QString &feedId);
    void moveFeed(const QString &id, const QString &folderId);
    void renameFeed(const QString &id, const QString &newName);
    void requestFeeds();

public slots:
    void feedDeletedCleanItems(int id);

signals:
    void requestedFeeds(QVariantMap feedsresult);
    void createdFeed(QVariantMap createFeedResult);
    void createdFeedUpdateDbSuccess(QVariantMap createFeedResult, QString feedname);
    void deletedFeed(int id);
    void movedFeed(int id, QString folderId);
    void markedReadFeed(int id);
    void renamedFeed(int id, QString name);

Q_SIGNALS: // SIGNALS
    void createdFeedError(const QString &createFeedResultError);
    void createdFeedSuccess(const QString &feedname);
    void deletedFeedError(const QString &deleteFeedResultError);
    void deletedFeedSuccess();
    void markedReadFeedError(const QString &markedReadFeedErrorResult);
    void markedReadFeedSuccess();
    void movedFeedError(const QString &moveFeedResultError);
    void movedFeedSuccess();
    void requestedFeedsError(const QString &requestedFeedsErrorString);
    void requestedFeedsSuccess();
    void renamedFeedSuccess(const QString &newName);
    void renamedFeedError(const QString &renamedFeedErrorString);

private slots:
    void feedsRequested();
    void feedsRequestedUpdateDb(QVariantMap feedsresult);
    void feedCreated(int eventView);
    void feedCreatedUpdateDb(QVariantMap createFeedResult);
    void feedCreatedFetchItems(QVariantMap createFeedResult, QString feedName);
    void feedDeleted();
    void feedDeletedUpdateDb(int id);
//    void feedDeletedCleanItems(int id);
    void feedMoved(QString folderId);
    void feedMovedUpdateDb(int id, QString folderId);
    void feedMarkedRead();
    void feedMarkedReadUpdateDb(int id);
    void feedRenamed(QString name);
    void feedRenamedUpdateDb(int id, const QString &name);

private:
    QUrl urlRequestFeeds, urlCreateFeed, urlDeleteFeed, urlMoveFeed, urlRenameFeed;
    QNetworkReply *replyRequestFeeds, *replyCreateFeed, *replyDeleteFeed, *replyMoveFeed, *replyGetFavicon, *replyMarkFeedRead, *replyRenameFeed;
    OcHelper helper;
    OcDbManager database;
    OcItems items;
    OcNetwork network;
    OcConfiguration config;

};

#endif // OCFEEDS_H
