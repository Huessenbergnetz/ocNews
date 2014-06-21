#include "ocdbusimagefetcher.h"

OcDBusImageFetcher::OcDBusImageFetcher(QObject *parent) :
    QObject(parent)
{
    imageFetcher = new de::buschmann23::ocNewsEngine::ImageFetcher("de.buschmann23.ocNewsEngine", "/ImageFetcher", QDBusConnection::sessionBus(), this);
    connect(imageFetcher, SIGNAL(startedFetchingImages(int)), this, SLOT(dbusStartedFetchingImages(int)));
    connect(imageFetcher, SIGNAL(finishedFetchingImages()), this, SLOT(dbusFinishedFetchingImages()));
    connect(imageFetcher, SIGNAL(fetchingImages(int)), this, SLOT(dbusFetchingImages(int)));
}


int OcDBusImageFetcher::isFetchImagesRunning()
{
    return imageFetcher->isFetchImagesRunning();
}


void OcDBusImageFetcher::dbusStartedFetchingImages(const int &numberOfItems)
{
    emit startedFetchingImages(numberOfItems);
}

void OcDBusImageFetcher::dbusFinishedFetchingImages()
{
    emit finishedFetchingImages();
}

void OcDBusImageFetcher::dbusFetchingImages(const int &currentItem)
{
    emit fetchingImages(currentItem);
}
