TEMPLATE = app

TARGET = harbour-ocnews-reader

#QT += declarative dbus core sql
#QT += quick qml dbus core sql
QT += dbus core sql

target.path = /usr/bin
INSTALLS += target

#qml.files = qml/*.qml
#qml.path = /usr/share/harbour-ocnews-reader/qml
#INSTALLS += qml

icons.path = /usr/share/harbour-ocnews-reader/icons
icons.files = icons/*
INSTALLS += icons

CONFIG += sailfishapp

SOURCES += main.cpp \
    ../common/ocdbmanager.cpp \
    ../common/models/ocitemsmodelsql.cpp \
    ../common/models/ocspecialitemsmodelsql.cpp \
    ../common/models/ocsingleitemmodelsql.cpp \
    ../common/models/ocfoldermodelsql.cpp \
    ../common/models/ocfeedsmodelsql.cpp \
    ../common/models/occombinedmodelsql.cpp \
    ../common/dbus/interfaces/ocupdaterif.cpp \
    ../common/dbus/interfaces/ocitemsif.cpp \
    ../common/dbus/interfaces/ocgenericif.cpp \
    ../common/dbus/interfaces/ocfoldersif.cpp \
    ../common/dbus/interfaces/ocfeedsif.cpp \
    ../common/dbus/interfaces/ocdbusupdater.cpp \
    ../common/dbus/interfaces/ocdbusitems.cpp \
    ../common/dbus/interfaces/ocdbusinterface.cpp \
    ../common/dbus/interfaces/ocdbusfolders.cpp \
    ../common/dbus/interfaces/ocdbusfeeds.cpp \
    ../common/dbus/interfaces/ocdbusaccount.cpp \
    ../common/dbus/interfaces/occonfigif.cpp \
    ../common/dbus/interfaces/ocaccountif.cpp

OTHER_FILES += qml/harbour-ocnews-reader.qml \
    qml/Views/MainView.qml \
    harbour-ocnews-reader.desktop \
    qml/Delegates/FolderListDelegate.qml \
    qml/Views/FeedListView.qml \
    qml/Common/CountBubble.qml \
    qml/Delegates/FeedListDelegate.qml \
    qml/Views/ItemListView.qml \
    qml/Delegates/ItemListDelegate.qml \
    qml/Views/SpecialItemListView.qml \
    qml/Delegates/SpecialItemListDelegate.qml \
    qml/Views/SingleItemView.qml \
    qml/Views/SingleItemWebView.qml \
    qml/JS/globals.js \
    qml/Dialogs/CreateFolder.qml \
    qml/Dialogs/CreateFeed.qml \
    qml/Dialogs/RenameFolder.qml \
    qml/Common/ComboBoxList.qml \
    qml/Dialogs/MoveFeed.qml \
    qml/Pages/Settings.qml \
    qml/Pages/CoverPage.qml \
    qml/Pages/About.qml \
    qml/Dialogs/PrivacyPolicy.qml \
    qml/Common/FancyScroller.qml \
    qml/Pages/Contributors.qml \
    qml/Pages/ContributorItem.qml \
    qml/Pages/images/placeholder.png \
    qml/Pages/icons/browser.png \
    qml/Pages/icons/twitter.png \
    qml/Pages/images/buschmann.png \
    qml/Pages/icons/github.png \
    qml/Pages/icons/linkedin.png \
    qml/Pages/icons/weibo.png \
    qml/Pages/images/tigre-bleu.png \
    qml/Common/ImageButton.qml \
    qml/Pages/images/finalmix.png \
    qml/Common/FetchImagesIndicator.qml

HEADERS += \
    ../common/ocdbmanager.h \
    ../common/models/ocspecialitemsmodelsql.h \
    ../common/models/ocitemsmodelsql.h \
    ../common/models/ocfoldermodelsql.h \
    ../common/models/ocsingleitemmodelsql.h \
    ../common/models/ocfeedsmodelsql.h \
    ../common/models/occombinedmodelsql.h \
    ../common/dbus/interfaces/ocupdaterif.h \
    ../common/dbus/interfaces/ocitemsif.h \
    ../common/dbus/interfaces/ocgenericif.h \
    ../common/dbus/interfaces/ocfoldersif.h \
    ../common/dbus/interfaces/ocfeedsif.h \
    ../common/dbus/interfaces/ocdbusupdater.h \
    ../common/dbus/interfaces/ocdbusitems.h \
    ../common/dbus/interfaces/ocdbusinterface.h \
    ../common/dbus/interfaces/ocdbusfolders.h \
    ../common/dbus/interfaces/ocdbusfeeds.h \
    ../common/dbus/interfaces/ocdbusaccount.h \
    ../common/dbus/interfaces/occonfigif.h \
    ../common/dbus/interfaces/ocaccountif.h \
    ../../common/globals.h

RESOURCES += \
    ../../translations/sailfishos/ocnewsreader.qrc


