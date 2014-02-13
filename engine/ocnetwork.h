#ifndef OCNETWORK_H
#define OCNETWORK_H

#include <QNetworkAccessManager>
#include <QAuthenticator>
#include <QNetworkReply>
#include <QSslError>
#include <QSslConfiguration>
#include <QSslCipher>
#include <QNetworkConfiguration>

#if defined(MEEGO_EDITION_HARMATTAN)
#include <qmsystem2/qmdevicemode.h>
#include <aegis_certman.h>
#else
#include <QNetworkConfigurationManager>
//#include <qmsystem2-qt5/qmdevicemode.h>
#endif

#include "occonfiguration.h"

class OcNetwork : public QNetworkAccessManager
{
    Q_OBJECT
public:
    explicit OcNetwork(QObject *parent = 0);
    bool isFlightMode();
    int bearerType();
    
signals:
#if !defined(MEEGO_EDITION_HARMATTAN)
    void networkOnline();
    void networkConfigChanged();
#endif
    
public slots:

private slots:
    void slotAuthenticationRequired(QNetworkReply*,QAuthenticator *);
    void sslErrorHandler(QNetworkReply *rep,const QList<QSslError> &errors);
#if !defined(MEEGO_EDITION_HARMATTAN)
    void networkStatusChanged(bool isOnline);
    void networkConfigurationChanged();
#endif

private:
    QSettings settings;
    QNetworkReply *sslReply;
#if defined(MEEGO_EDITION_HARMATTAN)
    MeeGo::QmDeviceMode *m_deviceMode;
#else
    QNetworkConfigurationManager *configurationManager;
#endif
    OcConfiguration config;
};

#endif // OCNETWORK_H
