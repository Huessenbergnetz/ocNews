#ifndef OCACCOUNT_H
#define OCACCOUNT_H

#include <QObject>

#include <AccountSetup/ProviderPluginProxy>
#include <accounts-qt/Accounts/Account>
#include <accounts-qt/Accounts/Manager>
#include <accounts-qt/Accounts/Provider>

#include "occonfiguration.h"

class OcAccount : public QObject
{
    Q_OBJECT
public:
    explicit OcAccount(QObject *parent = 0);
    Q_INVOKABLE void create();
    Q_INVOKABLE void edit();
    
signals:
    
public slots:

private:
    AccountSetup::ProviderPluginProxy *proxy;
    Accounts::Manager *manager;
    Accounts::Provider *provider;
    Accounts::Account *account;
    OcConfiguration config;
    
};

#endif // OCACCOUNT_H
