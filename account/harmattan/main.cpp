#include <QtGui/QApplication>
#include <MLocale>
#include "qmlapplicationviewer.h"
#include "../../common/globals.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(createApplication(argc, argv));
    QScopedPointer<QmlApplicationViewer> viewer(QmlApplicationViewer::create());
    QApplication::setOrganizationName("Buschtrommel");
    QApplication::setOrganizationDomain("buschmann23.de");
    QApplication::setApplicationName("ocNewsAccount");
    QApplication::setApplicationVersion(VERSION_STRING);

    // start background daemon ocnews-engine via dbus
    QDBusConnectionInterface* qDbusConInf = QDBusConnection::sessionBus().interface();
    qDbusConInf->startService("de.buschmann23.ocNewsEngine");

    MLocale locale;
    locale.installTrCatalog("ocnewsaccount");
    MLocale::setDefault(locale);

    AccountSetup::ProviderPluginProcess* plugin = new AccountSetup::ProviderPluginProcess;
    if ( plugin != AccountSetup::ProviderPluginProcess::instance() )
        qFatal("Instance not unique\n");

    viewer->parentWindowId = plugin->parentWindowId();
    switch(plugin->setupType()) {
    case AccountSetup::CreateNew:
        viewer->init(0);
        plugin->setReturnToAccountsList(true);
        break;

    case AccountSetup::EditExisting:
        viewer->init(1);
        viewer->accountx = plugin->account();
        if (viewer->accountx != NULL)
            viewer->onLoad();
        break;

    case AccountSetup::Unset:
        viewer->init(-1);
        break;
    }

    viewer->setMainQmlFile(QLatin1String("/opt/ocNewsAccount/qml/main.qml"));
    viewer->showExpanded();

    return app->exec();
}
