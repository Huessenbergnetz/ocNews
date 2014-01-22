#ifndef OCDBUSFEEDS_H
#define OCDBUSFEEDS_H

#include <QObject>

#include "ocfeedsif.h"

class OcDBusFeeds : public QObject
{
    Q_OBJECT
public:
    explicit OcDBusFeeds(QObject *parent = 0);

    Q_INVOKABLE QVariantMap getFeeds();
    
signals:
    void createdFeedError(const QString &createFeedResultError);
    void createdFeedSuccess(const QString &feedname);
    void deletedFeedError(const QString &deleteFeedResultError);
    void deletedFeedSuccess();
    void markedReadFeedError(const QString &markedReadFeedError);
    void markedReadFeedSuccess();
    void movedFeedError(const QString &moveFeedResultError);
    void movedFeedSuccess();
    void requestedFeedsError(const QString &requestedFeedsErrorString);
    void requestedFeedsSuccess();
    
public slots:
    void createFeed(const QString &url, const QString &folderId, bool eventView);
    void deleteFeed(const QString &id);
    void markFeedRead(const QString &feedId);
    void moveFeed(const QString &id, const QString &folderId);
    void requestFeeds();

private slots:
    void dbusCreatedFeedError(const QString &createFeedResultError);
    void dbusCreatedFeedSuccess(const QString &feedname);
    void dbusDeletedFeedError(const QString &deleteFeedResultError);
    void dbusDeletedFeedSuccess();
    void dbusMarkedReadFeedError(const QString &markedReadFeedErrorResult);
    void dbusMarkedReadFeedSuccess();
    void dbusMovedFeedError(const QString &moveFeedResultError);
    void dbusMovedFeedSuccess();
    void dbusRequestedFeedsError(const QString &requestedFeedsErrorString);
    void dbusRequestedFeedsSuccess();

private:
    de::buschmann23::ocNewsEngine::Feeds *feeds;
    
};

#endif // OCDBUSFEEDS_H
