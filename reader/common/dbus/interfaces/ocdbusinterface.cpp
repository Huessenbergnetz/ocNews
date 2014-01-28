#include "ocdbusinterface.h"
#include <QDebug>

OcDBusInterface::OcDBusInterface(QObject *parent) :
    QObject(parent)
{
    generic = new de::buschmann23::ocNewsEngine::Generic("de.buschmann23.ocNewsEngine", "/", QDBusConnection::sessionBus(), this);
    connect(generic, SIGNAL(gotVersion(QString)), this, SLOT(dbusGotVersion(QString)));
    connect(generic, SIGNAL(gotVersionError(QString)), this, SLOT(dbusGotVersionError(QString)));
    connect(generic, SIGNAL(initError(QString)), this, SLOT(dbusInitError(QString)));
    connect(generic, SIGNAL(initErrorFlightMode()), this, SLOT(dbusInitErrorFlightMode()));
    connect(generic, SIGNAL(initSuccess()), this, SLOT(dbusInitSuccess()));

    configuration = new de::buschmann23::ocNewsEngine::Configuration("de.buschmann23.ocNewsEngine", "/Configuration", QDBusConnection::sessionBus(), this);
    connect(configuration, SIGNAL(gotConfig(QVariantMap)), this, SLOT(dbusGotConfig(QVariantMap)));
    connect(configuration, SIGNAL(savedConfig()), this, SLOT(dbusSavedConfig()));
    connect(configuration, SIGNAL(cleanedDatabase()), this, SLOT(dbusCleanedDatabase()));
    connect(configuration, SIGNAL(gotStatistics(QVariantMap)), this, SLOT(dbusGotStatistics(QVariantMap)));
    connect(configuration, SIGNAL(cleanedCertificates()), this, SLOT(dbusCleanedCertificates()));
    connect(configuration, SIGNAL(changedConfig()), this, SLOT(dbusChangedConfig()));
}

/*
 * Interface to OcGeneric
 */

void OcDBusInterface::getVersion()
{
    generic->getVersion();
}

void OcDBusInterface::initConnection()
{
    generic->initConnection();
}

void OcDBusInterface::quitEngine()
{
    generic->quitEngine();
}

void OcDBusInterface::dbusGotVersion(const QString &version)
{
    emit gotVersion(version);
}

void OcDBusInterface::dbusGotVersionError(const QString &error)
{
    emit gotVersionError(error);
}

void OcDBusInterface::dbusInitError(const QString &errorMessage)
{
    emit initError(errorMessage);
}

void OcDBusInterface::dbusInitErrorFlightMode()
{
    emit initErrorFlightMode();
}

void OcDBusInterface::dbusInitSuccess()
{
    emit initSuccess();
}

/*
 * Interface to OcConfiguration
 */

void OcDBusInterface::getConfig()
{
    configuration->getConfig();
}

void OcDBusInterface::saveConfig(QVariantMap config)
{
    configuration->saveConfig(config);
}

QVariant OcDBusInterface::getSetting(QString entry, QVariant defaultValue)
{
    QDBusVariant dbvDefaultValue(defaultValue);
    QDBusVariant dbvResult;
    dbvResult = configuration->getSetting(entry, dbvDefaultValue);
    QVariant result = dbvResult.variant();
    return result;
}

void OcDBusInterface::setSetting(QString entry, QVariant value)
{
    QDBusVariant dbvValue(value);
    configuration->setSetting(entry, dbvValue);
}

void OcDBusInterface::resetConfig()
{
    configuration->resetConfig();
}

void OcDBusInterface::cleanDatabase()
{
    configuration->cleanDatabase();
}

void OcDBusInterface::getStatistics()
{
    configuration->getStatistics();
}

QVariant OcDBusInterface::getStat(int stat)
{
    QDBusVariant dbvResult;
    dbvResult = configuration->getStat(stat);
    QVariant result = dbvResult.variant();
    return result;
}

void OcDBusInterface::cleanCertificates()
{
    configuration->cleanCertificates();
}

bool OcDBusInterface::isConfigSet()
{
    return configuration->isConfigSet();
}

bool OcDBusInterface::isAccountEnabled()
{
    return configuration->isAccountEnabled();
}

void OcDBusInterface::dbusGotConfig(const QVariantMap &config)
{
    emit gotConfig(config);
}

void OcDBusInterface::dbusSavedConfig()
{
    emit savedConfig();
}

void OcDBusInterface::dbusCleanedDatabase()
{
    emit cleanedDatabase();
}

void OcDBusInterface::dbusGotStatistics(const QVariantMap &stats)
{
    emit gotStatistics(stats);
}

void OcDBusInterface::dbusCleanedCertificates()
{
    emit cleanCertificates();
}

void OcDBusInterface::dbusChangedConfig()
{
    emit changedConfig();
}
