//#ifdef QT_QML_DEBUG
//#include <QtQuick>
//#endif

//#include <QtQuick>
#include <QtQml>
#include <QGuiApplication>
#include <QQuickView>
//#include <QScopedPointer>
#include <QLocale>
#include <QTranslator>

#include <sailfishapp.h>

#include "../../common/globals.h"
#include "../common/ocdbmanager.h"
#include "../common/models/ocfoldermodelsql.h"
#include "../common/models/ocfeedsmodelsql.h"
#include "../common/models/ocitemsmodelsql.h"
#include "../common/models/ocsingleitemmodelsql.h"
#include "../common/models/ocspecialitemsmodelsql.h"
#include "../common/models/occombinedmodelsql.h"
#include "../common/dbus/interfaces/ocdbusfolders.h"
#include "../common/dbus/interfaces/ocdbusfeeds.h"
#include "../common/dbus/interfaces/ocdbusitems.h"
#include "../common/dbus/interfaces/ocdbusupdater.h"

int main(int argc, char *argv[])
{
    // SailfishApp::main() will display "qml/template.qml", if you need more
    // control over initialization, you can use:
    //
    //   - SailfishApp::application(int, char *[]) to get the QGuiApplication *
    //   - SailfishApp::createView() to get a new QQuickView * instance
    //   - SailfishApp::pathTo(QString) to get a QUrl to a resource file
    //
    // To display the view, call "show()" (will show fullscreen on device).

    QGuiApplication* app = SailfishApp::application(argc, argv);
//    QScopedPointer<QGuiApplication>* app(SailfishApp::application(argc, argv));

    QString locale = QLocale::system().name();
    QTranslator *translator = new QTranslator;
    if ((translator->load("ocnewsreader_"+locale, ":/")))
        app->installTranslator(translator);

    // start background daemon ocnews-engine via dbus
    QDBusConnectionInterface* qDbusConInf = QDBusConnection::sessionBus().interface();
    qDbusConInf->startService("de.buschmann23.ocNewsEngine");

    OcDbManager dbman;
    dbman.openDB();

    OcDBusInterface dbus;
    OcDBusFolders folders;
    OcDBusFeeds feeds;
    OcDBusItems items;
    OcDBusUpdater updater;

    OcFolderModelSql *folderModelSql = new OcFolderModelSql();
    OcCombinedModelSql *combinedModelSql = new OcCombinedModelSql();
    OcFeedsModelSql *feedsModelSql = new OcFeedsModelSql();
    OcItemsModelSql *itemsModelSql = new OcItemsModelSql();
    OcSpecialItemsModelSql *specialItemsModelSql = new OcSpecialItemsModelSql();
    OcSingleItemModelSql *singleItemModelSql = new OcSingleItemModelSql();

    QQuickView* view = SailfishApp::createView();

    QObject::connect(view->engine(), SIGNAL(quit()), QCoreApplication::instance(), SLOT(quit()));

    view->rootContext()->setContextProperty("folderModelSql", folderModelSql);
    view->rootContext()->setContextProperty("combinedModelSql", combinedModelSql);
    view->rootContext()->setContextProperty("feedsModelSql", feedsModelSql);
    view->rootContext()->setContextProperty("itemsModelSql", itemsModelSql);
    view->rootContext()->setContextProperty("specialItemsModelSql", specialItemsModelSql);
    view->rootContext()->setContextProperty("singleItemModelSql", singleItemModelSql);
    view->rootContext()->setContextProperty("folders", &folders);
    view->rootContext()->setContextProperty("feeds", &feeds);
    view->rootContext()->setContextProperty("items", &items);
    view->rootContext()->setContextProperty("updater", &updater);
    view->rootContext()->setContextProperty("dbus", &dbus);
    view->rootContext()->setContextProperty("versionString", VERSION_STRING);

    view->setSource(QUrl::fromLocalFile("/usr/share/harbour-ocnews-reader/qml/harbour-ocnews-reader.qml"));
    view->show();

    return app->exec();
}
