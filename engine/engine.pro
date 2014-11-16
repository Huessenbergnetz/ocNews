#-------------------------------------------------
#
# Project created by QtCreator 2013-12-04T15:21:36
#
#-------------------------------------------------

QT       += core
QT       += network
QT       += sql
QT       += dbus
isEmpty(MEEGO_VERSION_MAJOR) {
#QT       += systeminfo
}
QT       += gui

!isEmpty(MEEGO_VERSION_MAJOR) {
TARGET = ocNewsEngine
} else {
TARGET = harbour-ocnews-engine
}

!isEmpty(MEEGO_VERSION_MAJOR) {
DEFINES += MEEGO_EDITION_HARMATTAN
}

CONFIG   += console

!isEmpty(MEEGO_VERSION_MAJOR) {
CONFIG += meegotouch
CONFIG += meegotouchevents

CONFIG += mobility
MOBILITY += systeminfo
}

include("../qslog/QsLog.pri")

TEMPLATE = app

dbus.path = /usr/share/dbus-1/interfaces
dbus.files = interfaces/*.xml
INSTALLS += dbus

dbusservice.path = /usr/share/dbus-1/services
!isEmpty(MEEGO_VERSION_MAJOR) {
dbusservice.files = harmattan/de.buschmann23.ocNewsEngine.service
} else {
dbusservice.files = sailfishos/de.buschmann23.ocNewsEngine.service
}
INSTALLS += dbusservice

#!isEmpty(MEEGO_VERSION_MAJOR) {
#daemonconf.path = /etc/init/apps
#daemonconf.files = ocnewsengine.conf
#INSTALLS += daemonconf
#}

!isEmpty(MEEGO_VERSION_MAJOR) {
enginel10n.path = /usr/share/l10n/meegotouch
} else {
enginel10n.path = /usr/share/harbour-ocnews-reader/translations
}
enginel10n.files = l10n/*.qm
INSTALLS += enginel10n


isEmpty(MEEGO_VERSION_MAJOR) {
supportfiles.path = /usr/share/harbour-ocnews-reader
supportfiles.files = sailfishos/*.sh
INSTALLS += supportfiles
}

SOURCES += main.cpp \
    ocnetwork.cpp \
    ochelper.cpp \
    ocgeneric.cpp \
    ocdbmanager.cpp \
    occonfiguration.cpp \
    interfaces/ocgenericadaptor.cpp \
    interfaces/occonfigadaptor.cpp \
    ocitems.cpp \
    ocfolders.cpp \
    ocfeeds.cpp \
    interfaces/ocfoldersadaptor.cpp \
    ocupdater.cpp \
    interfaces/ocfeedsadaptor.cpp \
    interfaces/ocitemsadaptor.cpp \
    interfaces/ocupdateradaptor.cpp \
    interfaces/ocaccountadaptor.cpp \
    ocdownloadmanager.cpp \
    interfaces/ocdownloadsadaptor.cpp \
    ocupgradehelper.cpp \
    ../common/globals.cpp \
    ocnotifications.cpp \
    ocimagefetcher.cpp \
    interfaces/ocimagefetcheradaptor.cpp

!isEmpty(MEEGO_VERSION_MAJOR) {
SOURCES += ocaccount.cpp
}

contains(MEEGO_EDITION,harmattan) {
target.path = /opt/ocNewsEngine/bin
} else {
target.path = /usr/bin
}
INSTALLS += target

HEADERS += \
    ocnetwork.h \
    ochelper.h \
    ocgeneric.h \
    ocdbmanager.h \
    occonfiguration.h \
    interfaces/ocgenericadaptor.h \
    interfaces/occonfigadaptor.h \
    ocitems.h \
    ocfolders.h \
    ocfeeds.h \
    interfaces/ocfoldersadaptor.h \
    ocupdater.h \
    interfaces/ocfeedsadaptor.h \
    interfaces/ocitemsadaptor.h \
    interfaces/ocupdateradaptor.h \
    interfaces/ocaccountadaptor.h \
    ../common/globals.h \
    ocdownloadmanager.h \
    interfaces/ocdownloadsadaptor.h \
    ocupgradehelper.h \
    ocnotifications.h \
    ocimagefetcher.h \
    interfaces/ocimagefetcheradaptor.h

!isEmpty(MEEGO_VERSION_MAJOR) {
HEADERS += ocaccount.h
}

unix: CONFIG += link_pkgconfig
!isEmpty(MEEGO_VERSION_MAJOR) {
unix: PKGCONFIG += QJson
unix: PKGCONFIG += qmsystem2
unix: PKGCONFIG += libtuiclient
unix: PKGCONFIG += aegis-certman
unix: PKGCONFIG += accounts-qt
unix: PKGCONFIG += AccountSetup
} else {
LIBS += -lmlite5
INCLUDEPATH += /usr/include/mlite5
PKGCONFIG += keepalive
}

OTHER_FILES += \
    interfaces/de.buschmann23.ocNewsEngine.Generic.xml \
    interfaces/de.buschmann23.ocNewsEngine.Configuration.xml \
    interfaces/de.buschmann23.ocNewsEngine.Folders.xml \
    interfaces/de.buschmann23.ocNewsEngine.Feeds.xml \
    interfaces/de.buschmann23.ocNewsEngine.Items.xml \
    interfaces/de.buschmann23.ocNewsEngine.Updater.xml \
    harmattan/de.buschmann23.ocNewsEngine.service \
    sailfishos/de.buschmann23.ocNewsEngine.service \
    interfaces/de.buschmann23.ocNewsEngine.Account.xml \
    interfaces/de.buschmann23.ocNewsEngine.Downloads.xml \
    l10n/*.ts \
    interfaces/de.buschmann23.ocNewsEngine.ImageFetcher.xml
