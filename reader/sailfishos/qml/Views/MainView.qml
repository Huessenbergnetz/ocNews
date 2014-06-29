import QtQuick 2.0
import Sailfish.Silica 1.0

import "../Delegates"
import "../Dialogs"
import "../Common"
import "../BTComponents"
import "../JS/globals.js" as GLOBALS


Page {
    id: mainView

    property int configState: (config.isValid && config.accountEnabled) ? 0 : (!config.isValid) ? 1 : (config.isValid && !config.accountEnabled) ? 2 : 2

    FirstStartInfo {
        visible: config.displayedVersion < versionInt
        name: "ocNews"
        version: versionString
//        helpPage: "../pages/Help.qml"
        paypalOrganization: "Buschtrommel"
        paypalItem: "ocNews"
        paypalEmail: "kontakt@buschmann23.de"
        paypalMessage: qsTr("Leave a message (English or German):")
        description: qsTr("ocNews is a client for the ownCloud News app, a cloud base RSS/Atom feed aggregator. ")
        onClicked: config.displayedVersion = versionInt
    }

    SilicaListView {
        id: mainViewList
        anchors { top: parent.top; right: parent.right; left: parent.left; bottom: fetchIndicator.visible ? fetchIndicator.top : addActionsDock.open ? addActionsDock.top : parent.bottom }
        clip: true

        header: PageHeader {
            id: pHeader
            title: operationRunning ? qsTr("Update running...") : "ocNews"
        }

        section {
            property: 'folderName'
            delegate: SectionHeader {
                visible: text != ""
                text: section
                height: Theme.itemSizeExtraSmall
            }
        }

        Behavior on anchors.bottomMargin {
            NumberAnimation { duration: 300; easing.type: Easing.InOutQuad }
        }

        Behavior on height {
            NumberAnimation { duration: 300; easing.type: Easing.InOutQuad }
        }

        PullDownMenu {
            id: mainViewPully
            busy: operationRunning
            MenuItem {
                id: quit
                text: qsTr("Quit")
                visible: !config.quitEngine
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
                onClicked: { updater.startUpdate(); operationRunning = true }
            }
            MenuLabel {
                id: lastUpdated
                visible: configState === 0 && !operationRunning && dbus.getStat(1)
                text: qsTr("Last update:") + " " + Qt.formatDateTime(new Date(dbus.getStat(1)), Qt.DefaultLocaleShortDate)
                Connections {
                    target: updater
                    onUpdateFinished: lastUpdated.text = qsTr("Last update: ") + Qt.formatDateTime(new Date(dbus.getStat(1)), Qt.DefaultLocaleShortDate);
                }
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

        model: configState === 0 ? config.viewMode === 0 ? foldersModelFilter : combinedModelFilter : null
        onModelChanged: {
            if (config.viewMode === 0) {
                combinedModelSql.active = false
                foldersModelSql.active = true
            } else {
                foldersModelSql.active = false
                combinedModelSql.active = true
            }
        }

        delegate: FolderListDelegate { visible: configState === 0 && mainViewList.count > 1 }

        VerticalScrollDecorator {}

        PushUpMenu {
            id: mainViewPushy
            visible: mainViewList.contentHeight >= mainViewList.height
            enabled: mainViewList.contentHeight >= mainViewList.height
            MenuItem {
                id: goToTop
                text: qsTr("Scroll to top")
                onClicked: mainViewList.scrollToTop()
            }
        }
    }

    FetchImagesIndicator {
        id: fetchIndicator
    }


    DockedPanel {
        id: addActionsDock
        width: parent.width
        height: Theme.itemSizeExtraLarge
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
