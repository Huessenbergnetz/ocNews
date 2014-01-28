#ifndef OCCONFIGURATION_H
#define OCCONFIGURATION_H

#include <QObject>
#include <QDir>
#include <QSettings>
#include <QSqlQuery>
#include <QDateTime>
#include <QtDBus/QDBusVariant>
#if defined(MEEGO_EDITION_HARMATTAN)
#include <accounts-qt/Accounts/Account>
#include <accounts-qt/Accounts/Manager>
#include <aegis_certman.h>
#endif
#include "../common/globals.h"

class OcConfiguration : public QObject
{
    Q_OBJECT
public:
    explicit OcConfiguration(QObject *parent = 0);
    QVariantMap getAccount();

public Q_SLOTS: // METHODS
    void setSetting(const QString &entry, const QDBusVariant &value);
    QDBusVariant getSetting(const QString &entry, const QDBusVariant &defaultValue);
    QVariantMap getConfig();
    void resetConfig();
    void saveConfig(const QVariantMap &config);
    void cleanDatabase();
    QVariantMap getStatistics();
    QDBusVariant getStat(const int stat);
    void cleanCertificates();
    bool isConfigSet();
    bool isAccountEnabled();
    
signals:

Q_SIGNALS: // SIGNALS
    void gotConfig(const QVariantMap &config);
    void savedConfig();
    void cleanedDatabase();
    void gotStatistics(const QVariantMap &stats);
    void cleanedCertificates();
    void changedConfig();

    
public slots:

private:
    QSettings settings;
#if defined(MEEGO_EDITION_HARMATTAN)
    Accounts::Account *account;
    Accounts::Manager *accMan;
#endif
    
};

#endif // OCCONFIGURATION_H
