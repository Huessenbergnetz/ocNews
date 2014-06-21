#include "ocdbusupdater.h"

OcDBusUpdater::OcDBusUpdater(QObject *parent) :
    QObject(parent)
{
    updater = new de::buschmann23::ocNewsEngine::Updater("de.buschmann23.ocNewsEngine", "/Updater", QDBusConnection::sessionBus(), this);
    connect(updater, SIGNAL(updateError(QString)), this, SLOT(dbusUpdateError(QString)));
    connect(updater, SIGNAL(updateFinished()), this, SLOT(dbusUpdateFinished()));
    connect(updater, SIGNAL(updateStarted()), this, SLOT(dbusUpdateStarted()));
}

void OcDBusUpdater::startUpdate()
{
    updater->startUpdate();
}

bool OcDBusUpdater::isUpdateRunning()
{
    return updater->isUpdateRunning();
}

void OcDBusUpdater::dbusUpdateError(QString updateErrorMessage)
{
    emit updateError(updateErrorMessage);
}

void OcDBusUpdater::dbusUpdateFinished()
{
    emit updateFinished();
}

void OcDBusUpdater::dbusUpdateStarted()
{
    emit updateStarted();
}
