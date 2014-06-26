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
    void createdFeedSuccess(const QString &feedname, const int &feedId);
    void deletedFeedError(const QString &deleteFeedResultError);
    void deletedFeedSuccess(const int &id);
    void markedReadFeedError(const QString &markedReadFeedError);
    void markedReadFeedSuccess(const int &feedId);
    void movedFeedError(const QString &moveFeedResultError);
    void movedFeedSuccess(const int &feedId, const int &folderId);
    void requestedFeedsError(const QString &requestedFeedsErrorString);
    void requestedFeedsSuccess(const QList<int> &updated, const QList<int> &newFeeds, const QList<int> &deleted);
    void renamedFeedSuccess(const QString &newName, const int &feedId);
    void renamedFeedError(const QString &renamedFeedErrorString);
    
public slots:
    void createFeed(const QString &url, const QString &folderId, bool eventView);
    void deleteFeed(const QString &id);
    void markFeedRead(const QString &feedId);
    void moveFeed(const QString &id, const QString &folderId);
    void requestFeeds();
    void renameFeed(const QString &id, const QString &newName);

private slots:
    void dbusCreatedFeedError(const QString &createFeedResultError);
    void dbusCreatedFeedSuccess(const QString &feedname, const int &feedId);
    void dbusDeletedFeedError(const QString &deleteFeedResultError);
    void dbusDeletedFeedSuccess(const int &id);
    void dbusMarkedReadFeedError(const QString &markedReadFeedErrorResult);
    void dbusMarkedReadFeedSuccess(const int &feedId);
    void dbusMovedFeedError(const QString &moveFeedResultError);
    void dbusMovedFeedSuccess(const int &feedId, const int &folderId);
    void dbusRequestedFeedsError(const QString &requestedFeedsErrorString);
    void dbusRequestedFeedsSuccess(const QList<int> &updated, const QList<int> &newFeeds, const QList<int> &deleted);
    void dbusRenamedFeedSuccess(const QString &newName, const int &feedId);
    void dbusRenamedFeedError(const QString &renamedFeedErrorString);

private:
    de::buschmann23::ocNewsEngine::Feeds *feeds;
    
};

#endif // OCDBUSFEEDS_H
