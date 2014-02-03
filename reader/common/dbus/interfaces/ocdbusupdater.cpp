#include "ocdbusupdater.h"

OcDBusUpdater::OcDBusUpdater(QObject *parent) :
    QObject(parent)
{
    updater = new de::buschmann23::ocNewsEngine::Updater("de.buschmann23.ocNewsEngine", "/Updater", QDBusConnection::sessionBus(), this);
    connect(updater, SIGNAL(updateError(QString)), this, SLOT(dbusUpdateError(QString)));
    connect(updater, SIGNAL(updateFinished()), this, SLOT(dbusUpdateFinished()));
    connect(updater, SIGNAL(updateStarted()), this, SLOT(dbusUpdateStarted()));
    connect(updater, SIGNAL(startedFetchingImages(int)), this, SLOT(dbusStartedFetchingImages(int)));
    connect(updater, SIGNAL(finishedFetchingImages()), this, SLOT(dbusFinishedFetchingImages()));
    connect(updater, SIGNAL(fetchingImages(int)), this, SLOT(dbusFetchingImages(int)));
}

void OcDBusUpdater::startUpdate()
{
    updater->startUpdate();
}

bool OcDBusUpdater::isUpdateRunning()
{
    return updater->isUpdateRunning();
}

int OcDBusUpdater::isFetchImagesRunning()
{
    return updater->isFetchImagesRunning();
}

void OcDBusUpdater::dbusUpdateError(QString updateErrorMessage)
{
    emit updateError(updateErrorMessage);
}

void OcDBusUpdater::dbusUpdateFinished()
{
    emit updateFinished();
}

void OcDBusUpdater::dbusUpdateStarted()
{
    emit updateStarted();
}

void OcDBusUpdater::dbusFetchingImages(const int &currentItem)
{
    emit fetchingImages(currentItem);
}

void OcDBusUpdater::dbusFinishedFetchingImages()
{
    emit finishedFetchingImages();
}

void OcDBusUpdater::dbusStartedFetchingImages(const int &numberOfItems)
{
    emit startedFetchingImages(numberOfItems);
}
