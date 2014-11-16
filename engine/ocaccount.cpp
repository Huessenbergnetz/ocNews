#include "ocaccount.h"

OcAccount::OcAccount(QObject *parent) :
    QObject(parent)
{
    proxy = new AccountSetup::ProviderPluginProxy(this);
}

void OcAccount::create()
{
//    proxy = new AccountSetup::ProviderPluginProxy;

    manager = new Accounts::Manager();

    provider = manager->provider("ocnews");

    proxy->createAccount(provider, "");
}

void OcAccount::edit()
{
//    proxy = new AccountSetup::ProviderPluginProxy;

    manager = new Accounts::Manager();

    Accounts::AccountId id;
    id = config.value("account/id", -1).toInt();
    account = manager->account(id);
    proxy->editAccount(account, "");
}
