#ifndef OCDOWNLOADMANAGER_H
#define OCDOWNLOADMANAGER_H

#include <QObject>
#include <QFile>
#include <QQueue>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#if defined(MEEGO_EDITION_HARMATTAN)
#include <TransferUI/Client>
#include <TransferUI/Transfer>
#endif

class OcDownloadManager : public QObject
{
    Q_OBJECT
public:
    explicit OcDownloadManager(QObject *parent = 0);

public Q_SLOTS: // METHODS
    bool abortDownload(const QString &id);
    void append(const QString &id);
    QString getCurrentItem();
    QString itemExists(const QString &link, const QString &mime);
    bool itemInQueue(const QString &id);
    bool deleteFile(const QString &link, const QString &mime);
    
Q_SIGNALS: // SIGNALS
    void finished();
    void finishedFile(const QString &finishedFileId);
    void progress(int rec, int tot);
    void started(const QString &startedFileId);
    void enqueued(const QString &enqueuedId);
    
public slots:
    bool abortDownload();

private slots:
    void startNextDownload();
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void downloadFinished();
    void downloadReadyRead();

private:
    QNetworkAccessManager manager;
    QQueue<QString> downloadQueue;
    QNetworkReply *currentDownload;
    QString currentItem;
    QString currentFile;
    int currentType;
    bool transferAborted;
    QFile output;
    int getEnclosureType(const QString &encMime);
    QString getMimeIcon(int type);
    QString saveFileName(const QString &url, const QString &mime);
#if defined(MEEGO_EDITION_HARMATTAN)
    TransferUI::Client *transferClient;
    TransferUI::Transfer *transferItem;
#endif
    
};

#endif // OCDOWNLOADMANAGER_H
