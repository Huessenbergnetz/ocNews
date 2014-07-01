#include <QDebug>
#include "ocimagefetcher.h"

OcImageFetcher::OcImageFetcher(QObject *parent) :
    QObject(parent)
{
    itemsToFetchImages = 0;

#if defined(MEEGO_EDITION_HARMATTAN)
    transferClient = new TransferUI::Client();

    if(!transferClient->init()) {
        qDebug()<<"Cannot initialize TUIClient";//error
        delete transferClient;
    }
#endif
}


void OcImageFetcher::fetchImages(const QList<int> &updated, const QList<int> &newItems, const QList<int> &deleted)
{
    int imgHandling = config.value("display/handleimgs", 0).toInt();

    if (!newItems.isEmpty())
    {

        // wait one second before fetching images to give the database models some time to load
        QEventLoop loop;
        QTimer::singleShot(1000, &loop, SLOT(quit()));
        loop.exec();

#ifdef QT_DEBUG
        qDebug() << "Start fetching images.";
#endif

        itemsToFetchImages = newItems.size();

#if defined(MEEGO_EDITION_HARMATTAN)
        if (transferClient) {
            transferItem = transferClient->registerTransfer(tr("ocNews Image Fetcher"), TransferUI::Client::TRANSFER_TYPES_DOWNLOAD);
            if (transferItem) {
                transferItem->waitForCommit();
                transferItem->setTargetName("ocNews");
                transferItem->setName(tr("Fetching images"));
                transferItem->setSize(0);
                transferItem->setCanPause(false);
                transferItem->setIcon("icon-m-ocnews");
                transferItem->setActive(0);
                transferItem->commit();
            }
        }
#endif

        emit startedFetchingImages(itemsToFetchImages);

        for (int i = 0; i < newItems.size(); ++i)
        {

#if defined(MEEGO_EDITION_HARMATTAN)
            transferItem->setProgress((float)i/itemsToFetchImages);
#endif

            emit fetchingImages(i);

            QString body = QString("");

            QSqlQuery query;
            query.exec(QString("SELECT body FROM items WHERE id = %1").arg(newItems.at(i)));

            if (query.next())
                body = cacheImages(query.value(0).toString(), newItems.at(i), imgHandling);



            if (body != "")
            {
                QSqlQuery query2;
                query2.prepare("UPDATE items SET body = :body WHERE id = :id;");
                query2.bindValue(":body", body);
                query2.bindValue(":id", newItems.at(i));
                query2.exec();
            }
        }

#if defined(MEEGO_EDITION_HARMATTAN)
        transferItem->setProgress(1.0);
        transferItem->setName(tr("Fetching images finished"));
        transferItem->markDone();
        transferClient->removeTransfer(transferItem->transferId());
        delete transferItem;
        transferItem = 0;
#endif

        itemsToFetchImages = 0;
        emit finishedFetchingImages(itemsToFetchImages);

#ifdef QT_DEBUG
        qDebug() << "Finished fetching images.";
#endif
    }

    if (!deleted.isEmpty())
        deleteCachedImages(deleted);
}



QString OcImageFetcher::cacheImages(const QString &bodyText, int id, int imageHandling)
{
    QString newBodyText = bodyText;
    QRegExp findImg("<img[^>]*>");
    QStringList foundImages;
    int pos = 0;

    while((pos = findImg.indexIn(bodyText, pos)) != -1)
    {
        foundImages << findImg.cap(0);
        pos += findImg.matchedLength();
    }
#ifdef QT_DEBUG
    qDebug() << "Found imgs: " << foundImages;
#endif

    if (!foundImages.isEmpty())
    {
        QSqlQuery query;

        for (int i = 0; i < foundImages.size(); ++i)
        {
            QVariantMap imageInfo;

            if (imageHandling != 2)
            {
                imageInfo = extractImgData(foundImages.at(i), false);
            }
            else
            {
                imageInfo = extractImgData(foundImages.at(i), true);

                QUrl fileUrl(imageInfo["src"].toString());
                QString fileName("_image_");
                fileName.prepend(QString::number(id));
                fileName.append(QString::number(i)).append(".");

                QString storagePath(QDir::homePath());
                storagePath.append(IMAGE_CACHE);
                storagePath.append(QDir::separator()).append(fileName);
                storagePath = QDir::toNativeSeparators(storagePath);

                QEventLoop dlLoop;

                QNetworkRequest request(fileUrl);

                QNetworkReply *replyGetImage = network.get(request);

                connect(replyGetImage, SIGNAL(finished()), &dlLoop, SLOT(quit()));
                dlLoop.exec();

                if (replyGetImage->error() == QNetworkReply::NoError)
                {

                    QByteArray recData = replyGetImage->readAll();

                    QImage image;
                    if (recData.size() > 0 && image.loadFromData(recData))
                    {

                        imageInfo["width"] = image.width();
                        imageInfo["height"] = image.height();

                        storagePath.append(getFileTypeSuffix(recData));

#ifdef QT_DEBUG
            qDebug() << storagePath;
#endif


                        QString oldSrc = imageInfo["src"].toString();
                        QString newSrc = storagePath;

                        oldSrc.prepend("src=\"");
                        newSrc.prepend("src=\"");

#ifdef QT_DEBUG
                        qDebug() << "Image Seize: " << imageInfo["width"].toInt() << " x " << imageInfo["height"].toInt();
#endif

                        if (image.save(storagePath))
                        {
                            newBodyText.replace(oldSrc, newSrc, Qt::CaseSensitive);
                            imageInfo["src"] = storagePath;
                            replyGetImage->deleteLater();
                        }
                        else
                        {

                            qDebug() << "Try to save image with QFile method.";

                            QFile file(storagePath);
                            file.open(QIODevice::WriteOnly);

                            if (file.write(recData) != -1)
                            {

                                newBodyText.replace(oldSrc, newSrc, Qt::CaseSensitive);
                                imageInfo["src"] = storagePath;
                                replyGetImage->deleteLater();

                            } else {

                                qDebug() << "Failed to save image";
                                imageInfo = extractImgData(foundImages.at(i), false);
                                replyGetImage->deleteLater();

                            }

                            file.close();

                        }

                    }
                    else
                    {
                        qDebug() << "Can not load image data.";
                        imageInfo = extractImgData(foundImages.at(i), false);
                        replyGetImage->deleteLater();
                    }
                }
                else
                {
                    qDebug() << "Can not download image file.";
                    imageInfo = extractImgData(foundImages.at(i), false);
                    replyGetImage->deleteLater();
                }
            }

            query.prepare("INSERT INTO images (parentId, path, width, height) VALUES (:parentId, :path, :width, :height);");
            query.bindValue(":parentId", id);
            query.bindValue(":path", imageInfo["src"]);
            query.bindValue(":width", imageInfo["width"]);
            query.bindValue(":height", imageInfo["height"]);
            query.exec();
        }
    }

    return newBodyText;
}


QVariantMap OcImageFetcher::extractImgData(const QString &imgStr, bool srcOnly)
{
    QVariantMap result;

    QRegExp findSrc("(http|ftp)[^\"]*");
    imgStr.contains(findSrc);
    result["src"] =  findSrc.cap(0);

    if (!srcOnly)
    {
        QRegExp findWidth("width=\"\\d+\"");
        QRegExp findHeight("height=\"\\d+\"");

        imgStr.contains(findWidth);
        imgStr.contains(findHeight);

        QString widthString = findWidth.cap(0);
        QString heightString = findHeight.cap(0);

        widthString.chop(1);
        widthString.remove(0, 7);
        heightString.chop(1);
        heightString.remove(0, 8);

        result["width"] = widthString.toInt();
        result["height"] = heightString.toInt();
    }

#ifdef QT_DEBUG
    qDebug() << "Image information: " << result;
#endif

    return result;
}


void OcImageFetcher::deleteCachedImages(const QList<int> &idsToDelte)
{
    QSqlQuery query;

    QStringList imagesToDelete;

    QSqlDatabase::database().transaction();
    for (int i = 0; i < idsToDelte.size(); ++i)
    {
        query.exec(QString("SELECT path FROM images WHERE parentId = %1").arg(idsToDelte.at(i)));
        while(query.next())
            imagesToDelete << query.value(0).toString();

        query.exec(QString("DELETE FROM images WHERE parentId = %1").arg(idsToDelte.at(i)));
    }
    QSqlDatabase::database().commit();

    if (!imagesToDelete.isEmpty())
    {
        for (int f = 0; f < imagesToDelete.size(); ++f)
        {
            if (!imagesToDelete.at(f).startsWith("http", Qt::CaseInsensitive)) {
#ifdef QT_DEBUG
                qDebug() << "Deleting image: " << imagesToDelete.at(f);
#endif
                QFile::remove(imagesToDelete.at(f));
            }
        }
    }
}



QString OcImageFetcher::getFileTypeSuffix(const QByteArray &data)
{
    QByteArray t_data = data.toHex().toUpper();
    QString magic = t_data.left(16);
    QString result;

#ifdef QT_DEBUG
    qDebug() << "Try to get suffix from: " << magic;
#endif

    if (magic.contains("474946383961") || magic.contains("474946383761")) {
        result = "gif";
    } else if (magic.contains("FFD8FF")) {
        result = "jpg";
    } else if (magic.contains("89504E470D0A1A0A")) {
        result = "png";
    } else {
        result = "";
    }

    return result;
}



int OcImageFetcher::isFetchImagesRunning()
{
#ifdef QT_DEBUG
    qDebug() << "Items is fetching images: " << itemsToFetchImages;
#endif
    return itemsToFetchImages;
}
