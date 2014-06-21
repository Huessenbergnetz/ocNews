#include "ocdbusfeeds.h"

OcDBusFeeds::OcDBusFeeds(QObject *parent) :
    QObject(parent)
{
    feeds = new de::buschmann23::ocNewsEngine::Feeds("de.buschmann23.ocNewsEngine", "/Feeds", QDBusConnection::sessionBus(), this);
    connect(feeds, SIGNAL(createdFeedError(QString)), this, SLOT(dbusCreatedFeedError(QString)));
    connect(feeds, SIGNAL(createdFeedSuccess(QString)), this, SLOT(dbusCreatedFeedSuccess(QString)));
    connect(feeds, SIGNAL(deletedFeedError(QString)), this, SLOT(dbusDeletedFeedError(QString)));
    connect(feeds, SIGNAL(deletedFeedSuccess(int)), this, SLOT(dbusDeletedFeedSuccess(int)));
    connect(feeds, SIGNAL(markedReadFeedError(QString)), this, SLOT(dbusMarkedReadFeedError(QString)));
    connect(feeds, SIGNAL(markedReadFeedSuccess(QString)), this, SLOT(dbusMarkedReadFeedSuccess(QString)));
    connect(feeds, SIGNAL(movedFeedError(QString)), this, SLOT(dbusMovedFeedError(QString)));
    connect(feeds, SIGNAL(movedFeedSuccess()), this, SLOT(dbusMovedFeedSuccess()));
    connect(feeds, SIGNAL(requestedFeedsError(QString)), this, SLOT(dbusRequestedFeedsError(QString)));
    connect(feeds, SIGNAL(requestedFeedsSuccess(QList<int>, QList<int>, QList<int>)), this, SLOT(dbusRequestedFeedsSuccess(QList<int>, QList<int>, QList<int>)));
    connect(feeds, SIGNAL(renamedFeedError(QString)), this, SLOT(dbusRenamedFeedError(QString)));
    connect(feeds, SIGNAL(renamedFeedSuccess(QString)), this, SLOT(dbusRenamedFeedSuccess(QString)));
}

void OcDBusFeeds::createFeed(const QString &url, const QString &folderId, bool eventView)
{
    feeds->createFeed(url, folderId, eventView);
}

void OcDBusFeeds::deleteFeed(const QString &id)
{
    feeds->deleteFeed(id);
}

void OcDBusFeeds::markFeedRead(const QString &feedId)
{
    feeds->markFeedRead(feedId);
}

void OcDBusFeeds::moveFeed(const QString &id, const QString &folderId)
{
    feeds->moveFeed(id, folderId);
}

void OcDBusFeeds::requestFeeds()
{
    feeds->requestFeeds();
}

QVariantMap OcDBusFeeds::getFeeds()
{
    return feeds->getFeeds();
}

void OcDBusFeeds::renameFeed(const QString &id, const QString &newName)
{
    feeds->renameFeed(id, newName);
}

void OcDBusFeeds::dbusCreatedFeedError(const QString &createFeedResultError)
{
    emit createdFeedError(createFeedResultError);
}

void OcDBusFeeds::dbusCreatedFeedSuccess(const QString &feedname)
{
    emit createdFeedSuccess(feedname);
}

void OcDBusFeeds::dbusDeletedFeedError(const QString &deleteFeedResultError)
{
    emit deletedFeedError(deleteFeedResultError);
}

void OcDBusFeeds::dbusDeletedFeedSuccess(const int &id)
{
    emit deletedFeedSuccess(id);
}

void OcDBusFeeds::dbusMarkedReadFeedError(const QString &markedReadFeedErrorResult)
{
    emit markedReadFeedError(markedReadFeedErrorResult);
}

void OcDBusFeeds::dbusMarkedReadFeedSuccess(const QString &feedId)
{
    emit markedReadFeedSuccess(feedId);
}

void OcDBusFeeds::dbusMovedFeedError(const QString &moveFeedResultError)
{
    emit movedFeedError(moveFeedResultError);
}

void OcDBusFeeds::dbusMovedFeedSuccess()
{
    emit movedFeedSuccess();
}

void OcDBusFeeds::dbusRequestedFeedsError(const QString &requestedFeedsErrorString)
{
    emit requestedFeedsError(requestedFeedsErrorString);
}

void OcDBusFeeds::dbusRequestedFeedsSuccess(const QList<int> &updated, const QList<int> &newFeeds, const QList<int> &deleted)
{
    emit requestedFeedsSuccess(updated, newFeeds, deleted);
}

void OcDBusFeeds::dbusRenamedFeedError(const QString &renamedFeedErrorString)
{
    emit renamedFeedError(renamedFeedErrorString);
}

void OcDBusFeeds::dbusRenamedFeedSuccess(const QString &newName)
{
    emit renamedFeedSuccess(newName);
}
