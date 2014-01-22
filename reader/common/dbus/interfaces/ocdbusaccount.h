#ifndef OCDBUSACCOUNT_H
#define OCDBUSACCOUNT_H

#include <QObject>
#include "ocaccountif.h"

class OcDBusAccount : public QObject
{
    Q_OBJECT
public:
    explicit OcDBusAccount(QObject *parent = 0);
    Q_INVOKABLE void create();
    Q_INVOKABLE void edit();
    
signals:
    
public slots:

private:
    de::buschmann23::ocNewsEngine::Account *account;
    
};

#endif // OCDBUSACCOUNT_H
