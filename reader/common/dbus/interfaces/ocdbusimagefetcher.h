#ifndef OCDBUSIMAGEFETCHER_H
#define OCDBUSIMAGEFETCHER_H

#include <QObject>

#include "ocimagefetcherif.h"

class OcDBusImageFetcher : public QObject
{
    Q_OBJECT
public:
    explicit OcDBusImageFetcher(QObject *parent = 0);

public slots:
    int isFetchImagesRunning();

signals:
    void startedFetchingImages(const int &numberOfItems);
    void finishedFetchingImages();
    void fetchingImages(const int &currentItem);

private slots:
    void dbusStartedFetchingImages(const int &numberOfItems);
    void dbusFinishedFetchingImages();
    void dbusFetchingImages(const int &currentItem);

private:
    de::buschmann23::ocNewsEngine::ImageFetcher *imageFetcher;
};

#endif // OCDBUSIMAGEFETCHER_H
