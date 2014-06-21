#include "ocupdater.h"
#include <QDebug>

#define TIMER_DELTA 90
#define MINIMUM_BATTERY 20


/*!
 * \class OcUpdater
 * \brief The OcUpdater class handles full updates of the ownCloud News content
 *
 * This class contains functions for updating/synchronizing the content of the
 * ownCloud News app to the ocNews local database. It defines a timer for automatic
 * background updates.
*/

OcUpdater::OcUpdater(QObject *parent) :
    QObject(parent)
{
    updateRunning = false;

#if defined(MEEGO_EDITION_HARMATTAN)
    transferClient = new TransferUI::Client();

    if(!transferClient->init()) {
        qDebug()<<"Cannot initialize TUIClient";//error
        delete transferClient;
    }

    networkInfo = new QSystemNetworkInfo();
    batteryInfo = new QSystemBatteryInfo();
    timer = new QSystemAlignedTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(startUpdateTimed()));
    timer->setMinimumInterval(0);
    timer->setMaximumInterval(config.getSetting(QString("update/interval"), QDBusVariant(3600)).variant().toInt() + TIMER_DELTA);
    timer->start();

#ifdef QT_DEBUG
    qDebug() << "Initial update minimum interval: " << timer->minimumInterval();
    qDebug() << "Initial update maximum interval: " << timer->maximumInterval();
#endif

    connect(batteryInfo, SIGNAL(chargingStateChanged(QSystemBatteryInfo::ChargingState)), this, SLOT(handleNetAndConfChanges()));
    connect(networkInfo, SIGNAL(networkStatusChanged(QSystemNetworkInfo::NetworkMode,QSystemNetworkInfo::NetworkStatus)), this, SLOT(handleNetAndConfChanges()));
#else
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(startUpdateTimed()));
    timer->setInterval(config.getSetting(QString("update/interval"), QDBusVariant(3600)).variant().toInt() * 1000);
    timer->setTimerType(Qt::CoarseTimer);
    timer->start();


    connect(&network, SIGNAL(networkOnline()), this, SLOT(handleNetAndConfChanges()));
    connect(&network, SIGNAL(networkConfigChanged()), this, SLOT(handleNetAndConfChanges()));

    // after startup, check if an update should be performed
//    handleNetAndConfChanges();  // is currently anywayy called by network online check
#endif

}




/*!
 * \fn void OcUpdater::handleNetAndConfChanges()
 * \brief Is called after a change in the network connection
 *
 * This internal slot is connected to the networkStatusChanged signal of QSystemNetworkInfo
 * and the savedConfig signal of OcConfiguration to check, if an update should be performed
 * after something changed in the network or application config.
 */

void OcUpdater::handleNetAndConfChanges()
{

#if defined(MEEGO_EDITION_HARMATTAN)
#ifdef QT_DEBUG
    qDebug() << "Networkmode changed to: " << networkInfo->currentMode();
#endif
#endif

    if (!updateRunning) {

        QDateTime ts;

        uint lastFullUpdate = config.getSetting(QString("storage/lastFullUpdate"), QDBusVariant(0)).variant().toUInt();
        uint currentTime = ts.currentDateTimeUtc().toTime_t();

        uint timeDiff = currentTime - lastFullUpdate;
        uint triggerTime = config.getSetting(QString("update/interval"), QDBusVariant(3600)).variant().toUInt();

#ifdef QT_DEBUG
    qDebug() << "Last full update: " << lastFullUpdate;
    qDebug() << "Current time: " << currentTime;
    qDebug() << "Time difference: " << timeDiff;
    qDebug() << "Trigger time: " << triggerTime;
#endif

        if (timeDiff >= triggerTime)
        {

#if defined(MEEGO_EDITION_HARMATTAN)
            timer->wokeUp();
#else
            timer->start();
#endif

            startUpdateTimed();
        }
    }

}





/*!
 * \fn void OcUpdater::startUpdateTimed()
 * \brief Starts a timed update
 *
 * This function is triggered by the timer and starts a timed update.
 * Before starting the update, it checks if the current settings and environment
 * permitting an automated update.
 */

void OcUpdater::startUpdateTimed()
{
    int updateBehavior = config.getSetting(QString("update/behavior"), QDBusVariant(0)).variant().toInt();

#if defined(MEEGO_EDITION_HARMATTAN)

    int interval = config.getSetting(QString("update/interval"), QDBusVariant(3600)).variant().toInt();

    if (timer->minimumInterval() != interval - TIMER_DELTA)
        timer->setMinimumInterval(interval - TIMER_DELTA);

    if (timer->maximumInterval() != interval + TIMER_DELTA)
        timer->setMaximumInterval(interval + TIMER_DELTA);

#ifdef QT_DEBUG
    qDebug() << "Settings before timed update start:";
    qDebug() << "-----------------------------------";
    qDebug() << "Update behavior: " << updateBehavior;
    qDebug() << "Mimimum interval: " << timer->minimumInterval();
    qDebug() << "Maximum interval: " << timer->maximumInterval();
    qDebug() << "Battery load in %: " << batteryInfo->remainingCapacityPercent();
    qDebug() << "Battery charging state: " << batteryInfo->chargingState();
    qDebug() << "Network mode: " << networkInfo->currentMode();
    qDebug() << "Network status: " << networkInfo->networkStatus(networkInfo->currentMode());
#endif

    if (batteryInfo->remainingCapacityPercent() >= MINIMUM_BATTERY || batteryInfo->chargingState() == QSystemBatteryInfo::Charging )
    {
        if ((updateBehavior == 1 && networkInfo->currentMode() == QSystemNetworkInfo::WlanMode) || updateBehavior == 2)
        {
            if (config.isConfigSet() && config.isAccountEnabled())
            {
                switch(networkInfo->networkStatus(networkInfo->currentMode()))
                {
                    case QSystemNetworkInfo::Connected:
                    case QSystemNetworkInfo::Busy:
                    case QSystemNetworkInfo::HomeNetwork:
                                                startUpdatePrivate();
                                                break;
                    default:
                        qDebug() << "No appropriate Network available.";
                }
            }
        }
    }

#else
// currentely not as API on Sailfish available
// TODO: implement complete features from Harmattan
//    networkInfo = new QNetworkInfo();
//    batteryInfo = new QBatteryInfo();

    int interval = config.getSetting(QString("update/interval"), QDBusVariant(3600)).variant().toInt() * 1000;

    if (timer->interval() != interval)
        timer->setInterval(interval);

    if ((updateBehavior == 1 && network.bearerType() == 1) || updateBehavior == 2)
    {
        if (config.isConfigSet() && config.isAccountEnabled())
        {
            startUpdatePrivate();
        }
    }

#endif


}



/*!
 * \fn void OcUpdater::startUpdate()
 * \brief Starts a manually triggered update
 *
 * This function starts a manually triggered update and resets the update timer.
 */

void OcUpdater::startUpdate()
{

#if defined(MEEGO_EDITION_HARMATTAN)
    int intervall = config.getSetting(QString("update/interval"), QDBusVariant(3600)).variant().toInt();
    if (timer->minimumInterval() != intervall - TIMER_DELTA)
        timer->setMinimumInterval(intervall - TIMER_DELTA);

    if (timer->maximumInterval() != intervall + TIMER_DELTA)
        timer->setMaximumInterval(intervall + TIMER_DELTA);
#else
    int intervall = config.getSetting(QString("update/interval"), QDBusVariant(3600)).variant().toInt() * 1000;

    if (timer->interval() != intervall)
        timer->setInterval(intervall);
#endif

    if (config.isConfigSet() && config.isAccountEnabled())
    {

#if defined(MEEGO_EDITION_HARMATTAN)
        timer->wokeUp();
#else
        timer->start();
#endif

        startUpdatePrivate();
    } else {
        emit updateError(tr("Your account is disabled or you have not created an account yet."));
    }
}





/*!
 * \fn void OcUpdater::startUpdatePrivate()
 * \brief Starts the update process
 *
 * This function starts the update process, creates transfer UI items, and requests the folders.
 */

void OcUpdater::startUpdatePrivate()
{
    if (network.isFlightMode() || network.networkAccessible() == QNetworkAccessManager::NotAccessible)
    {
        emit updateFinished();
    } else {

        updateRunning = true;
        inOperation = true;

#ifdef QT_DEBUG
        qDebug() << "Start Update";
        qDebug() << "Updating Folders";
#endif


#if defined(MEEGO_EDITION_HARMATTAN)
        transferItem = transferClient->registerTransfer(tr("Synchronizing ownCloud News"), TransferUI::Client::TRANSFER_TYPES_SYNC);
        transferItem->waitForCommit();
        transferItem->setTargetName(config.getSetting(QString("server/domain"), QDBusVariant("")).variant().toString());
        transferItem->setName(tr("Updating Folders"));
        transferItem->setSize(0);
        transferItem->setCanPause(false);
        transferItem->setIcon("icon-m-ocnews");
        transferItem->setActive(.05);
        transferItem->commit();
#else

#endif
        emit updateStarted();
    }
}



/*!
 * \fn void OcUpdater::updateFeeds()
 * \brief Performs the feed updates
 *
 * This function updates the feeds and updates the transfer UI
 */

void OcUpdater::updateFeeds()
{
    if (updateRunning)
    {

#ifdef QT_DEBUG
        qDebug() << "Updating Feeds";
#endif

#if defined(MEEGO_EDITION_HARMATTAN)
        transferItem->setProgress(.33);
        transferItem->setName(tr("Updating Feeds"));
#endif
        emit startRequestFeeds();
    }
}




/*!
 * \fn void OcUpdater::updateItems()
 * \brief Performs the item updates
 *
 * This function updates the items and updates the transfer UI
 */

void OcUpdater::updateItems()
{
    if (updateRunning)
    {

#ifdef QT_DEBUG
        qDebug() << "Updating Items";
#endif

#if defined(MEEGO_EDITION_HARMATTAN)
        transferItem->setProgress(.66);
        transferItem->setName(tr("Updating Items"));
#endif

        QSqlQuery query;
        query.exec("SELECT id FROM items LIMIT 1;");
        if (query.next())
        {
            if (query.value(0).toString() != "")
            {
                emit startUpdateItems("0", "3", "0");
            } else {
                emit startRequestItems("1000", "0", "3", "0", "true");
            }
        } else {
            emit startRequestItems("1000", "0", "3", "0", "true");
        }
    }
}




/*!
 * \fn void OcUpdater::endUpdate()
 * \brief End point of the update process
 *
 * This function performs the end of the update by emitting the signal
 * updateFinished() and cleaning the transfer UI items.
 */

void OcUpdater::endUpdate()
{
    if (updateRunning)
    {

#if defined(MEEGO_EDITION_HARMATTAN)
        transferItem->setProgress(1.0);
//        transferItem->setName(tr("Update Finished"));
        transferItem->markDone(tr("Update Finished"));
        transferClient->removeTransfer(transferItem->transferId());
        delete transferItem;
        transferItem = 0;
#else

#endif

        updateRunning = false;
        inOperation = false;
        emit updateFinished();
    }
}



/*!
 * \fn void OcUpdater::errorInUpdate(QString errorMessage)
 * \brief Handles errors in the update process
 *
 * This slot handles errors in the update process, emits a signal that contains the
 * error message and updates the transfer UI accordingly.
 *
 * \param errorMessage      The error occured during the update process
 */

void OcUpdater::errorInUpdate(QString errorMessage)
{
    if (updateRunning)
    {

#if defined(MEEGO_EDITION_HARMATTAN)
        transferItem->markFailure(tr("Update Failed"), errorMessage);
#else

#endif

        updateRunning = false;
        inOperation = false;
        emit updateError(errorMessage);
    }
}




/*!
 * \fn bool OcUpdater::isUpdateRunning()
 * \brief Returns true if update is running
 *
 * This function checks if an update is running and returns true if one is running.
 *
 * \return bool true if update is running
 */

bool OcUpdater::isUpdateRunning()
{
    return updateRunning;
}
