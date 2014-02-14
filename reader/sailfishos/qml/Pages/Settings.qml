import QtQuick 2.0
import Sailfish.Silica 1.0

import "../Common"

Page {
    id: settings

    property bool isConfigSet
    property bool isPrivacyAccepted
    property bool privacyShown

    Connections {
        target: dbus
        onGotConfig: {
            username.text = config["uname"]
            password.text = config["pword"]
            server.text = config["server"]
            usessl.checked = config["usessl"]
            ignoresslerrors.checked = config["ignoresslerrors"]
            updateBehavior.initialValue = config["updatebehavior"]
            updateInterval.initialValue = config["updateinterval"]
            chooseMaxItems.initialValue = config["maxitems"]
            quitEngine.checked = config["quitengine"]
            chooseViewMode.initialValue = config["viewmode"]
            chooseOrderBy.initialValue = config["orderby"]
            showImgsSelection.initialValue = config["handleimgs"]
            handleReadSelection.initialValue = config["handleread"]
            sortAsc.checked = config["sortasc"]
            isConfigSet = dbus.isConfigSet()
        }
    }

    function saveValues() {
        var saveConf={
            uname:username.text,
            pword:password.text,
            server:server.text,
            usessl:usessl.checked,
            ignoresslerrors:ignoresslerrors.checked,
            updatebehavior:updateBehaviorModel.get(updateBehavior.currentIndex).value,
            updateinterval:updateIntervalModel.get(updateInterval.currentIndex).value,
            maxitems:maxItemModel.get(chooseMaxItems.currentIndex).value,
            quitengine:quitEngine.checked,
            viewmode:viewModeModel.get(chooseViewMode.currentIndex).value,
            orderby:orderByModel.get(chooseOrderBy.currentIndex).value,
            handleimgs:showImgsModel.get(showImgsSelection.currentIndex).value,
            handleread:handleReadModel.get(handleReadSelection.currentIndex).value,
            sortasc:sortAsc.checked,
            eventfeeds:"",
            enabled:true,
            themecolor:"black"
        };

        return saveConf;
    }

    Component.onCompleted: {
//        isConfigSet = dbus.isConfigSet()
        dbus.getConfig()
    }

    Component.onDestruction: dbus.saveConfig(saveValues());

    onStatusChanged: {
        if (status == PageStatus.Active) {
            if (dbus.getSetting("display/privacypolicy", false) == false && privacyShown === false) {
                pageStack.push(Qt.resolvedUrl("../Dialogs/PrivacyPolicy.qml"));
                privacyShown = true;
            }
        }
    }


    SilicaFlickable {
        id: singleItem
        anchors.fill: parent
        contentHeight: settingsCol.height
        VerticalScrollDecorator {}

        Column {
            id: settingsCol
            anchors { left: parent.left; right: parent.right }
            spacing: Theme.paddingSmall

            PageHeader { title: qsTr("Settings") }

            SectionHeader { text: qsTr("Server") }

            TextField {
                id: username
                anchors { left: parent.left; right: parent.right }
                label: qsTr("User name"); placeholderText: label
                EnterKey.enabled: text || inputMethodComposing
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                EnterKey.onClicked: password.focus = true
            }
            TextField {
                id: password
                anchors { left: parent.left; right: parent.right }
                label: qsTr("Password"); placeholderText: label
                echoMode: TextInput.Password
                EnterKey.enabled: text || inputMethodComposing
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                EnterKey.onClicked: server.focus = true
            }
            TextField {
                id: server
                anchors { left: parent.left; right: parent.right }
                label: qsTr("Server"); placeholderText: qsTr("Server: e.g. cloud.example.com")
                inputMethodHints: Qt.ImhUrlCharactersOnly
                EnterKey.enabled: text || inputMethodComposing
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                EnterKey.onClicked: server.focus = false
            }
            Button {
                id: testConnectionButton
                text: qsTr("Test connection")
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: { dbus.saveConfig(saveValues()); dbus.getVersion(); testConnectionButton.visible = false }
                Connections {
                    target: dbus
                    onGotVersion: testConnectionButton.visible = true
                    onGotVersionError: testConnectionButton.visible = true
                }
            }
            BusyIndicator {
                id: testConnectionBusy
                anchors.horizontalCenter: parent.horizontalCenter
                size: BusyIndicatorSize.Medium
                visible: !testConnectionButton.visible
                running: testConnectionBusy.visible
            }
            Label {
                id: testConnectionResult
                visible: false
                anchors.horizontalCenter: parent.horizontalCenter
                horizontalAlignment: Text.AlignHCenter
                Timer {
                    interval: 5000
                    running: testConnectionResult.visible
                    onTriggered: testConnectionResult.visible = false
                }
                Connections {
                    target: dbus
                    onGotVersion: { testConnectionResult.visible = true; testConnectionResult.text = qsTr("Found ownCloud News Version %1").arg(version) }
                    onGotVersionError: { testConnectionResult.visible = true; testConnectionResult.text = error }
                }
            }
            TextSwitch {
                id: usessl
                text: qsTr("Use HTTPS (SSL/TLS) connection")
                anchors { left: parent.left; right: parent.right }
                description: qsTr("Because ownCloud News requires to send your username and password with every request, you should keep this enabled to use an encrypted connection, when your server supports or even requires encryption.")
            }
            TextSwitch {
                id: ignoresslerrors
                text: qsTr("Ignore SSL errors")
                anchors { left: parent.left; right: parent.right }
                description: qsTr("Only ignore SSL errors when you really know what you are doing. (Currently there is no system service for handling SSL certificates. ocNews will implement it's own in the future, but for now you have to ignore SSL errors when you are using a self signed certificate.)")
                enabled: usessl.checked
            }

            SectionHeader { text: qsTr("Updating") }

            ListModel {
                id: updateBehaviorModel
                ListElement { name: ""; value: "0" }
                ListElement { name: ""; value: "1" }
                ListElement { name: ""; value: "2" }
                Component.onCompleted: {
                    updateBehaviorModel.get(0).name = qsTr("Only manually")
                    updateBehaviorModel.get(1).name = qsTr("Automatic on Wi-Fi")
                    updateBehaviorModel.get(2).name = qsTr("Always automatic")
                }
            }

            ComboBoxList {
                id: updateBehavior
                anchors { left: parent.left; right: parent.right }
                label: qsTr("Updating")
                model: updateBehaviorModel
            }

            ListModel {
                id: updateIntervalModel
                ListElement { name: ""; value: "900" }
                ListElement { name: ""; value: "1800" }
                ListElement { name: ""; value: "3600" }
                ListElement { name: ""; value: "10800" }
                ListElement { name: ""; value: "21600" }
                ListElement { name: ""; value: "43200" }
                Component.onCompleted: {
                    updateIntervalModel.get(0).name = qsTr("15 Minutes")
                    updateIntervalModel.get(1).name = qsTr("30 Minutes")
                    updateIntervalModel.get(2).name = qsTr("1 Hour")
                    updateIntervalModel.get(3).name = qsTr("3 Hours")
                    updateIntervalModel.get(4).name = qsTr("6 Hours")
                    updateIntervalModel.get(5).name = qsTr("12 Hours")
                }
            }

            ComboBoxList {
                id: updateInterval
                visible: updateBehavior.currentIndex != 0
                anchors { left: parent.left; right: parent.right }
                label: qsTr("Update interval")
                model: updateIntervalModel
            }

            SectionHeader { text: qsTr("Engine") }

            ListModel {
                id: maxItemModel
                ListElement { name: "50"; value: "50" }
                ListElement { name: "100"; value: "100" }
                ListElement { name: "200"; value: "200" }
                ListElement { name: "500"; value: "500" }
            }

            ComboBoxList {
                id: chooseMaxItems
                anchors { left: parent.left; right: parent.right }
                label: qsTr("Number of items to keep")
                model: maxItemModel
            }

            TextSwitch {
                id: quitEngine
                text: qsTr("Quit engine on closing reader")
                anchors { left: parent.left; right: parent.right }
                description: qsTr("By default the engine daemon ist started on application startup and keeps running after closing the application to operate/synchronize in the background. This option needs an application restart.")
            }

            SectionHeader { text: qsTr("Appearance") }

            ListModel {
                id: viewModeModel
                ListElement { name: ""; value: 0 }
                ListElement { name: ""; value: 1 }
                Component.onCompleted: {
                    viewModeModel.get(0).name = qsTr("Folders")
                    viewModeModel.get(1).name = qsTr("Feeds")
                }
            }

            ComboBoxList {
                id: chooseViewMode
                anchors { left: parent.left; right: parent.right }
                label: qsTr("Main view layout")
                model: viewModeModel
            }

            ListModel {
                id: orderByModel
                ListElement { name: ""; value: "id" }
                ListElement { name: ""; value: "title" }
                ListElement { name: ""; value: "unreadCount" }
                Component.onCompleted: {
                    orderByModel.get(0).name = qsTr("Added (like on server)")
                    orderByModel.get(1).name = qsTr("Title")
                    orderByModel.get(2).name = qsTr("Unread count")
                }
            }

            ComboBoxList {
                id: chooseOrderBy
                anchors { left: parent.left; right: parent.right }
                label: qsTr("Order feeds and folders by")
                model: orderByModel
            }

            ListModel {
                id: showImgsModel
                ListElement { name: ""; value: 0 }
                ListElement { name: ""; value: 1 }
                ListElement { name: ""; value: 2 }
                Component.onCompleted: {
                    showImgsModel.get(0).name = qsTr("On request")
                    showImgsModel.get(1).name = qsTr("When item loads")
                    showImgsModel.get(2).name = qsTr("Prefetch at updating")
                }
            }

            ComboBoxList {
                id: showImgsSelection
                anchors { left: parent.left; right: parent.right }
                label: qsTr("Load content images")
                enabled: textFormatSelection.currentIndex === 0
                model: showImgsModel
            }

            ListModel {
                id: handleReadModel
                ListElement { name: ""; value: 0 }
                ListElement { name: ""; value: 1 }
                ListElement { name: ""; value: 2 }
                Component.onCompleted: {
                    handleReadModel.get(0).name = qsTr("Show")
                    handleReadModel.get(1).name = qsTr("Hide")
                    handleReadModel.get(2).name = qsTr("Show after unread")
                }
            }

            ComboBoxList {
                id: handleReadSelection
                anchors { left: parent.left; right: parent.right }
                label: qsTr("Read articles")
                model: handleReadModel
            }

            TextSwitch {
                id: sortAsc
                text: qsTr("Show oldest items on top")
                anchors { left: parent.left; right: parent.right }
                description: qsTr("Set the default order of the news items to show the oldest at the top. You can still change it in the pully menu.")
            }

            SectionHeader { text: qsTr("Maintenance") }

            Button {
                id: resetConfigurationButton
                text: qsTr("Reset configuration")
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: resetConfiguration()
            }

            Button {
                id: deleteDatabaseButton
                text: qsTr("Delete database")
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: deleteDatabase()
            }
        }
    }

    function deleteDatabase()
    {
        remorsePop.execute(qsTr("Deleting database"), function() { dbus.cleanDatabase() } );
    }

    function resetConfiguration()
    {
        remorsePop.execute(qsTr("Resetting configuration"), function() { dbus.resetConfig() })
    }

    RemorsePopup {
        id: remorsePop
    }
}
