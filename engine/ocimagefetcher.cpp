#include "QsLog.h"
#include "ocimagefetcher.h"

OcImageFetcher::OcImageFetcher(QObject *parent) :
    QObject(parent)
{
    itemsToFetchImages = 0;

#if defined(MEEGO_EDITION_HARMATTAN)
    transferClient = new TransferUI::Client();

    if(!transferClient->init()) {
        QLOG_ERROR() << "Image fetcher: Cannot initialize TUIClient";
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

        QLOG_INFO() << "Start fetching images. Handling type: " << imgHandling;

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
            } else {
                QLOG_ERROR() << "Image fetcher: can not register transfer on transfer client.";
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
            if (!query.exec(QString("SELECT body FROM items WHERE id = %1").arg(newItems.at(i)))) {
                QLOG_ERROR() << "Image fetcher database error when selecting body text of item ID " << newItems.at(i) << ": " << query.lastError().text();
            }

            if (query.next())
                body = cacheImages(query.value(0).toString(), newItems.at(i), imgHandling);



            if (body != "")
            {
                QSqlQuery query2;
                query2.prepare("UPDATE items SET body = :body WHERE id = :id;");
                query2.bindValue(":body", body);
                query2.bindValue(":id", newItems.at(i));
                if (!query2.exec()) {
                    QLOG_ERROR() << "Image fetcher database error while writing new body text for item ID " << newItems.at(i) << " to DB: " << query2.lastError().text();
                }
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

        QLOG_INFO() << "Image fetcher: finished.";
    }

    if (!deleted.isEmpty())
        deleteCachedImages(deleted);
}



QString OcImageFetcher::cacheImages(const QString &bodyText, int id, int imageHandling)
{
    QLOG_INFO() << "Image fetcher: Analyzing body text of item ID " << id;

    QString newBodyText = bodyText;
    QRegExp findImg("<img[^>]*>");
    QStringList foundImages;
    int pos = 0;

    while((pos = findImg.indexIn(bodyText, pos)) != -1)
    {
        foundImages << findImg.cap(0);
        pos += findImg.matchedLength();
    }

    QLOG_DEBUG() << "Image fetcher: found images: " << foundImages;

    if (!foundImages.isEmpty())
    {
        QSqlQuery query;

        for (int i = 0; i < foundImages.size(); ++i)
        {
            QVariantMap imageInfo;

            imageInfo = extractImgData(foundImages.at(i));

            if (imageInfo["tracker"].toBool())
            {
                newBodyText.replace(foundImages.at(i), "");
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
                newBodyText.remove(QRegularExpression("<a[^>]*>\\s*</a>"));
#else
                newBodyText.remove(QRegExp("<a[^>]*>\\s*</a>"));
#endif

                QLOG_DEBUG() << "Image fetcher: Removing possible tracker image";

            }
            else
            {

                if (imageHandling == 2)
                {

                    QUrl fileUrl(imageInfo["src"].toString());

                    QLOG_DEBUG() << "Image fetcher: Start to download image: " << imageInfo["src"].toString();

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

                            imageInfo["tracker"] = bool(image.width() == 1 && image.height() == 1);

                            if (imageInfo["tracker"].toBool())
                            {
                                newBodyText.replace(foundImages.at(i), "");
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
                                newBodyText.remove(QRegularExpression("<a[^>]*>\\s*</a>"));
#else
                                newBodyText.remove(QRegExp("<a[^>]*>\\s*</a>"));
#endif

                                QLOG_DEBUG() << "Removing possible tracker image.";
                            }
                            else
                            {


                                imageInfo["width"] = image.width();
                                imageInfo["height"] = image.height();

                                QString fileName("_image_");
                                fileName.prepend(QString::number(id));
                                fileName.append(QString::number(i)).append(".");

                                QString storagePath(QDir::homePath());
                                storagePath.append(IMAGE_CACHE);
                                storagePath.append(QDir::separator()).append(fileName);
                                storagePath = QDir::toNativeSeparators(storagePath);

                                storagePath.append(getFileTypeSuffix(recData));

                                QLOG_DEBUG() << "Image fetcher: image storage path: " << storagePath;

                                QString oldSrc = imageInfo["src"].toString();
                                QString newSrc = storagePath;

                                oldSrc.prepend("src=\"");
                                newSrc.prepend("src=\"");

                                QLOG_DEBUG() << "Image fetcher: image size: " << imageInfo["width"].toInt() << " x " << imageInfo["height"].toInt();

                                if (image.save(storagePath))
                                {
                                    newBodyText.replace(oldSrc, newSrc, Qt::CaseSensitive);
                                    imageInfo["src"] = storagePath;
                                    replyGetImage->deleteLater();
                                }
                                else
                                {
                                    QLOG_WARN() << "Image fetcher: try to save image with QFile method.";

                                    QFile file(storagePath);
                                    file.open(QIODevice::WriteOnly);

                                    if (file.write(recData) != -1)
                                    {

                                        newBodyText.replace(oldSrc, newSrc, Qt::CaseSensitive);
                                        imageInfo["src"] = storagePath;
                                        replyGetImage->deleteLater();

                                    } else {

                                        QLOG_ERROR() << "Image fetcher: failed to save image.";

                                        replyGetImage->deleteLater();
                                    }

                                    file.close();

                                }
                            }

                        }
                        else
                        {
                            QLOG_ERROR() << "Image fetcher: cann not download image.";

                            replyGetImage->deleteLater();
                        }
                    }
                    else
                    {
                        QLOG_ERROR() << "Image fetcher: cann not download image.";

                        replyGetImage->deleteLater();
                    }
                }

                if (!imageInfo["tracker"].toBool()) {
                    query.prepare("INSERT INTO images (parentId, path, width, height) VALUES (:parentId, :path, :width, :height);");
                    query.bindValue(":parentId", id);
                    query.bindValue(":path", imageInfo["src"]);
                    query.bindValue(":width", imageInfo["width"]);
                    query.bindValue(":height", imageInfo["height"]);
                    if (!query.exec()) {
                        QLOG_ERROR() << "Image fetcher: Database error while creating new entry in image database: " << query.lastError().text();
                    }
                }
            }
        }
    }

    return newBodyText;
}


QVariantMap OcImageFetcher::extractImgData(const QString &imgStr)
{
    QVariantMap result;

    QRegExp findSrc("(http|ftp)[^\"]*");
    imgStr.contains(findSrc);
    result["src"] =  findSrc.cap(0);

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

    int width = widthString.toInt();
    int height = heightString.toInt();

    result["width"] = width;
    result["height"] = height;
    result["tracker"] = bool(width == 1 && height == 1);

    QLOG_DEBUG() << "Image fetcher: image information: " << result;

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

                QLOG_DEBUG() << "Image fetcher: deleting image: " << imagesToDelete.at(f);

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

    if (magic.contains("474946383961") || magic.contains("474946383761")) {
        result = "gif";
    } else if (magic.contains("FFD8FF")) {
        result = "jpg";
    } else if (magic.contains("89504E470D0A1A0A")) {
        result = "png";
    } else {
        result = "";
    }

    QLOG_DEBUG() << "Image fetcher: try to get suffix from: " << magic << " - Result: " << result;

    return result;
}



int OcImageFetcher::isFetchImagesRunning()
{
    QLOG_INFO() << "Items is fetching images: " << itemsToFetchImages;

    return itemsToFetchImages;
}


void OcImageFetcher::databaseCleaned()
{
    QDir imageCache(QDir::homePath().append(IMAGE_CACHE));

    QStringList imgs = imageCache.entryList(QDir::Files);

    for (int i = 0; i < imgs.size(); ++i) {
        imageCache.remove(imgs.at(i));
    }

}
