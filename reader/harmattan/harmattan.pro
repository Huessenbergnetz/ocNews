TEMPLATE = app

TARGET = ocNewsReader

QT       += core
QT       += network
QT       += sql
QT       += dbus

# Add more folders to ship with the application, here
folder_01.source = qml/ocNewsReader
folder_01.target = qml
folder_02.source = images
DEPLOYMENTFOLDERS = folder_01 folder_02

icons.path = /usr/share/themes/blanco/meegotouch/icons
icons.files = icons/*
INSTALLS += icons

dbusservice.path = /usr/share/dbus-1/services
dbusservice.files = misc/de.buschmann23.ocNewsReader.service
INSTALLS += dbusservice

desktopservice.path = /usr/share/applications
desktopservice.files = misc/ocNewsReader_showContent.desktop
INSTALLS += desktopservice

desktopwrapper.path = /opt/ocNewsReader/bin
desktopwrapper.files = misc/ocNewsReader_showContent_Wrapper.sh
INSTALLS += desktopwrapper

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

symbian:TARGET.UID3 = 0xE86F743E

# Smart Installer package's UID
# This UID is from the protected range and therefore the package will
# fail to install if self-signed. By default qmake uses the unprotected
# range value if unprotected UID is defined for the application and
# 0x2002CCCF value if protected UID is given to the application
#symbian:DEPLOYMENT.installer_header = 0x2002CCCF

# Allow network access on Symbian
symbian:TARGET.CAPABILITY += NetworkServices

# If your application uses the Qt Mobility libraries, uncomment the following
# lines and add the respective components to the MOBILITY variable.
# CONFIG += mobility
# MOBILITY +=

# Speed up launching on MeeGo/Harmattan when using applauncherd daemon
CONFIG += qdeclarative-boostable

# Add dependency to Symbian components
# CONFIG += qt-components

# Add dependency to Meego Share UI
#CONFIG += meegotouch
CONFIG += shareuiinterface-maemo-meegotouch share-ui-plugin share-ui-common mdatauri


DEFINES += MEEGO_EDITION_HARMATTAN


# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp \
    ../common/models/ocspecialitemsmodelsql.cpp \
    ../common/models/ocsingleitemmodelsql.cpp \
    ocshareui.cpp \
    ../common/models/ocitemsmodelsql.cpp \
    ../common/models/ocfoldermodelsql.cpp \
    ../common/models/ocfeedsmodelsql.cpp \
    ../common/ocdbmanager.cpp \
    ../common/dbus/interfaces/ocgenericif.cpp \
    ../common/dbus/interfaces/ocdbusinterface.cpp \
    ../common/dbus/interfaces/occonfigif.cpp \
    ../common/dbus/interfaces/ocfoldersif.cpp \
    ../common/dbus/interfaces/ocdbusfolders.cpp \
    ../common/dbus/interfaces/ocfeedsif.cpp \
    ../common/dbus/interfaces/ocdbusfeeds.cpp \
    ../common/dbus/interfaces/ocitemsif.cpp \
    ../common/dbus/interfaces/ocdbusitems.cpp \
    ../common/dbus/interfaces/ocupdaterif.cpp \
    ../common/dbus/interfaces/ocdbusupdater.cpp \
    ../common/dbus/adaptor/ocdbusadaptor.cpp \
    ../common/dbus/adaptor/ocdbusproxy.cpp \
    ../common/oclauncher.cpp \
    ../common/dbus/interfaces/ocaccountif.cpp \
    ../common/dbus/interfaces/ocdbusaccount.cpp \
    ../common/models/occombinedmodelsql.cpp

# Please do not modify the following two lines. Required for deployment.
include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()

HEADERS += \
    ../common/models/ocspecialitemsmodelsql.h \
    ../common/models/ocsingleitemmodelsql.h \
    ocshareui.h \
    ../common/models/ocitemsmodelsql.h \
    ../common/models/ocfoldermodelsql.h \
    ../common/models/ocfeedsmodelsql.h \
    ../common/ocdbmanager.h \
    ../common/dbus/interfaces/ocgenericif.h \
    ../common/dbus/interfaces/ocdbusinterface.h \
    ../common/dbus/interfaces/occonfigif.h \
    ../common/dbus/interfaces/ocfoldersif.h \
    ../common/dbus/interfaces/ocdbusfolders.h \
    ../common/dbus/interfaces/ocfeedsif.h \
    ../common/dbus/interfaces/ocdbusfeeds.h \
    ../common/dbus/interfaces/ocitemsif.h \
    ../common/dbus/interfaces/ocdbusitems.h \
    ../common/dbus/interfaces/ocupdaterif.h \
    ../common/dbus/interfaces/ocdbusupdater.h \
    ../common/dbus/adaptor/ocdbusadaptor.h \
    ../common/dbus/adaptor/ocdbusproxy.h \
    ../common/oclauncher.h \
    ../common/dbus/interfaces/ocaccountif.h \
    ../common/dbus/interfaces/ocdbusaccount.h \
    ../../common/globals.h \
    ../common/models/occombinedmodelsql.h

OTHER_FILES += \
    ../common/dbus/adaptor/de.buschmann23.ocNewsReader.xml \
    misc/ocNewsReader_showContent.desktop \
    misc/de.buschmann23.ocNewsReader.service \
    misc/ocNewsReader_showContent_Wrapper.sh

RESOURCES += \
    ../../translations/harmattan/ocnewsreader.qrc
