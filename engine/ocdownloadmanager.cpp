#include "ocdownloadmanager.h"
#include "../common/globals.h"

#include <QFileInfo>
#include <QDir>
#include <QTimer>
#include <QDebug>
#include <QSqlQuery>

OcDownloadManager::OcDownloadManager(QObject *parent) :
    QObject(parent)
{
    currentItem = "";
    currentFile = "";
    currentType = 0;
    transferAborted = false;

#if defined(MEEGO_EDITION_HARMATTAN)
    transferClient = new TransferUI::Client();

    if(!transferClient->init()) {
        qDebug()<<"Cannot initialize TUIClient";//error
        delete transferClient;
    }
#endif

}


void OcDownloadManager::append(const QString &id)
{
    if (currentItem == "" && downloadQueue.isEmpty())
        QTimer::singleShot(0, this, SLOT(startNextDownload()));

    downloadQueue.enqueue(id);
    emit enqueued(id);
}


QString OcDownloadManager::saveFileName(const QString &url, const QString &mime)
{
    QString basename = QFileInfo(url).fileName();

    if (basename.isEmpty())
        basename = "download";

    QString storagePath(QDir::homePath());
//    storagePath.append(MEDIA_PATH);

    switch(getEnclosureType(mime))
    {
    case 1:
        storagePath.append(MEDIA_PATH_AUDIO);
        break;
    case 2:
        storagePath.append(MEDIA_PATH_VIDEO);
        break;
    case 3:
        storagePath.append(MEDIA_PATH_PDF);
        break;
    case 4:
        storagePath.append(MEDIA_PATH_IMAGE);
        break;
    default:
        storagePath.append(MEDIA_PATH);
        break;
    }

    storagePath.append("/").append(basename);

    return storagePath;
}



void OcDownloadManager::startNextDownload()
{

    if (downloadQueue.isEmpty())
    {
        emit finished();
        return;
    }

    QString id = downloadQueue.dequeue();
    qDebug() << "ItemID to download: " << id;

    QSqlQuery query;
    query.exec(QString("SELECT enclosureMime, enclosureLink FROM items WHERE id = %1").arg(id.toInt()));

    QString link;
    QString mime;
    if (query.next())
    {
        mime = query.value(0).toString();
        link = query.value(1).toString();
    }

    qDebug() << "Download link: " << link;
    qDebug() << "Download mime: " << mime;

    QUrl url(link);
//    QString fileName = saveFileName(link);
    currentFile = saveFileName(link, mime);
    currentType = getEnclosureType(mime);

    output.setFileName(currentFile);

    if (output.exists())
    {
        qDebug() << "File exits already";
        startNextDownload();
        return;
    }


    if (!output.open(QIODevice::WriteOnly))
    {
        qDebug() << "Can not open file for downloading";
        startNextDownload();
        return;
    }

#if defined(MEEGO_EDITION_HARMATTAN)
    transferItem = transferClient->registerTransfer(tr("Downloading enclosed "), TransferUI::Client::TRANSFER_TYPES_DOWNLOAD);
    transferItem->waitForCommit();
    transferItem->setTargetName(QFileInfo(link).fileName());
    transferItem->setName(url.host());
    transferItem->setCanPause(false);
    transferItem->setIcon(getMimeIcon(getEnclosureType(mime)));
    transferItem->setActive(.00);
    transferItem->commit();

    connect(transferItem, SIGNAL(cancel()), this, SLOT(abortDownload()));
#endif

    currentItem = id;

    QNetworkRequest request(url);
    currentDownload = manager.get(request);
    connect(currentDownload, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(downloadProgress(qint64,qint64)));
    connect(currentDownload, SIGNAL(finished()), this, SLOT(downloadFinished()));
    connect(currentDownload, SIGNAL(readyRead()), this, SLOT(downloadReadyRead()));

    emit started(id);
}


void OcDownloadManager::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
#if defined(MEEGO_EDITION_HARMATTAN)
    transferItem->setProgress((float)bytesReceived/bytesTotal);
#endif
    emit progress((int)bytesReceived, (int)bytesTotal);
}


void OcDownloadManager::downloadFinished()
{
    output.close();

#if defined(MEEGO_EDITION_HARMATTAN)
    if (transferAborted)
    {
        transferItem->markCancelled();
        transferClient->removeTransfer(transferItem->transferId());
    } else {
        transferItem->markCompleted(true, "", currentFile);
        transferClient->removeTransfer(transferItem->transferId());
    }
#endif

    emit finishedFile(currentItem);

    currentItem = "";
    currentFile = "";
    currentType = 0;
    transferAborted = false;

    currentDownload->deleteLater();
    startNextDownload();
}


void OcDownloadManager::downloadReadyRead()
{
    output.write(currentDownload->readAll());
}


bool OcDownloadManager::abortDownload()
{
    return abortDownload(currentItem);
}


bool OcDownloadManager::abortDownload(const QString &id)
{
    if (currentItem == id)
    {
        QFile file(currentFile);
        transferAborted = true;
        currentDownload->abort();
        file.remove();
//        startNextDownload();
        return true;
    } else if (downloadQueue.removeOne(id)) {
        return true;
    } else {
        return false;
    }
}


QString OcDownloadManager::getCurrentItem()
{
    return currentItem;
}


QString OcDownloadManager::itemExists(const QString &link, const QString &mime)
{
    QFile file(saveFileName(link, mime));
    QString result = "";

    if (file.exists())
        result = file.fileName();


    qDebug() << "Does file exists: " << result;
    return result;
}


bool OcDownloadManager::deleteFile(const QString &link, const QString &mime)
{
    QString fileName = saveFileName(link, mime);

    if (fileName == currentFile)
    {
        return false;
    } else {
        QFile file(fileName);
        return file.remove();
    }
}

bool OcDownloadManager::itemInQueue(const QString &id)
{
    return downloadQueue.contains(id);
}


int OcDownloadManager::getEnclosureType(const QString &encMime)
{

    if (encMime.contains("audio", Qt::CaseInsensitive)) {
        return 1;
    } else if (encMime.contains("video", Qt::CaseInsensitive)) {
        return 2;
    } else if (encMime.contains("pdf", Qt::CaseInsensitive)) {
        return 3;
    } else if (encMime.contains("image", Qt::CaseInsensitive)) {
        return 4;
    } else {
        return 0;
    }
}


QString OcDownloadManager::getMimeIcon(int type)
{
    QString icon;

    switch(type)
    {
    case 1:
        icon = MIME_ICON_AUDIO;
        break;
    case 2:
        icon = MIME_ICON_VIDEO;
        break;
    case 3:
        icon = MIME_ICON_PDF;
        break;
    case 4:
        icon = MIME_ICON_IMAGE;
        break;
    default:
        icon = MIME_ICON_GENERAL;
        break;
    }

    return icon;
}


