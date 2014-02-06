#include <QtCore/QCoreApplication>
#include <QSettings>
//#include <MRemoteAction>
#if defined(MEEGO_EDITION_HARMATTAN)
#include <aegis_certman.h>
#endif

#include "../common/globals.h"
#include "ocdbmanager.h"
#include "occonfiguration.h"
#include "interfaces/occonfigadaptor.h"
#include "ocgeneric.h"
#include "interfaces/ocgenericadaptor.h"
#include "ocfolders.h"
#include "interfaces/ocfoldersadaptor.h"
#include "ocfeeds.h"
#include "interfaces/ocfeedsadaptor.h"
#include "ocitems.h"
#include "interfaces/ocitemsadaptor.h"
#include "ocupdater.h"
#include "interfaces/ocupdateradaptor.h"
#include "ocdownloadmanager.h"
#include "interfaces/ocdownloadsadaptor.h"
#if defined(MEEGO_EDITION_HARMATTAN)
#include "ocaccount.h"
#include "interfaces/ocaccountadaptor.h"
#endif


#if defined(MEEGO_EDITION_HARMATTAN)
// Callback function for aegis_certman_iterate_certs
QList<QSslCertificate> localCerts;
QStringList localCertsToDelete;
int addCert(int ordering, X509* cert, void* ctx)
{
    // convert internal X509 structure to DER
    int len;
    unsigned char *buf;

    buf = NULL;

    len = i2d_X509(cert, &buf);

    if (len > 0) {
        int guiCheck = aegis_certman_gui_check_certificate(cert, 120);

        if (guiCheck == 0) {
            QByteArray buffer(reinterpret_cast<const char*>(buf), len);
            QSslCertificate sslCert(buffer, QSsl::Der);
            localCerts.append(sslCert);
        } else {
            aegis_key_id crtKey;
            aegis_certman_get_key_id(cert, crtKey);

            char buffer[AEGIS_KEY_ID_STR_LEN];

            aegis_certman_key_id_to_str(crtKey, buffer, AEGIS_KEY_ID_STR_LEN);

            localCertsToDelete.append(buffer);
        }
    }
    return 0;
}
#endif

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    a.setOrganizationName("Buschtrommel");
    a.setOrganizationDomain("buschmann23.de");
    a.setApplicationName("ocNewsEngine");
    a.setApplicationVersion(VERSION_STRING);

    QString locale = QLocale::system().name();
    QTranslator translator;
    if ((translator.load("ocnewsengine_"+locale, ":/")))
        a.installTranslator(&translator);

    QString settingsPath(QDir::homePath().append(SETTINGS_PATH));
    QSettings::setPath(QSettings::NativeFormat, QSettings::UserScope, settingsPath);

    // set paths
    QString basePath(QDir::homePath().append(BASE_PATH));
    QString faviconsPath = basePath.append("/favicons");
    QString enclosuresPath = basePath.append("/enclosures");
    QString imgCache(QDir::homePath().append(IMAGE_CACHE));
    QString localCertsPath = basePath.append("/certs");
    QString globalCertsPath = QString(GLOBAL_CERTS_PATH);
    QString mediaPath(QDir::homePath().append(MEDIA_PATH));

    // create storage dirs
    QDir().mkpath(faviconsPath);
    QDir().mkpath(enclosuresPath);
    QDir().mkpath(imgCache);
    QDir().mkpath(localCertsPath);
    QDir().mkpath(mediaPath);

#if defined(MEEGO_EDITION_HARMATTAN)
    // set credential for ssl domain
    int credSuc = aegis_certman_set_credentials("buschtrommel-ocnews::CertOCNewsSSL");
    if (credSuc != 0) qDebug() << "set credential error: " << credSuc;

    // open ssl domain
    domain_handle ownDomain;
    int openCheck = aegis_certman_open_domain("ssl-ocnews", AEGIS_CERTMAN_DOMAIN_PRIVATE, &ownDomain);
    if (openCheck != 0) qDebug() << "Error Opening Domain: " << openCheck;

    aegis_certman_iterate_certs(ownDomain, &addCert, NULL);

    // delete untrusted certs from private cert domain
    if (!localCertsToDelete.isEmpty())
    {
        for (int i = 0; i < localCertsToDelete.size(); ++i)
        {
            aegis_key_id crtKey;
            aegis_certman_str_to_key_id(localCertsToDelete.at(i).toLocal8Bit().data(),crtKey);
            int rmCertCheck = aegis_certman_rm_cert(ownDomain, crtKey);
            if (rmCertCheck != 0)
                qDebug() << "Error removing Certificate from private domain.";
        }
    }

    aegis_certman_close_domain(ownDomain);
#endif


#if defined(MEEGO_EDITION_HARMATTAN)
    QList<QSslCertificate> certs = QSslCertificate::fromPath(globalCertsPath.append(QDir::separator()).append("*.pem"), QSsl::Pem, QRegExp::Wildcard);
    certs += QSslCertificate::fromPath(globalCertsPath.append(QDir::separator()).append("*.crt"), QSsl::Pem, QRegExp::Wildcard);

    certs += localCerts;
    QSslSocket::setDefaultCaCertificates(certs);
#endif


#ifdef QT_DEBUG
    qDebug() << "Locale: " << locale;
    qDebug() << "Read SSL Certificates: " << QSslSocket::defaultCaCertificates().count();
#endif

    // create database
    OcDbManager dbman;
    dbman.openDB();
    dbman.createTables();

    OcConfiguration* configuration = new OcConfiguration;
    new OcConfigAdaptor(configuration);
    OcGeneric* generic = new OcGeneric;
    new OcGenericAdaptor(generic);
    OcFolders* folders = new OcFolders;
    new OcFoldersAdaptor(folders);
    OcFeeds* feeds = new OcFeeds;
    new OcFeedsAdaptor(feeds);
    OcItems* items = new OcItems;
    new OcItemsAdaptor(items);
    OcUpdater* updater = new OcUpdater;
    new OcUpdaterAdaptor(updater);
    OcDownloadManager* dManager = new OcDownloadManager;
    new OcDownloadManagerAdaptor(dManager);
#if defined(MEEGO_EDITION_HARMATTAN)
    OcAccount* account = new OcAccount;
    new OcAccountAdaptor(account);
#endif

    QObject::connect(configuration, SIGNAL(savedConfig()), updater, SLOT(handleNetAndConfChanges()));


    QDBusConnection connection = QDBusConnection::sessionBus();
    bool ret = connection.registerService("de.buschmann23.ocNewsEngine");
    ret = connection.registerObject("/", generic);
    ret = connection.registerObject("/Configuration", configuration);
    ret = connection.registerObject("/Folders", folders);
    ret = connection.registerObject("/Feeds", feeds);
    ret = connection.registerObject("/Items", items);
    ret = connection.registerObject("/Updater", updater);
    ret = connection.registerObject("/Downloads", dManager);
#if defined(MEEGO_EDITION_HARMATTAN)
    ret = connection.registerObject("/Account", account);
#endif

    configuration->setSetting("system/version", QDBusVariant(VERSION));

//    MRemoteAction evenFeedAction("de.buschmann23.ocNewsEngine", "/Updater", "de.buschmann23.ocNewsEngine", "startUpdate");
//    QDBusInterface eventFeedInterface("com.nokia.home.EventFeed", "/eventfeed", "com.nokia.home.EventFeed", QDBusConnection::sessionBus());
//    eventFeedInterface.call(QDBus::NoBlock, "addRefreshAction", evenFeedAction.toString());

    return a.exec();
}
