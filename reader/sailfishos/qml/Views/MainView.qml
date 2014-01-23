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
            title: operationRunning ? qsTr("Update running...") : "ocNews"
        }

        PullDownMenu {
            id: mainViewPully
            busy: operationRunning
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
                enabled: configState === 0 && !operationRunning
                text: qsTr("Add")
                onClicked: addActionsDock.open = !addActionsDock.open
            }
            MenuItem {
                id: updateAll
                enabled: configState === 0 && !operationRunning
                text: qsTr("Update all")
//                onClicked: { updater.startUpdate(); mainViewPully.busy = true; updateAll.enabled = false; pHeader.title = qsTr("Update running...") }
                onClicked: { updater.startUpdate(); operationRunning = true }
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
                enabled: !operationRunning
                onClicked: {
                    addActionsDock.open = false
                    var dialog = pageStack.push(Qt.resolvedUrl("../Dialogs/CreateFeed.qml"))
                    dialog.accepted.connect(function() { operationRunning = true })
                }
            }

            Button {
                id: addFolder
                text: qsTr("Add folder")
                enabled: !operationRunning
                onClicked: {
                    addActionsDock.open = false
                    var dialog = pageStack.push(Qt.resolvedUrl("../Dialogs/CreateFolder.qml"))
                    dialog.accepted.connect(function() {operationRunning = true })
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
