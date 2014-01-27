#ifndef OCUPDATER_H
#define OCUPDATER_H

#include <QObject>
#include "ocnetwork.h"
#include "ocfolders.h"
#include "ocfeeds.h"
#include "ocitems.h"
#include "occonfiguration.h"

#if defined(MEEGO_EDITION_HARMATTAN)
#include <TransferUI/Client>
#include <TransferUI/Transfer>
#include <QSystemAlignedTimer>
#include <QSystemNetworkInfo>
#include <QSystemBatteryInfo>
#else
#include <QTimer>
//#include <mlite5/MNotification>
//#include <mlite5/MNotificationGroup>
//#include <QtSystemInfo/QNetworkInfo>
//#include <QtSystemInfo/QBatteryInfo>
#endif

#if defined(MEEGO_EDITION_HARMATTAN)
QTM_USE_NAMESPACE
#endif

class OcUpdater : public QObject
{
    Q_OBJECT
public:
    explicit OcUpdater(QObject *parent = 0);

public Q_SLOTS: // METHODS
    void startUpdate();
    bool isUpdateRunning();

Q_SIGNALS: // SIGNALS
    void updateError(const QString &updateErrorMessage);
    void updateFinished();
    void updateStarted();
    
public slots:
    void handleNetAndConfChanges();

private slots:
    void startUpdateTimed();
    void startUpdatePrivate();
    void updateFeeds();
    void updateItems();
    void endUpdate();
    void errorInUpdate(QString errorMessage);

private:
    OcFolders folders;
    OcFeeds feeds;
    OcItems items;
    OcNetwork network;
    OcConfiguration config;
    bool updateRunning;
#if defined(MEEGO_EDITION_HARMATTAN)
    TransferUI::Client *transferClient;
    TransferUI::Transfer *transferItem;
    QSystemAlignedTimer *timer;
    QSystemNetworkInfo *networkInfo;
    QSystemBatteryInfo *batteryInfo;
#else
    QTimer *timer;
//    MNotificationGroup *notificationGroup;
//    MNotification *notification;
//    QNetworkInfo *networkInfo;
//    QBatteryInfo *batteryInfo;
#endif
};

#endif // OCUPDATER_H
