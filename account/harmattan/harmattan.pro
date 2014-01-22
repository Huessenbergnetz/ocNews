TEMPLATE = app

TARGET = ocnewsplugin

QT += declarative dbus core
CONFIG += meegotouch link_pkgconfig
CONFIG += qdeclarative-boostable

PKGCONFIG += accounts-qt \
             AccountSetup

qml.files = qml/*.qml
qml.path = /opt/ocNewsAccount/qml
INSTALLS += qml

target.path = /usr/lib/AccountSetup/bin
INSTALLS += target

CONFIG += qdeclarative-boostable
SOURCES += main.cpp qmlapplicationviewer.cpp \
    occonfigif.cpp
HEADERS += qmlapplicationviewer.h \
    occonfigif.h

invoker.files = invoker/*
invoker.path = /usr/lib/AccountSetup
INSTALLS += invoker

# Register to accounts
accountsprovider.files = *.provider
accountsprovider.path = /usr/share/accounts/providers
INSTALLS += accountsprovider

OTHER_FILES += \
    invoker/* \
    ocnews.provider \
    qml/main.qml \
    qml/ButtonElement.qml \
    qml/LabeledSwitch.qml \
    qml/LabeledTextField.qml \
    qml/Title.qml \
    qml/MainPage.qml \
    qml/HelpSheet.qml







