import QtQuick 2.0
import Sailfish.Silica 1.0

import "../BTComponents"

Page {
    id: settings

    property bool isConfigSet

    Component.onDestruction: saveAccount()

    function saveAccount()
    {
        config.accountUser = username.text
        config.accountPassword = password.text
        config.accountServer = server.text
        config.accountEnabled = true
    }

    onStatusChanged: {
        if (status === PageStatus.Active) {
            if (!config.privacyShown) pageStack.push(Qt.resolvedUrl("../Dialogs/PrivacyPolicy.qml"));
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

            PullDownMenu {
                MenuItem {
                    text: qsTr("Delete database")
                    onClicked: deleteDatabase()
                }
                MenuItem {
                    text: qsTr("Reset configuration")
                    onClicked: resetConfiguration()
                }
                MenuItem {
                    id: testConnection
                    text: qsTr("Test connection")
                    onClicked: { saveAccount(); dbus.getVersion(); testConnection.enabled = false }
                    Connections {
                        target: dbus
                        onGotVersion: testConnection.enabled = true
                        onGotVersionError: testConnection.enabled = true
                    }
                }
            }

            TextField {
                id: username
                anchors { left: parent.left; right: parent.right }
                label: qsTr("User name"); placeholderText: label
                EnterKey.enabled: text || inputMethodComposing
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                EnterKey.onClicked: password.focus = true
                text: config.accountUser
            }

            TextField {
                id: password
                anchors { left: parent.left; right: parent.right }
                label: qsTr("Password"); placeholderText: label
                echoMode: TextInput.Password
                EnterKey.enabled: text || inputMethodComposing
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                EnterKey.onClicked: server.focus = true
                text: config.accountPassword
            }

            TextField {
                id: server
                anchors { left: parent.left; right: parent.right }
                label: qsTr("Server"); placeholderText: qsTr("Server: e.g. cloud.example.com")
                inputMethodHints: Qt.ImhUrlCharactersOnly
                EnterKey.enabled: text || inputMethodComposing
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                EnterKey.onClicked: server.focus = false
                text: config.accountServer
            }

            BusyIndicator {
                id: testConnectionBusy
                anchors.horizontalCenter: parent.horizontalCenter
                size: BusyIndicatorSize.Medium
                visible: !testConnection.enabled
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
                checked: config.accountUseSSL
                onCheckedChanged: config.accountUseSSL = checked
            }

            TextSwitch {
                id: ignoresslerrors
                text: qsTr("Ignore SSL errors")
                anchors { left: parent.left; right: parent.right }
                description: qsTr("Only ignore SSL errors when you really know what you are doing. (Currently there is no system service for handling SSL certificates. ocNews will implement it's own in the future, but for now you have to ignore SSL errors when you are using a self signed certificate.)")
                enabled: usessl.checked
                checked: config.accountIgnoreSSLErrors
                onCheckedChanged: config.accountIgnoreSSLErrors = checked
            }

            SectionHeader { text: qsTr("Updating") }

            ListModel {
                id: updateBehaviorModel
                ListElement { name: ""; value: 0 }
                ListElement { name: ""; value: 1 }
                ListElement { name: ""; value: 2 }
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
                initialValue: config.updateBehavior
                onChoosenValueChanged: config.updateBehavior = choosenValue
            }

            ListModel {
                id: updateIntervalModel
                ListElement { name: ""; value: 900 }
                ListElement { name: ""; value: 1800 }
                ListElement { name: ""; value: 3600 }
                ListElement { name: ""; value: 10800 }
                ListElement { name: ""; value: 21600 }
                ListElement { name: ""; value: 43200 }
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
                initialValue: config.updateInterval
                onChoosenValueChanged: config.updateInterval = choosenValue
            }

            SectionHeader { text: qsTr("Engine") }

            ListModel {
                id: maxItemModel
                ListElement { name: "50"; value: 50 }
                ListElement { name: "100"; value: 100 }
                ListElement { name: "200"; value: 200 }
                ListElement { name: "500"; value: 500 }
            }

            ComboBoxList {
                anchors { left: parent.left; right: parent.right }
                label: qsTr("Number of items to keep")
                model: maxItemModel
                initialValue: config.maxItems
                onChoosenValueChanged: config.maxItems = choosenValue
            }

            TextSwitch {
                id: quitEngine
                text: qsTr("Quit engine on closing reader")
                anchors { left: parent.left; right: parent.right }
                description: qsTr("By default the engine daemon ist started on application startup and keeps running after closing the application to operate/synchronize in the background. This option needs an application restart.")
                checked: config.quitEngine
                onCheckedChanged: config.quitEngine = checked
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
                anchors { left: parent.left; right: parent.right }
                label: qsTr("Main view layout")
                model: viewModeModel
                initialValue: config.viewMode
                onChoosenValueChanged: config.viewMode = choosenValue
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
                initialValue: config.orderBy
                onChoosenValueChanged: config.orderBy = choosenValue
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
                model: showImgsModel
                initialValue: config.handleImgs
                onChoosenValueChanged: config.handleImgs = choosenValue
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
                initialValue: config.handleRead
                onChoosenValueChanged: config.handleRead = choosenValue
            }

            TextSwitch {
                id: hideReadFeeds
                text: qsTr("Hide read feeds and folders")
                anchors { left: parent.left; right: parent.right }
                description: qsTr("Hide feeds and folders that do not contain unread items.")
                checked: config.hideReadFeeds
                onCheckedChanged: config.hideReadFeeds = checked
            }

            TextSwitch {
                id: sortAsc
                text: qsTr("Show oldest items on top")
                anchors { left: parent.left; right: parent.right }
                description: qsTr("Set the default order of the news items to show the oldest at the top. You can still change it in the pully menu.")
                checked: config.sortAsc
                onCheckedChanged: config.sortAsc = checked
            }

            TextSwitch {
                id: excerpts
                text: qsTr("Show article excerpts")
                anchors { left: parent.left; right: parent.right }
                description: qsTr("Shows some lines of the article content in the article list.")
                checked: config.showExcerpts
                onCheckedChanged: config.showExcerpts = checked
            }

            TextSwitch {
                id: listPictures
                text: qsTr("Display images in list")
                anchors { left: parent.left; right: parent.right }
                description: qsTr("Displays an image of the article in the list.")
                checked: config.showPicturesInList
                onCheckedChanged: config.showPicturesInList = checked
            }

            Slider {
                id: fontSizeSlider
                anchors { left: parent.left; right: parent.right }
                minimumValue: Theme.fontSizeTiny
                maximumValue: Theme.fontSizeHuge
                valueText: value + "px"
                stepSize: 1
                label: qsTr("Item view font size")
                value: config.fontSize
                onValueChanged: config.fontSize = value
            }

            SectionHeader { text: qsTr("Notifications") }

            TextSwitch {
                id: notifyFeedsFolders
                text: qsTr("Notify about added/removed feeds and folders")
                anchors { left: parent.left; right: parent.right }
                checked: config.notifyFeedsFolders
                onCheckedChanged: config.notifyFeedsFolders = checked
            }

            TextSwitch {
                id: notifyNewItems
                text: qsTr("Notify about new articles")
                anchors { left: parent.left; right: parent.right }
                checked: config.notifyNewItems
                onCheckedChanged: config.notifyNewItems = checked
            }

            SectionHeader { text: qsTr("Privacy") }

            TextSwitch {
                text: qsTr("Enable private browsing for web view")
                anchors { left: parent.left; right: parent.right }
                checked: config.privateBrowsing
                onCheckedChanged: config.privateBrowsing = checked
            }

            TextSwitch {
                text: qsTr("Allow cookies in web view")
                anchors { left: parent.left; right: parent.right }
                checked: config.enableCookies
                onCheckedChanged: config.enableCookies = checked
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
