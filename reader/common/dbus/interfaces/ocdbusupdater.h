#ifndef OCDBUSUPDATER_H
#define OCDBUSUPDATER_H

#include <QObject>

#include "ocupdaterif.h"

class OcDBusUpdater : public QObject
{
    Q_OBJECT
public:
    explicit OcDBusUpdater(QObject *parent = 0);

    Q_INVOKABLE void startUpdate();
    Q_INVOKABLE bool isUpdateRunning();
    Q_INVOKABLE int isFetchImagesRunning();
    
signals:
    void updateStarted();
    void updateFinished();
    void updateError(QString updateErrorMessage);
    void startedFetchingImages(const int &numberOfItems);
    void finishedFetchingImages();
    void fetchingImages(const int &currentItem);
    
public slots:

private slots:
    void dbusUpdateStarted();
    void dbusUpdateFinished();
    void dbusUpdateError(QString updateErrorMessage);
    void dbusStartedFetchingImages(const int &numberOfItems);
    void dbusFinishedFetchingImages();
    void dbusFetchingImages(const int &currentItem);

private:
    de::buschmann23::ocNewsEngine::Updater *updater;
    
};

#endif // OCDBUSUPDATER_H
