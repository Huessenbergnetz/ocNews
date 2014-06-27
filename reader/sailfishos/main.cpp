#include <QtQml>
#include <QGuiApplication>
#include <QQuickView>
#include <QLocale>
#include <QTranslator>

#include <sailfishapp.h>

#include "../../common/globals.h"
#include "../common/ocdbmanager.h"
#include "../common/occonfiguration.h"
#include "../common/models/ocfoldermodelsql.h"
#include "../common/models/ocfoldersmodelnew.h"
#include "../common/models/ocfoldersmodelfilter.h"
#include "../common/models/ocfeedsmodelsql.h"
#include "../common/models/ocfeedsmodelnew.h"
#include "../common/models/ocfeedsmodelfilter.h"
#include "../common/models/ocitemsmodelnew.h"
#include "../common/models/ocitemsmodelfilter.h"
#include "../common/models/ocsingleitemmodelnew.h"
#include "../common/models/ocspecialitemsmodelsql.h"
#include "../common/models/ocspecialitemsmodelnew.h"
#include "../common/models/occombinedmodelsql.h"
#include "../common/models/occombinedmodelnew.h"
#include "../common/models/occombinedmodelfilter.h"
#include "../common/dbus/interfaces/ocdbusfolders.h"
#include "../common/dbus/interfaces/ocdbusfeeds.h"
#include "../common/dbus/interfaces/ocdbusitems.h"
#include "../common/dbus/interfaces/ocdbusupdater.h"
#include "../common/dbus/interfaces/ocdbusdownloads.h"
#include "../common/dbus/interfaces/ocdbusimagefetcher.h"
#include "../common/dbus/adaptor/ocdbusadaptor.h"
#include "../common/dbus/adaptor/ocdbusproxy.h"
#include "ocnaminteractor.h"

int main(int argc, char *argv[])
{
    QGuiApplication* app = SailfishApp::application(argc, argv);

    app->setOrganizationName("Buschtrommel");
    app->setOrganizationDomain("buschmann23.de");
    app->setApplicationName("harbour-ocnews-reader");
    app->setApplicationVersion(VERSION_STRING);

    QString locale = QLocale::system().name();
    QTranslator *translator = new QTranslator;
    if ((translator->load("ocnewsreader_"+locale, L10N_PATH)))
        app->installTranslator(translator);
#ifdef QT_DEBUG
    qDebug() << locale;
#endif

    // start background daemon ocnews-engine via dbus
    QDBusConnectionInterface* qDbusConInf = QDBusConnection::sessionBus().interface();
    qDbusConInf->startService("de.buschmann23.ocNewsEngine");

    OcDbManager dbman;
    dbman.openDB();

    OcDBusProxy* dbusproxy = new OcDBusProxy();
    new OcDBusAdaptor(dbusproxy);

    OcDBusInterface dbus;
    OcDBusFolders folders;
    OcDBusFeeds feeds;
    OcDBusItems items;
    OcDBusUpdater updater;
    OcDBusDownloads downloads;
    OcDBusImageFetcher imageFetcher;
    OcConfiguration *config = new OcConfiguration;

//    OcFolderModelSql *folderModelSql = new OcFolderModelSql();
//    OcFeedsModelSql *feedsModelSql = new OcFeedsModelSql();


    OcFoldersModelNew *foldersModelSql = new OcFoldersModelNew;
    OcFoldersModelFilter *foldersModelFilter = new OcFoldersModelFilter;
    foldersModelFilter->setHideRead(config->hideReadFeeds());
    foldersModelFilter->setOrderBy(config->orderBy());
    foldersModelFilter->setSourceModel(foldersModelSql);

    OcCombinedModelNew *combinedModelSql = new OcCombinedModelNew;
    OcCombinedModelFilter *combinedModelFilter = new OcCombinedModelFilter;
    combinedModelFilter->setHideRead(config->hideReadFeeds());
    combinedModelFilter->setOrderBy(config->orderBy());
    combinedModelFilter->setSourceModel(combinedModelSql);

    OcFeedsModelNew *feedsModelSql = new OcFeedsModelNew;
    OcFeedsModelFilter *feedsModelFilter = new OcFeedsModelFilter;
    feedsModelFilter->setHideRead(config->hideReadFeeds());
    feedsModelFilter->setOrderBy(config->orderBy());
    feedsModelFilter->setSourceModel(feedsModelSql);

    OcItemsModelNew *itemsModelSql = new OcItemsModelNew;
    itemsModelSql->setShowExcerpts(config->showExcerpts());
    itemsModelSql->setShowImages(config->showPicturesInList());
    OcItemsModelFilter *itemsModelFilter = new OcItemsModelFilter;
    itemsModelFilter->setSourceModel(itemsModelSql);

    OcSpecialItemsModelNew *specialItemsModelSql = new OcSpecialItemsModelNew;
    specialItemsModelSql->setShowExcerpts(config->showExcerpts());
    specialItemsModelSql->setShowImages(config->showPicturesInList());
    OcItemsModelFilter *specialItemsModelFilter = new OcItemsModelFilter;
    specialItemsModelFilter->setSourceModel(specialItemsModelSql);

    OcSingleItemModelNew *singleItemModelNew = new OcSingleItemModelNew;



    // connections to the items model
    QObject::connect(&items, SIGNAL(markedItemsSuccess(QStringList,QString)), itemsModelSql, SLOT(itemsMarked(QStringList,QString)));
    QObject::connect(&items, SIGNAL(starredItemsSuccess(QStringList,QString)), itemsModelSql, SLOT(itemsStarred(QStringList,QString)));
    QObject::connect(&feeds, SIGNAL(markedReadFeedSuccess(int)), itemsModelSql, SLOT(feedMarkedRead(int)));
    QObject::connect(&items, SIGNAL(updatedItemsSuccess(QList<int>,QList<int>,QList<int>)), itemsModelSql, SLOT(itemsUpdated(QList<int>,QList<int>,QList<int>)));
    QObject::connect(&items, SIGNAL(requestedItemsSuccess(QList<int>,QList<int>,QList<int>)), itemsModelSql, SLOT(itemsUpdated(QList<int>,QList<int>,QList<int>)));
    QObject::connect(&items, SIGNAL(markedAllItemsReadSuccess()), itemsModelSql, SLOT(allMarkedRead()));
    QObject::connect(config, SIGNAL(showExcerptsChanged(bool)), itemsModelSql, SLOT(setShowExcerpts(bool)));
    QObject::connect(config, SIGNAL(showPicturesInListChanged(bool)), itemsModelSql, SLOT(setShowImages(bool)));


    // connections to the special items model
    QObject::connect(&items, SIGNAL(markedItemsSuccess(QStringList,QString)), specialItemsModelSql, SLOT(itemsMarked(QStringList,QString)));
    QObject::connect(&items, SIGNAL(starredItemsSuccess(QStringList,QString)), specialItemsModelSql, SLOT(itemsStarred(QStringList,QString)));
    QObject::connect(&folders, SIGNAL(markedReadFolderSuccess(int)), specialItemsModelSql, SLOT(folderMarkedRead(int)));
    QObject::connect(&items, SIGNAL(updatedItemsSuccess(QList<int>,QList<int>,QList<int>)), specialItemsModelSql, SLOT(itemsUpdated(QList<int>,QList<int>,QList<int>)));
    QObject::connect(&items, SIGNAL(requestedItemsSuccess(QList<int>,QList<int>,QList<int>)), specialItemsModelSql, SLOT(itemsUpdated(QList<int>,QList<int>,QList<int>)));
    QObject::connect(&items, SIGNAL(markedAllItemsReadSuccess()), specialItemsModelSql, SLOT(allMarkedRead()));
    QObject::connect(config, SIGNAL(showExcerptsChanged(bool)), specialItemsModelSql, SLOT(setShowExcerpts(bool)));
    QObject::connect(config, SIGNAL(showPicturesInListChanged(bool)), specialItemsModelSql, SLOT(setShowImages(bool)));


    // connections to combined model filter
    QObject::connect(config, SIGNAL(hideReadFeedsChanged(bool)), combinedModelFilter, SLOT(setHideRead(bool)));
    QObject::connect(config, SIGNAL(orderByChanged(QString)), combinedModelFilter, SLOT(setOrderBy(QString)));

    // connections to combined model
    QObject::connect(&items, SIGNAL(updatedItemsSuccess(QList<int>,QList<int>,QList<int>)), combinedModelSql, SLOT(itemsUpdated(QList<int>,QList<int>,QList<int>)));
    QObject::connect(&items, SIGNAL(requestedItemsSuccess(QList<int>,QList<int>,QList<int>)), combinedModelSql, SLOT(itemsUpdated(QList<int>,QList<int>,QList<int>)));
    QObject::connect(&items, SIGNAL(markedItemsSuccess(QStringList,QString)), combinedModelSql, SLOT(itemsMarked()));
    QObject::connect(&items, SIGNAL(starredItemsSuccess(QStringList,QString)), combinedModelSql, SLOT(itemsStarred()));
    QObject::connect(&feeds, SIGNAL(requestedFeedsSuccess(QList<int>,QList<int>,QList<int>)), combinedModelSql, SLOT(feedsRequested(QList<int>,QList<int>,QList<int>)));
    QObject::connect(&feeds, SIGNAL(createdFeedSuccess(QString,int)), combinedModelSql, SLOT(feedCreated(QString,int)));
    QObject::connect(&feeds, SIGNAL(deletedFeedSuccess(int)), combinedModelSql, SLOT(feedDeleted(int)));
    QObject::connect(&feeds, SIGNAL(markedReadFeedSuccess(int)), combinedModelSql, SLOT(feedMarkedRead(int)));
    QObject::connect(&feeds, SIGNAL(movedFeedSuccess(int,int)), combinedModelSql, SLOT(feedMoved(int,int)));
    QObject::connect(&feeds, SIGNAL(renamedFeedSuccess(QString,int)), combinedModelSql, SLOT(feedRenamed(QString,int)));


    // connections to folders model filter
    QObject::connect(config, SIGNAL(hideReadFeedsChanged(bool)), foldersModelFilter, SLOT(setHideRead(bool)));
    QObject::connect(config, SIGNAL(orderByChanged(QString)), foldersModelFilter, SLOT(setOrderBy(QString)));

    // connections to folders model
    QObject::connect(&items, SIGNAL(updatedItemsSuccess(QList<int>,QList<int>,QList<int>)), foldersModelSql, SLOT(itemsUpdated(QList<int>,QList<int>,QList<int>)));
    QObject::connect(&items, SIGNAL(requestedItemsSuccess(QList<int>,QList<int>,QList<int>)), foldersModelSql, SLOT(itemsUpdated(QList<int>,QList<int>,QList<int>)));
    QObject::connect(&items, SIGNAL(markedItemsSuccess(QStringList,QString)), foldersModelSql, SLOT(itemsMarked()));
    QObject::connect(&items, SIGNAL(starredItemsSuccess(QStringList,QString)), foldersModelSql, SLOT(itemsStarred()));
    QObject::connect(&feeds, SIGNAL(requestedFeedsSuccess(QList<int>,QList<int>,QList<int>)), foldersModelSql, SLOT(feedsRequested(QList<int>,QList<int>,QList<int>)));
    QObject::connect(&feeds, SIGNAL(createdFeedSuccess(QString,int)), foldersModelSql, SLOT(feedCreated(QString,int)));
    QObject::connect(&feeds, SIGNAL(deletedFeedSuccess(int)), foldersModelSql, SLOT(feedDeleted(int)));
    QObject::connect(&feeds, SIGNAL(markedReadFeedSuccess(int)), foldersModelSql, SLOT(feedMarkedRead(int)));
    QObject::connect(&feeds, SIGNAL(movedFeedSuccess(int,int)), foldersModelSql, SLOT(feedMoved(int,int)));
    QObject::connect(&feeds, SIGNAL(renamedFeedSuccess(QString,int)), foldersModelSql, SLOT(feedRenamed(QString,int)));
    QObject::connect(&folders, SIGNAL(requestedFoldersSuccess(QList<int>,QList<int>,QList<int>)), foldersModelSql, SLOT(foldersRequested(QList<int>,QList<int>,QList<int>)));
    QObject::connect(&folders, SIGNAL(createdFolderSuccess(QString,int)), foldersModelSql, SLOT(folderCreated(QString,int)));
    QObject::connect(&folders, SIGNAL(deletedFolderSuccess(int)), foldersModelSql, SLOT(folderDeleted(int)));
    QObject::connect(&folders, SIGNAL(markedReadFolderSuccess(int)), foldersModelSql, SLOT(folderMarkedRead(int)));


    // connections to feeds model filter
    QObject::connect(config, SIGNAL(hideReadFeedsChanged(bool)), feedsModelFilter, SLOT(setHideRead(bool)));
    QObject::connect(config, SIGNAL(orderByChanged(QString)), feedsModelFilter, SLOT(setOrderBy(QString)));


    // connections to feeds model
    QObject::connect(&items, SIGNAL(updatedItemsSuccess(QList<int>,QList<int>,QList<int>)), feedsModelSql, SLOT(itemsUpdated(QList<int>,QList<int>,QList<int>)));
    QObject::connect(&items, SIGNAL(requestedItemsSuccess(QList<int>,QList<int>,QList<int>)), feedsModelSql, SLOT(itemsUpdated(QList<int>,QList<int>,QList<int>)));
    QObject::connect(&items, SIGNAL(markedItemsSuccess(QStringList,QString)), feedsModelSql, SLOT(itemsMarked()));
    QObject::connect(&feeds, SIGNAL(requestedFeedsSuccess(QList<int>,QList<int>,QList<int>)), feedsModelSql, SLOT(feedsRequested(QList<int>,QList<int>,QList<int>)));


    // register reader dbus interface
    QDBusConnection connection = QDBusConnection::sessionBus();
    bool ret = connection.registerService("harbour.ocnews.reader");
    ret = connection.registerObject("/", dbusproxy);

    if (!ret)
        qDebug() << "Failed to register D-Bus interface...";

    QQuickView* view = SailfishApp::createView();

    OcNamInteractor *nami = new OcNamInteractor(view->engine(), view);

    QObject::connect(view->engine(), SIGNAL(quit()), QCoreApplication::instance(), SLOT(quit()));

    if (config->quitEngine())
        QObject::connect(QCoreApplication::instance(), SIGNAL(aboutToQuit()), &dbus, SLOT(quitEngine()));

    view->rootContext()->setContextProperty("foldersModelSql", foldersModelSql);
    view->rootContext()->setContextProperty("foldersModelFilter", foldersModelFilter);
    view->rootContext()->setContextProperty("combinedModelSql", combinedModelSql);
    view->rootContext()->setContextProperty("combinedModelFilter", combinedModelFilter);
    view->rootContext()->setContextProperty("feedsModelSql", feedsModelSql);
    view->rootContext()->setContextProperty("feedsModelFilter", feedsModelFilter);
    view->rootContext()->setContextProperty("itemsModelSql", itemsModelSql);
    view->rootContext()->setContextProperty("itemsModelFilter", itemsModelFilter);
    view->rootContext()->setContextProperty("specialItemsModelSql", specialItemsModelSql);
    view->rootContext()->setContextProperty("specialItemsModelFilter", specialItemsModelFilter);
    view->rootContext()->setContextProperty("item", singleItemModelNew);
    view->rootContext()->setContextProperty("folders", &folders);
    view->rootContext()->setContextProperty("feeds", &feeds);
    view->rootContext()->setContextProperty("items", &items);
    view->rootContext()->setContextProperty("updater", &updater);
    view->rootContext()->setContextProperty("dbus", &dbus);
    view->rootContext()->setContextProperty("dbusproxy", dbusproxy);
    view->rootContext()->setContextProperty("downloads", &downloads);
    view->rootContext()->setContextProperty("imageFetcher", &imageFetcher);
    view->rootContext()->setContextProperty("nami", nami);
    view->rootContext()->setContextProperty("config", config);
    view->rootContext()->setContextProperty("versionString", VERSION_STRING);
    view->rootContext()->setContextProperty("versionInt", VERSION);

    view->setSource(QUrl::fromLocalFile("/usr/share/harbour-ocnews-reader/qml/harbour-ocnews-reader.qml"));
    view->show();

    return app->exec();
}
