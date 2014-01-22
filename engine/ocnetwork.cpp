#include <QDebug>

#include "ocnetwork.h"
#include "../common/globals.h"


/*!
 * \class OcNetwork
 * \brief The OcNetwork class reimplements the QNetworkAccessManager
 *
 * This class reimplments the QNetworkAccessManager class and adds some functions
 * for handling authentication requests, SSL and network states.
*/

OcNetwork::OcNetwork(QObject *parent) :
    QNetworkAccessManager(parent)
{
    connect(this, SIGNAL(authenticationRequired(QNetworkReply*,QAuthenticator*)),
                 this, SLOT(slotAuthenticationRequired(QNetworkReply*,QAuthenticator*)));

    connect(this, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)),
                this, SLOT(sslErrorHandler(QNetworkReply*,QList<QSslError>)));
}




/*!
 * \fn void OcNetwork::slotAuthenticationRequired(QNetworkReply* rep, QAuthenticator *authenticator)
 * \brief Handles network authentication requests
 *
 * This slot handles the network authentication requests. It takes the login data from the settings.
 *
 * \param rep               The reply that requests the authentication
 * \param authenticator     The used QAuthenticator
 */

void OcNetwork::slotAuthenticationRequired(QNetworkReply* rep, QAuthenticator *authenticator)
{
    QVariantMap account = config.getAccount();

#ifdef QT_DEBUG
        qDebug() << "Account: " << account;
        qDebug() << "Current user: " << authenticator->user();
        qDebug() << "Current password: " << authenticator->password();
#endif

    if (account["state"].toInt() == 0)
    {
        if (authenticator->user().isEmpty() && authenticator->password().isEmpty())
        {
            authenticator->setUser(account["uname"].toString());
            authenticator->setPassword(account["pword"].toString());
        } else {
            if ((authenticator->user() != account["uname"].toString()) || (authenticator->password() !=  account["pword"].toString() ))
            {
                authenticator->setUser(account["uname"].toString());
                authenticator->setPassword(account["pword"].toString());
            } else {
                rep->abort();
                #ifdef QT_DEBUG
                qDebug() << "Abort authentication!";
                #endif
            }
        }
    } else {
        rep->abort();
    }
}




/*!
 * \fn void OcNetwork::sslErrorHandler(QNetworkReply* rep,const QList<QSslError> &errors)
 * \brief Handles SSL errors
 *
 * This slot handles the SSL erorrs produced by the network connection. On Harmattan it uses
 * aegis-certman to verify and trust not known certificates.
 *
 * \param rep           The reply that contents the SSL error information
 * \param errors        The list of SSL errors
 */

void OcNetwork::sslErrorHandler(QNetworkReply* rep,const QList<QSslError> &errors)
{
    QVariantMap account = config.getAccount();
    bool ignoreSSLerrors = false;
    if (account["state"].toInt() == 0)
    {
        ignoreSSLerrors = account["ignoresslerror"].toBool();
    }

    if (ignoreSSLerrors)
    {
        rep->ignoreSslErrors();

#ifdef QT_DEBUG
        qDebug() << "Ignore SSL-Errors";
#endif

    } else {

        #ifdef QT_DEBUG
        foreach (const QSslError &error, errors)
        {
            qDebug() << error.errorString();
        }
        #endif

        // get certificate checksum
        QString checksum = QString::fromLatin1(rep->sslConfiguration().peerCertificateChain().last().digest(QCryptographicHash::Md5).toHex().toLower());

        // set string for temporary file path
        QString filePath(QDir::homePath());
        filePath.append(BASE_PATH).append(QDir::separator()).append(checksum).append(".der");

        // store server certificate temporary
        QFile x509Temp(filePath);
        x509Temp.open(QIODevice::WriteOnly);
        x509Temp.write(rep->sslConfiguration().peerCertificateChain().last().toDer());
        x509Temp.close();


#if defined(MEEGO_EDITION_HARMATTAN)

        // set credential
        int credSuc = aegis_certman_set_credentials("buschtrommel-ocnews::CertOCNewsSSL");
        if (credSuc != 0) qDebug() << "set credential error: " << credSuc;

        // open file for X509 struct
        FILE * crtFile;
        crtFile = fopen(filePath.toAscii().data(), "r");
        if (crtFile == NULL) qDebug() << "Can not open cert file.";

        X509 * crt;
        crt = d2i_X509_fp(crtFile, NULL);

        if (crt == NULL) qDebug() << "Error importing X509 Certificate";

        // get server key id
        aegis_key_id crtKeyId;
        aegis_certman_get_key_id(crt, crtKeyId);

        // open ssl domain
        domain_handle ownDomain;
        int openCheck = aegis_certman_open_domain("ssl-ocnews", AEGIS_CERTMAN_DOMAIN_PRIVATE, &ownDomain);
        if (openCheck != 0) qDebug() << "Error Opening Domain: " << openCheck;

        int guiCheck = aegis_certman_gui_check_certificate(crt, 120);
        qDebug() << "Certcheck: " << guiCheck;
        if (guiCheck == 0)
        {
            // check if cert is already in domain
            X509 * storedCert;
            int loadStoredCert = aegis_certman_load_cert(ownDomain, crtKeyId, &storedCert);

            if (loadStoredCert == 0 && storedCert != NULL)
            {
                qDebug() << "Load Cert: " << loadStoredCert;

                // convert internal X509 structure to DER
                int len;
                unsigned char *buf;

                buf = NULL;

                len = i2d_X509(storedCert, &buf);

                if (len > 0) {
                    // create Qt Certificate from buffer
                    QByteArray buffer(reinterpret_cast<const char*>(buf), len);
                    QSslCertificate sslCert(buffer, QSsl::Der);

                    // create list and append cert
                    QList<QSslCertificate> sslCerts;
                    sslCerts.append(sslCert);

                    // put ssl into ssl error for ignored errors
                    QSslError sslError(QSslError::SelfSignedCertificate, sslCerts.at(0));
                    QList<QSslError> expectedSslErrors;
                    expectedSslErrors.append(sslError);

                    // add certificate to socket and current configuration
                    QSslSocket::addDefaultCaCertificates(sslCerts);
                    QList<QSslCipher> ciphers = rep->sslConfiguration().ciphers();
                    QSslConfiguration sslConfig;
                    sslConfig.setCiphers(ciphers);
                    sslConfig.setCaCertificates(sslCerts);
                    rep->setSslConfiguration(sslConfig);

                    // ignore only these ssl error
                    rep->ignoreSslErrors(expectedSslErrors);
                } else {
                    qDebug() << "Can not decode cert to DER.";
                }
            } else {
                int addCheck = aegis_certman_add_cert(ownDomain, crt);
                qDebug() << "Add Cert: " << addCheck;


                // convert internal X509 structure to DER
                int len;
                unsigned char *buf;

                buf = NULL;

                len = i2d_X509(crt, &buf);

                if (len > 0) {
                    // create Qt Certificate from buffer
                    QByteArray buffer(reinterpret_cast<const char*>(buf), len);
                    QSslCertificate sslCert(buffer, QSsl::Der);

                    // create list and append cert
                    QList<QSslCertificate> sslCerts;
                    sslCerts.append(sslCert);

                    // put ssl into ssl error for ignored errors
                    QSslError sslError(QSslError::SelfSignedCertificate, sslCerts.at(0));
                    QList<QSslError> expectedSslErrors;
                    expectedSslErrors.append(sslError);

                    // add certificate to socket and current configuration
                    QSslSocket::addDefaultCaCertificates(sslCerts);
                    QList<QSslCipher> ciphers = rep->sslConfiguration().ciphers();
                    QSslConfiguration sslConfig;
                    sslConfig.setCiphers(ciphers);
                    sslConfig.setCaCertificates(sslCerts);
                    rep->setSslConfiguration(sslConfig);

                    // ignore only these ssl error
                    rep->ignoreSslErrors(expectedSslErrors);
                } else {
                    qDebug() << "Can not decode cert to DER.";
                }
            }
        } else {
            // remove cert if not approved
            int removeCheck = aegis_certman_rm_cert(ownDomain, crtKeyId);
            qDebug() << "Remove Cert: " << removeCheck;
        }

        aegis_certman_close_domain(ownDomain);
        x509Temp.remove(); // remove temporary cert file

#else

//        rep->ignoreSslErrors();
        rep->abort();
        qDebug() << "Abort network operation...";

#endif
    }
}




/*!
 * \fn bool OcNetwork::isFlightMode()
 * \brief Check if device is in flight mode
 *
 * This function returns true if the device is in flight mode. Currently
 * this only works on Harmattan. On Sailfish OS it returns false by default.
 *
 * \return      true if device is in flight mode, otherwise false
 */

bool OcNetwork::isFlightMode()
{
    bool flightMode = false;

#if defined(MEEGO_EDITION_HARMATTAN)
    m_deviceMode = new MeeGo::QmDeviceMode();
    MeeGo::QmDeviceMode::DeviceMode mode = m_deviceMode->getMode();

    switch (mode) {
        case MeeGo::QmDeviceMode::Normal:
            flightMode = false;
            break;
        case MeeGo::QmDeviceMode::Flight:
            flightMode = true;
            break;
        case MeeGo::QmDeviceMode::Error:
        default:
            flightMode = false;
            break;
    }
#endif
#ifdef QT_DEBUG
    qDebug() << "Fligth Mode: " << flightMode;
#endif

    return flightMode;
}




/*!
 * \fn int OcNetwork::bearerType()
 * \brief Returns the currently used bearer type
 *
 * This function returns the category of the currently used network bearer.
 *
 * \return      1 if WLAN or LAN, 2 if mobile network, 0 for everything else
 */

int OcNetwork::bearerType()
{
    QNetworkConfiguration netConf = this->activeConfiguration();

#ifdef QT_DEBUG
    qDebug() << netConf.bearerTypeName();
#endif

    QNetworkConfiguration::BearerType bt;
    bt = netConf.bearerType();
    int btNum;

    switch (bt) {
        case QNetworkConfiguration::BearerEthernet:
        case QNetworkConfiguration::BearerWLAN:
            btNum = 1;
            break;
        case QNetworkConfiguration::Bearer2G:
        case QNetworkConfiguration::BearerCDMA2000:
        case QNetworkConfiguration::BearerWCDMA:
        case QNetworkConfiguration::BearerHSPA:
            btNum = 2;
            break;
        case QNetworkConfiguration::BearerUnknown:
        default:
            btNum = 0;
            break;
    }

    return btNum;

}
