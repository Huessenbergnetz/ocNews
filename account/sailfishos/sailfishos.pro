# The name of your app.
# NOTICE: name defined in TARGET has a corresponding QML filename.
#         If name defined in TARGET is changed, following needs to be
#         done to match new name:
#         - corresponding QML filename must be changed
#         - desktop icon filename must be changed
#         - desktop filename must be changed
#         - icon definition filename in desktop file must be changed
TARGET = harbour-ocnews-account

QT += declarative dbus core

target.path = /usr/bin
INSTALLS += target

qml.files = qml/*.qml
qml.path = /usr/share/harbour-ocnews-account/qml
INSTALLS += qml

CONFIG += sailfishapp

SOURCES += src/main.cpp \
    src/ocgenericif.cpp \
    src/ocdbusinterface.cpp \
    src/occonfigif.cpp

OTHER_FILES += qml/harbour-ocnews-account.qml \
    qml/AccountPage.qml

HEADERS += \
    src/ocgenericif.h \
    src/ocdbusinterface.h \
    src/occonfigif.h
