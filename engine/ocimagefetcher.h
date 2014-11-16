#ifndef OCIMAGEFETCHER_H
#define OCIMAGEFETCHER_H

#include <QObject>
#include <QSqlQuery>
#include <QSqlError>
#include <QImage>
#include <QEventLoop>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QFile>
#include <QTimer>
#include <QStringList>

#if defined(MEEGO_EDITION_HARMATTAN)
#include <TransferUI/Client>
#include <TransferUI/Transfer>
#endif

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
#include <QRegularExpression>
#endif

#include "../common/globals.h"
#include "occonfiguration.h"
#include "ocnetwork.h"

class OcImageFetcher : public QObject
{
    Q_OBJECT
public:
    explicit OcImageFetcher(QObject *parent = 0);

public Q_SLOTS: // METHODS
    int isFetchImagesRunning();

Q_SIGNALS: // SIGNALS
    void startedFetchingImages(const int &numberOfItems);
    void finishedFetchingImages(const int &numberOfItems);
    void fetchingImages(const int &currentItem);

public slots:
    void fetchImages(const QList<int> &updated, const QList<int> &newItems, const QList<int> &deleted);
    void databaseCleaned();

private:
    QString cacheImages(const QString &bodyText, int id, int imageHandling);
    QVariantMap extractImgData(const QString &imgStr);
    void deleteCachedImages(const QList<int> &idsToDelte);
    int itemsToFetchImages;
    QString getFileTypeSuffix(const QByteArray &data);

    OcConfiguration config;
    OcNetwork network;

#if defined(MEEGO_EDITION_HARMATTAN)
    TransferUI::Client *transferClient;
    TransferUI::Transfer *transferItem;
#endif

};

#endif // OCIMAGEFETCHER_H
