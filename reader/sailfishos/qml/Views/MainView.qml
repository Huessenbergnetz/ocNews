import QtQuick 2.0
import Sailfish.Silica 1.0

import "../Delegates"
import "../Dialogs"
import "../Common"


Page {
    id: mainView

    property int configState

    Component.onCompleted: {
        if (dbus.isConfigSet() && dbus.isAccountEnabled()) {
            configState = 0
        } else if (!dbus.isConfigSet()) {
            configState = 1
        } else if (dbus.isConfigSet() && !dbus.isAccountEnabled()) {
            configState = 2
        }
    }

    Connections {
        target: updater
        onUpdateFinished: { mainViewPully.busy = false; updateAll.enabled = true }
        onUpdateError: { mainViewPully.busy = false; updateAll.enabled = true }
        onUpdateStarted: { mainViewPully.busy = true; updateAll.enabled = false }
    }
    Connections {
        target: folders
        onCreatedFolderError: mainViewPully.busy = false;
        onCreatedFolderSuccess: mainViewPully.busy = false;
    }
    Connections {
        target: feeds
        onCreatedFeedError: mainViewPully.busy = false;
        onCreatedFeedSuccess: mainViewPully.busy = false;
    }
    Connections {
        target: dbus
        onSavedConfig: {
            if (dbus.isConfigSet() && dbus.isAccountEnabled()) {
                configState = 0
            } else if (!dbus.isConfigSet()) {
                configState = 1
            } else if (dbus.isConfigSet() && !dbus.isAccountEnabled()) {
                configState = 2
            }
        }
    }

    SilicaListView {
        id: mainViewList
        anchors.fill: parent
        anchors.bottomMargin: addActionsDock.open ? addActionsDock.height * 1.5 : 0

        header: PageHeader {
            id: pHeader
            title: updater.isUpdateRunning() ? qsTr("Update running...") : "ocNews"
        }

        PullDownMenu {
            id: mainViewPully
            busy: updater.isUpdateRunning() ? true : false
            MenuItem {
                id: quit
                text: qsTr("Quit")
                onClicked: quitEngine()
            }
            MenuItem {
                id: about
                text: qsTr("About")
                onClicked: pageStack.push(Qt.resolvedUrl("../Pages/About.qml"))
            }
            MenuItem {
                id: settings
                text: qsTr("Settings")
                onClicked: pageStack.push(Qt.resolvedUrl("../Pages/Settings.qml"))
            }
            MenuItem {
                id: add
                enabled: configState === 0
                text: qsTr("Add")
                onClicked: addActionsDock.open = !addActionsDock.open
            }
            MenuItem {
                id: updateAll
                enabled: configState === 0
                text: qsTr("Update all")
                onClicked: { updater.startUpdate(); mainViewPully.busy = true; updateAll.enabled = false; pHeader.title = qsTr("Update running...") }
            }
        }

        ViewPlaceholder {
            enabled: configState > 0
            text: qsTr("With these app you can synchronize and view your ownCloud News App content on your smartphone. Before you can start, you have to setup your ownCloud server account in the settings.")
        }

        ViewPlaceholder {
            enabled: configState === 0 && mainViewList.count <= 1
            text: qsTr("The local database is empty. Please make an update or add new feeds and folders.")
        }

        model: folderModelSql

        delegate: FolderListDelegate { visible: configState === 0 && mainViewList.count > 1 }

        VerticalScrollDecorator {}

        FancyScroller {}

        PushUpMenu {
            id: mainViewPushy
            enabled: mainViewList.contentHeight >= mainViewList.height
            visible: mainViewList.contentHeight >= mainViewList.height
            MenuItem {
                id: goToTop
                text: qsTr("Scroll to top")
                onClicked: mainViewList.scrollToTop()
            }
        }
    }

    DockedPanel {
        id: addActionsDock
        width: parent.width
        height: Theme.itemSizeExtraLarge + Theme.paddingLarge
        dock: Dock.bottom

        Row {
            anchors.centerIn: parent

            Button {
                id: addFeed
                text: qsTr("Add feed")
                onClicked: {
                    addActionsDock.open = false
                    var dialog = pageStack.push(Qt.resolvedUrl("../Dialogs/CreateFeed.qml"))
                    dialog.accepted.connect(function() { mainViewPully.busy = true })
                }
            }

            Button {
                id: addFolder
                text: qsTr("Add folder")
                onClicked: {
                    addActionsDock.open = false
                    var dialog = pageStack.push(Qt.resolvedUrl("../Dialogs/CreateFolder.qml"))
                    dialog.accepted.connect(function() { mainViewPully.busy = true })
                }
            }
        }
    }

    function quitEngine()
    {
        remorsePop.execute(qsTr("Quit reader and engine"), function() { dbus.quitEngine(); Qt.quit() } );
    }

    RemorsePopup {
        id: remorsePop
    }
}
