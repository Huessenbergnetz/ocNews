#include <QtGui/QApplication>
#include <QDeclarativeContext>
#include <QDebug>
#include <QGraphicsObject>
#include <QDeclarativeContext>
#include <QtDeclarative>
#include "qmlapplicationviewer.h"

#include "../../common/globals.h"
#include "../common/ocdbmanager.h"
#include "../common/models/ocfoldermodelsql.h"
#include "../common/models/occombinedmodelsql.h"
#include "../common/models/ocfeedsmodelsql.h"
#include "../common/models/ocitemsmodelsql.h"
#include "../common/models/ocspecialitemsmodelsql.h"
#include "../common/models/ocsingleitemmodelsql.h"
#include "../common/dbus/interfaces/ocdbusinterface.h"
#include "../common/dbus/interfaces/ocdbusfolders.h"
#include "../common/dbus/interfaces/ocdbusfeeds.h"
#include "../common/dbus/interfaces/ocdbusitems.h"
#include "../common/dbus/interfaces/ocdbusupdater.h"
#include "../common/dbus/interfaces/ocdbusaccount.h"
#include "../common/dbus/interfaces/ocdbusdownloads.h"
#include "../common/dbus/adaptor/ocdbusadaptor.h"
#include "../common/dbus/adaptor/ocdbusproxy.h"
#include "../common/oclauncher.h"
#include "ocshareui.h"


Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(createApplication(argc, argv));
    QApplication::setOrganizationName("Buschtrommel");
    QApplication::setOrganizationDomain("buschmann23.de");
    QApplication::setApplicationName("ocNewsReader");
    QApplication::setApplicationVersion(VERSION_STRING);

    QString locale = QLocale::system().name();
    QTranslator translator;
    if ((translator.load("ocnewsreader_"+locale, ":/")))
        app->installTranslator(&translator);

#ifdef QT_DEBUG
    qDebug() << "Locale: " << locale;
#endif

    // start background daemon ocnews-engine via dbus
    QDBusConnectionInterface* qDbusConInf = QDBusConnection::sessionBus().interface();
    qDbusConInf->startService("de.buschmann23.ocNewsEngine");

    QStringList startupArgs = QCoreApplication::arguments();
    QString openItemId = "0";
    if (startupArgs.size() > 1) {
        openItemId = startupArgs.at(1);
        openItemId.remove(QRegExp("ocnews://"));
    }


#ifdef QT_DEBUG
    qDebug() << QApplication::applicationName() << " " << QApplication::applicationVersion() << " by " << QApplication::organizationName();
#endif

    // create database
    OcDbManager dbman;
    dbman.openDB();

    OcDBusProxy* dbusproxy = new OcDBusProxy();
    new OcDBusAdaptor(dbusproxy);

    OcDBusInterface dbus;
    OcDBusFolders folders;
    OcDBusFeeds feeds;
    OcDBusItems items;
    OcDBusUpdater updater;
    OcDBusAccount account;
    OcDBusDownloads downloads;
    OcShareUi shareUi;
    OcLauncher launcher;



    OcFolderModelSql *folderModelSql = new OcFolderModelSql();
    OcCombinedModelSql *combinedModelSql = new OcCombinedModelSql();
    OcFeedsModelSql *feedsModelSql = new OcFeedsModelSql();
    OcItemsModelSql *itemsModelSql = new OcItemsModelSql();
    OcSpecialItemsModelSql *specialItemsModelSql = new OcSpecialItemsModelSql();
    OcSingleItemModelSql *singleItemModelSql = new OcSingleItemModelSql();


    QDBusConnection connection = QDBusConnection::sessionBus();
    bool ret = connection.registerService("de.buschmann23.ocNewsReader");
    ret = connection.registerObject("/", dbusproxy);

    QmlApplicationViewer viewer;

    viewer.rootContext()->setContextProperty("folders", &folders);
    viewer.rootContext()->setContextProperty("feeds", &feeds);
    viewer.rootContext()->setContextProperty("items", &items);
    viewer.rootContext()->setContextProperty("updater", &updater);
    viewer.rootContext()->setContextProperty("downloads", &downloads);
    viewer.rootContext()->setContextProperty("folderModelSql", folderModelSql);
    viewer.rootContext()->setContextProperty("combinedModelSql", combinedModelSql);
    viewer.rootContext()->setContextProperty("feedsModelSql", feedsModelSql);
    viewer.rootContext()->setContextProperty("itemsModelSql", itemsModelSql);
    viewer.rootContext()->setContextProperty("specialItemsModelSql", specialItemsModelSql);
    viewer.rootContext()->setContextProperty("singleItemModelSql", singleItemModelSql);
    viewer.rootContext()->setContextProperty("shareUi", &shareUi);
    viewer.rootContext()->setContextProperty("dbus", &dbus);
    viewer.rootContext()->setContextProperty("dbusproxy", dbusproxy);
    viewer.rootContext()->setContextProperty("openItemId", openItemId);
    viewer.rootContext()->setContextProperty("account", &account);
    viewer.rootContext()->setContextProperty("launcher", &launcher);
    viewer.rootContext()->setContextProperty("versionString", VERSION_STRING);



    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer.setMainQmlFile(QLatin1String("qml/ocNewsReader/main.qml"));

    viewer.showExpanded();

    return app->exec();
}
