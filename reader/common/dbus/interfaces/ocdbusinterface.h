#ifndef OCDBUSINTERFACE_H
#define OCDBUSINTERFACE_H

#include <QObject>
#include "ocgenericif.h"
#include "occonfigif.h"

class OcDBusInterface : public QObject
{
    Q_OBJECT
public:
    explicit OcDBusInterface(QObject *parent = 0);
    // generic
    Q_INVOKABLE void quitEngine();
    // configuration
    Q_INVOKABLE void getConfig();
    Q_INVOKABLE QVariant getSetting(QString entry, QVariant defaultValue = "");
    Q_INVOKABLE void setSetting(QString entry, QVariant value);
    Q_INVOKABLE void resetConfig();
    Q_INVOKABLE void cleanDatabase();
    Q_INVOKABLE void getStatistics();
    Q_INVOKABLE void cleanCertificates();
    Q_INVOKABLE bool isConfigSet();
    Q_INVOKABLE bool isAccountEnabled();
    Q_INVOKABLE QVariant getStat(int stat);
    Q_INVOKABLE void removeAllAccounts();
    
signals:
    // generic
    void gotVersion(QString version);
    void gotVersionError(const QString &error);
    void initError(const QString &errorMessage);
    void initErrorFlightMode();
    void initSuccess();
    // configuration
    void gotConfig(const QVariantMap &config);
    void cleanedDatabase();
    void gotStatistics(const QVariantMap &stats);
    void cleanedCertificates();
    void changedConfig();
    void configReset();
    void removedAllAccounts();
    
public slots:
    // generic
    void getVersion();

private slots:
    // generic
    void dbusGotVersion(const QString &version);
    void dbusGotVersionError(const QString &error);
    void dbusInitError(const QString &errorMessage);
    void dbusInitErrorFlightMode();
    void dbusInitSuccess();
    // configuration
    void dbusGotConfig(const QVariantMap &config);
    void dbusCleanedDatabase();
    void dbusGotStatistics(const QVariantMap &stats);
    void dbusCleanedCertificates();
    void dbusChangedConfig();
    void dbusConfigReset();
    void dbusRemovedAllAccounts();
    
private:
    de::buschmann23::ocNewsEngine::Generic *generic;
    de::buschmann23::ocNewsEngine::Configuration *configuration;
};

#endif // OCDBUSINTERFACE_H
