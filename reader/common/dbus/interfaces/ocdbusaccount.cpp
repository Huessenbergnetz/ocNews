#include "ocdbusaccount.h"

OcDBusAccount::OcDBusAccount(QObject *parent) :
    QObject(parent)
{
    account = new de::buschmann23::ocNewsEngine::Account("de.buschmann23.ocNewsEngine", "/Account", QDBusConnection::sessionBus(), this);
}

void OcDBusAccount::create()
{
    account->create();
}

void OcDBusAccount::edit()
{
    account->edit();
}
