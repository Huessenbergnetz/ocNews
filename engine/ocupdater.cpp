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

    m_updateBehavior = config.value("update/behavior", 0).toInt();

    m_interval = config.value("update/interval", 3600).toInt();

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
    if (m_updateBehavior != 0) {
        timer->setMinimumInterval(0);
        timer->setMaximumInterval(m_interval + TIMER_DELTA);
        timer->start();
    }

#ifdef QT_DEBUG
    qDebug() << "Initial update minimum interval: " << timer->minimumInterval();
    qDebug() << "Initial update maximum interval: " << timer->maximumInterval();
#endif

    connect(batteryInfo, SIGNAL(chargingStateChanged(QSystemBatteryInfo::ChargingState)), this, SLOT(handleNetAndConfChanges()));
    connect(networkInfo, SIGNAL(networkStatusChanged(QSystemNetworkInfo::NetworkMode,QSystemNetworkInfo::NetworkStatus)), this, SLOT(handleNetAndConfChanges()));
#else
    m_timer = new BackgroundActivity(this);
    connect(m_timer, SIGNAL(running()), this, SLOT(startUpdateTimed()));
    if (m_updateBehavior != 0)
        m_timer->wait(m_interval - TIMER_DELTA, m_interval + TIMER_DELTA);

#ifdef QT_DEBUG
    qDebug() << "Initial update minimum interval: " << m_interval - TIMER_DELTA;
    qDebug() << "Initial update maximum interval: " << m_interval + TIMER_DELTA;
#endif

    connect(&network, SIGNAL(networkOnline()), this, SLOT(handleNetAndConfChanges()));
    connect(&network, SIGNAL(networkConfigChanged()), this, SLOT(handleNetAndConfChanges()));

#endif
}




/*!
 * \fn void OcUpdater::handleNetAndConfChanges()
 * \brief Is called after a change in the network connection
 *
 * This internal slot is connected to the networkStatusChanged signal of QSystemNetworkInfo
 * and two signals of OcConfiguration to check, if an update should be performed
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

        uint lastFullUpdate = config.value("storage/lastFullUpdate", 0).toUInt();
        uint currentTime = ts.currentDateTimeUtc().toTime_t();

        uint timeDiff = currentTime - lastFullUpdate;
        uint triggerTime = config.value("update/interval", 3600).toUInt();

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
            m_timer->stop();
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
#if defined(MEEGO_EDITION_HARMATTAN)

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
        if ((m_updateBehavior == 1 && networkInfo->currentMode() == QSystemNetworkInfo::WlanMode) || m_updateBehavior == 2)
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

    if ((m_updateBehavior == 1 && network.bearerType() == 1) || m_updateBehavior == 2)
    {
        if (config.isConfigSet() && config.isAccountEnabled())
        {
            startUpdatePrivate();
        } else {
            if (m_updateBehavior != 0)
                m_timer->wait(m_interval - TIMER_DELTA, m_interval + TIMER_DELTA);
        }
    } else {
        if (m_updateBehavior != 0)
            m_timer->wait(m_interval - TIMER_DELTA, m_interval + TIMER_DELTA);
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
    if (config.isConfigSet() && config.isAccountEnabled())
    {

#if defined(MEEGO_EDITION_HARMATTAN)
        timer->wokeUp();
#else
        m_timer->stop();
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
        transferItem->setTargetName(config.value("server/domain", "").toString());
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
        if (m_updateBehavior != 0)
            m_timer->wait(m_interval - TIMER_DELTA, m_interval + TIMER_DELTA);
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
        if (m_updateBehavior != 0)
            m_timer->wait(m_interval - TIMER_DELTA, m_interval + TIMER_DELTA);
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




/*!
 * \fn void OcUpdater::setUpdateBehavior(const int &nBehavior)
 * \brief Handles changes in the update behavior configuration
 *
 * If the new update behavior is not 0 (means manual update only)
 * it calls handleNetAndConfChanges to perform an update if appropriate.
 *
 * \param nBehavior     The new behavior
 */
void OcUpdater::setUpdateBehavior(const int &nBehavior)
{
    if (nBehavior != m_updateBehavior) {
        int oldBehavior = m_updateBehavior;
        m_updateBehavior = nBehavior;

        if (oldBehavior == 0 && nBehavior != 0) {

#if defined(MEEGO_EDITION_HARMATTAN)
            timer->setMinimumInterval(m_interval - TIMER_DELTA);
            timer->setMaximumInterval(m_interval + TIMER_DELTA);
            timer->start();
#else
            m_timer->wait(m_interval - TIMER_DELTA, m_interval + TIMER_DELTA);
#endif

        } else if (oldBehavior != 0 && nBehavior == 0) {

#if defined(MEEGO_EDITION_HARMATTAN)
            timer->stop();
#else
            m_timer->stop();
#endif
        }

        if (m_updateBehavior != 0)
            handleNetAndConfChanges();
    }
}



/*!
 * \fn void OcUpdater::setUpdateInterval(const int &nInterval)
 * \fn Handles changes in the update interval configuration
 *
 * If the new update interval is different it sets the new interval
 * to the timer. If the update behavior is not 0 (means manual update only)
 * it calls handleNetAndConfChanges to perform an update ir appropriate.
 *
 * \param nInterval
 */
void OcUpdater::setUpdateInterval(const int &nInterval)
{
    if (nInterval != m_interval) {
        m_interval = nInterval;
#if defined(MEEGO_EDITION_HARMATTAN)
        timer->setMinimumInterval(m_interval - TIMER_DELTA);
        timer->setMaximumInterval(m_interval + TIMER_DELTA);
#else
        m_timer->stop();
        m_timer->wait(m_interval - TIMER_DELTA, m_interval + TIMER_DELTA);
#endif
        if (m_updateBehavior != 0)
            handleNetAndConfChanges();
    }
}
