#ifndef OCDBUSDOWNLOADS_H
#define OCDBUSDOWNLOADS_H

#include <QObject>
#include "ocdownloadsif.h"

class OcDBusDownloads : public QObject
{
    Q_OBJECT
public:
    explicit OcDBusDownloads(QObject *parent = 0);

public slots:
    bool abortDownload(const QString &id);
    void append(const QString &id);
    QString getCurrentItem();
    QString itemExists(const QString &link, const QString &mime);
    bool itemInQueue(const QString &id);
    bool deleteFile(const QString &link, const QString &mime);
    
signals:
    void finished();
    void finishedFile(const QString &finishedFileId);
    void progress(int rec, int tot);
    void started(const QString &startedFileId);
    void enqueued(const QString &enqueuedId);

private slots:
    void dbusFinished();
    void dbusFinishedFile(const QString &finishedFileId);
    void dbusProgress(int rec, int tot);
    void dbusStarted(const QString &startedFileId);
    void dbusEnqueued(const QString &enqueuedId);

private:
    de::buschmann23::ocNewsEngine::Downloads *downloads;
    
};

#endif // OCDBUSDOWNLOADS_H
