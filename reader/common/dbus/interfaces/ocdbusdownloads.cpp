#include "ocdbusdownloads.h"

OcDBusDownloads::OcDBusDownloads(QObject *parent) :
    QObject(parent)
{
    downloads = new de::buschmann23::ocNewsEngine::Downloads("de.buschmann23.ocNewsEngine", "/Downloads", QDBusConnection::sessionBus(), this);
    connect(downloads, SIGNAL(finished()), this, SLOT(dbusFinished()));
    connect(downloads, SIGNAL(finishedFile(QString)), this, SLOT(dbusFinishedFile(QString)));
    connect(downloads, SIGNAL(progress(int,int)), this, SLOT(dbusProgress(int,int)));
    connect(downloads, SIGNAL(started(QString)), this, SLOT(dbusStarted(QString)));
}

void OcDBusDownloads::append(const QString &id)
{
    downloads->append(id);
}

bool OcDBusDownloads::abortDownload(const QString &id)
{
    return downloads->abortDownload(id);
}

QString OcDBusDownloads::getCurrentItem()
{
    return downloads->getCurrentItem();
}

QString OcDBusDownloads::itemExists(const QString &link, const QString &mime)
{
    return downloads->itemExists(link, mime);
}

bool OcDBusDownloads::itemInQueue(const QString &id)
{
    return downloads->itemInQueue(id);
}

bool OcDBusDownloads::deleteFile(const QString &link, const QString &mime)
{
    return downloads->deleteFile(link, mime);
}

void OcDBusDownloads::dbusFinished()
{
    emit finished();
}

void OcDBusDownloads::dbusFinishedFile(const QString &finishedFileId)
{
    emit finishedFile(finishedFileId);
}

void OcDBusDownloads::dbusProgress(int rec, int tot)
{
    emit progress(rec, tot);
}

void OcDBusDownloads::dbusStarted(const QString &startedFileId)
{
    emit started(startedFileId);
}

void OcDBusDownloads::dbusEnqueued(const QString &enqueuedId)
{
    emit enqueued(enqueuedId);
}
