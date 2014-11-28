import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.0

import "../Common"
import "../Sheets"
import "../Delegates"

Page {
    id: settings

    tools: settingsTools

    orientationLock: PageOrientation.LockPortrait

    property int maxFeeds
    property bool isConfigSet

    function saveEventViewFeeds() {
        // get IDs of the selected feeds for the event view
        var selInd = feedSelectionDialog.selectedIndexes;
        var selFed = new Array();
        for (var i=0, tot=selInd.length; i < tot; i++) {
            selFed.push(feedSelectionDialog.model.get(feedSelectionDialog.selectedIndexes[i]).value);
        }

        config.eventFeeds = selFed.toString()
    }

    Component.onCompleted: {
        isConfigSet = dbus.isConfigSet();
        if (!config.privacyShown) {
            settingsPrivacySheet.policy = config.privacyShown
            settingsPrivacySheet.open();
        }
    }

    Component.onDestruction: {
        saveEventViewFeeds()
        config.fontSize = fontSizeSelector.value
    }

    // ------------- Header Start ----------------

        Header {
            id: settingsPageHeader
            text: qsTr("Account Settings")
        }

    // ------------- Header End ----------------



    TabGroup {
        id: tabgroup
        currentTab: tab1
        anchors { top: settingsPageHeader.bottom; topMargin: 5 }

        Page {
            id: tab1
            orientationLock: PageOrientation.LockPortrait

            Button {
                id: accountButton
                anchors { horizontalCenter: parent.horizontalCenter; top: parent.top; topMargin: 20 }
                text: isConfigSet ? qsTr("Open account manager") : qsTr("Create new account")
                onClicked: isConfigSet ? launcher.launch("/usr/bin/invoker", "--single-instance,--type=m,--splash=/usr/share/themes/blanco/meegotouch/images/splash/meegotouch-accounts-splash.jpg,/usr/bin/accounts-ui") : launcher.launch("/usr/bin/invoker", "-w,--type=d,--single-instance,/usr/lib/AccountSetup/bin/ocnewsplugin")
            }

            Text {
                id: accountButtonDescription
                anchors { top: accountButton.bottom; topMargin: 20; left: parent.left; leftMargin: 20; right: parent.right; rightMargin: 20 }
                textFormat: Text.PlainText
                width: parent.width
                wrapMode: Text.WordWrap
                color: theme.inverted ? "white" : "black"
                font.pointSize: 17
                font.weight: Font.Light
                text: isConfigSet ? qsTr("Your account is successfully set. You can configure it in the account manager.") : qsTr("You need to create an account for your ownCloud server. Click the button above to create a new account in the account manager.")
            }
        }

        Page {
            id: tab2
            orientationLock: PageOrientation.LockPortrait

            Flickable {
                id: tab2Content
                anchors { right: parent.right; left: parent.left; top: parent.top; bottom: parent.bottom; bottomMargin: 70 }
                contentWidth: parent.width
                contentHeight: settingsCol.height + 50

                flickableDirection:  Flickable.VerticalFlick

                Column {
                    id: settingsCol
                    anchors { left: parent.left; right: parent.right; top: parent.top; topMargin: 10 }
                    spacing: 10

                    ListModel {
                        id: maxItemModel
                        ListElement { name: "50"; value: 50 }
                        ListElement { name: "100"; value: 100 }
                        ListElement { name: "200"; value: 200 }
                        ListElement { name: "500"; value: 500 }
                    }

                    SelectionItem {
                        id: chooseMaxItems
                        title: qsTr("Number of items to keep")
                        width: (parent.width - 40)
                        anchors.horizontalCenter: parent.horizontalCenter
                        model: maxItemModel
                        initialValue: config.maxItems
                        onCurrentValueChanged: config.maxItems = currentValue
                    }

                    ListModel {
                        id: viewModeModel
                        ListElement { name: QT_TR_NOOP("Folders"); value: 0 }
                        ListElement { name: QT_TR_NOOP("Feeds"); value: 1 }
                    }

                    SelectionItem {
                        id: chooseViewMode
                        title: qsTr("Main view layout")
                        width: (parent.width - 40)
                        anchors.horizontalCenter: parent.horizontalCenter
                        model: viewModeModel
                        initialValue: config.viewMode
                        onCurrentValueChanged: config.viewMode = currentValue
                        tsBasename: "Settings"
                    }

                    ListModel {
                        id: orderByModel
                        ListElement { name: QT_TR_NOOP("Added (like on server)"); value: "id" }
                        ListElement { name: QT_TR_NOOP("Title"); value: "title" }
                        ListElement { name: QT_TR_NOOP("Unread count"); value: "unreadCount" }
                    }

                    SelectionItem {
                        id: chooseOrderBy
                        title: qsTr("Order feeds and folders by")
                        width: parent.width - 40
                        anchors.horizontalCenter: parent.horizontalCenter
                        model: orderByModel
                        initialValue: config.orderBy
                        onCurrentValueChanged: config.orderBy = currentValue
                        tsBasename: "Settings"
                    }

                    LabeledSwitch {
                        id: hideReadFeeds
                        width: parent.width - 40
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: qsTr("Hide read feeds and folders")
                        checked: config.hideReadFeeds
                        onCheckedChanged: config.hideReadFeeds = checked
                    }

                    ListModel {
                        id: updateBehaviorModel
                        ListElement { name: QT_TR_NOOP("Only manually"); value: 0 }
                        ListElement { name: QT_TR_NOOP("Automatic on Wi-Fi"); value: 1 }
                        ListElement { name: QT_TR_NOOP("Always automatic"); value: 2 }
                    }

                    SelectionItem {
                        id: updateBehavior
                        title: qsTr("Updating")
                        width: parent.width - 40
                        anchors.horizontalCenter: parent.horizontalCenter
                        model: updateBehaviorModel
                        initialValue: config.updateBehavior
                        onCurrentValueChanged: config.updateBehavior = currentValue
                        tsBasename: "Settings"
                    }

                    ListModel {
                        id: updateIntervalModel
                        ListElement { name: QT_TR_NOOP("15 Minutes"); value: 900 }
                        ListElement { name: QT_TR_NOOP("30 Minutes"); value: 1800 }
                        ListElement { name: QT_TR_NOOP("1 Hour"); value: 3600 }
                        ListElement { name: QT_TR_NOOP("3 Hours"); value: 10800 }
                        ListElement { name: QT_TR_NOOP("6 Hours"); value: 21600 }
                        ListElement { name: QT_TR_NOOP("12 Hours"); value: 43200 }
                    }

                    SelectionItem {
                       id: updateInterval
                       visible: config.updateBehavior !== 0
                       title: qsTr("Update interval")
                       width: parent.width - 40
                       anchors.horizontalCenter: parent.horizontalCenter
                       model: updateIntervalModel
                       initialValue: config.updateInterval
                       onCurrentValueChanged: config.updateInterval
                       tsBasename: "Settings"
                    }

                    LabeledSwitch {
                        id: richText
                        width: parent.width - 40
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: qsTr("Use prettier but slower Rich Text")
                        checked: config.useRichText
                        onCheckedChanged: config.useRichText = checked
                    }

                    LabeledSwitch {
                        id: themeInverted
                        width: parent.width - 40
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: qsTr("Invert the theme color")
                        checked: config.themeInverted
                        onCheckedChanged: config.themeInverted = checked
                    }

                    ListModel {
                        id: handleImgsModel
                        ListElement { name: QT_TR_NOOP("On request"); value: 0 }
                        ListElement { name: QT_TR_NOOP("When item loads"); value: 1 }
                        ListElement { name: QT_TR_NOOP("Prefetch at updating"); value: 2 }
                    }

                    SelectionItem {
                        id: handleImgs
                        title: qsTr("Load content images")
                        width: parent.width - 40
                        anchors.horizontalCenter: parent.horizontalCenter
                        enabled: config.useRichText
                        model: handleImgsModel
                        initialValue: config.handleImgs
                        onCurrentValueChanged: config.handleImgs === currentValue
                        tsBasename: "Settings"
                    }


                    ListModel {
                        id: handleReadModel
                        ListElement { name: QT_TR_NOOP("Show"); value: 0 }
                        ListElement { name: QT_TR_NOOP("Hide"); value: 1 }
                        ListElement { name: QT_TR_NOOP("Show after unread"); value: 2 }
                    }

                    SelectionItem {
                       id: handleRead
                       title: qsTr("Read articles")
                       width: parent.width - 40
                       anchors.horizontalCenter: parent.horizontalCenter
                       model: handleReadModel
                       initialValue: config.handleRead
                       onCurrentValueChanged: config.handleRead = currentValue
                    }

                    LabeledSwitch {
                        id: sortAsc
                        width: parent.width - 40
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: qsTr("Show oldest items on top")
                        checked: config.sortAsc
                        onCheckedChanged: config.sortAsc = checked
                    }

                    LabeledSwitch {
                        id: excerpts
                        width: parent.width - 40
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: qsTr("Show article excerpts in list")
                        checked: config.showExcerpts
                        onCheckedChanged: config.showExcerpts = checked
                    }

//                    LabeledSwitch {
//                        id: articlePicture
//                        width: parent.width - 40
//                        anchors.horizontalCenter: parent.horizontalCenter
//                        text: qsTr("Display images in lists")
//                        checked: config.showPicturesInList
//                        onCheckedChanged: config.showPicturesInList = checked
//                    }

                    Item {
                        width: parent.width
                        height: fontSizeLabel.height + fontSizeSelector.height

                        Label {
                            id: fontSizeLabel
                            anchors { left: parent.left; leftMargin: 20; right: parent.right; rightMargin: 20; top: parent.top }
                            text: qsTr("Item view font size:") + " " + fontSizeSelector.value + "pt"
                            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                        }

                        Slider {
                            id: fontSizeSelector
                            anchors { left: parent.left; right: parent.right; top: fontSizeLabel.bottom }
                            minimumValue: 10;
                            maximumValue: 40;
                            valueIndicatorVisible: false
                            stepSize: 1
                            value: config.fontSize
                        }
                    }

                    LabeledSwitch {
                        id: directLinkOpening
                        width: parent.width - 40
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: qsTr("Open links directly")
                        checked: config.directLinkOpening
                        onCheckedChanged: config.directLinkOpening = checked
                    }

                    LabeledSwitch {
                        id: feedsFoldersNotify
                        width: parent.width - 40
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: qsTr("Notify about added/removed feeds and folders")
                        checked: config.notifyFeedsFolders
                        onCheckedChanged: config.notifyFeedsFolders = checked
                    }

                    LabeledSwitch {
                        id: newItemsNotify
                        width: parent.width - 40
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: qsTr("Notify about new articles")
                        checked: config.notifyNewItems
                        onCheckedChanged: config.notifyNewItems = checked
                    }

                    Item {
                        width: parent.width
                        height: quitEngine.height + quitEngineDesc.height

                        LabeledSwitch {
                            id: quitEngine
                            anchors { left: parent.left; leftMargin: 20; right: parent.right; rightMargin: 20; top: parent.top }
                            anchors.horizontalCenter: parent.horizontalCenter
                            text: qsTr("Quit engine on closing reader")
                            checked: config.quitEngine
                            onCheckedChanged: config.quitEngine = checked
                        }

                        Text {
                            id: quitEngineDesc
                            text: qsTr("This option needs an application restart.")
                            anchors { left: parent.left; leftMargin: 20; right: parent.right; rightMargin: 20; top: quitEngine.bottom }
                            wrapMode: Text.WordWrap
                            font.pointSize: 17
                            font.weight: Font.Light
                            color: theme.inverted ? "white" : "black"
                            textFormat: Text.PlainText
                        }
                    }
                }
            }

            ScrollDecorator {
                flickableItem: tab2Content
                anchors { right: parent.right }
            }
        }

        Page {
            id: tab3
            orientationLock: PageOrientation.LockPortrait

            function buildFeedList()
            {
                var fee = feeds.getFeeds();

                feedModelData.clear()

                for (var prop in fee)
                {
                    feedModelData.append({"name":prop, "value":fee[prop]})
                }
            }

            Component.onCompleted: buildFeedList()

            ListModel {
                id: feedModelData
                ListElement { name: "Root";  value: "0"; checked: "false" }
            }

            MultiSelectionDialog {
                id: feedSelectionDialog
                titleText: qsTr("Select feeds for the event view")
                model: feedModelData
                acceptButtonText: "OK"

                Component.onCompleted: {
                    var loadedFeedIds = JSON.parse("[" + config.eventFeeds + "]");
                    var selectedModelIdx = new Array()
                    for (var i=0, tot=loadedFeedIds.length; i < tot; i++) {
                        var found = false;
                        var ii = 0;
                        while ((!found) && (ii < feedModelData.count)) {
                            if (feedModelData.get(ii).value == loadedFeedIds[i]) {
                                selectedModelIdx.push(ii);
                                found = true;
                            }
                            ii++;
                        }
                    }
                    feedSelectionDialog.selectedIndexes = selectedModelIdx;
                }
            }

            Button {
                id: feedSelectionButton
                anchors { horizontalCenter: parent.horizontalCenter; top: parent.top; topMargin: 20 }
                text: qsTr("Select event view feeds")
                onClicked: feedSelectionDialog.open();
            }

            Text {
                id: feedSelectionDescription
                anchors { top: feedSelectionButton.bottom; topMargin: 20; left: parent.left; leftMargin: 20; right: parent.right; rightMargin: 20 }
                textFormat: Text.PlainText
                wrapMode: Text.WordWrap
                color: theme.inverted ? "white" : "black"
                font.pointSize: 17
                font.weight: Font.Light
                text: qsTr("Click the button to select the feeds that you want to aggregate to the Event screen. Only newly downloaded items will be added to the Events screen.")
            }

        }

        Page {
            id: tab4
            orientationLock: PageOrientation.LockPortrait

            Flickable {
                id: tab4Content
                anchors { right: parent.right; left: parent.left; top: parent.top; bottom: parent.bottom; bottomMargin: 70 }
                contentWidth: parent.width
                contentHeight: statCol.height + buttonCol.height + 70

                Column {
                    id: statCol
                    anchors { top: parent.top; left: parent.left; leftMargin: 20; right: parent.right; rightMargin: 20; topMargin: 20 }
                    spacing: 10

                    Label {
                        id: statsLabel
                        text: qsTr("Database statistics")
                        width: parent.width
                        textFormat: Text.PlainText
                        visible: folderCount.visible
                    }

                    Text {
                        id: folderCount
                        width: parent.width
                        font.pointSize: 17
                        font.weight: Font.Light
                        textFormat: Text.PlainText
                        color: theme.inverted ? "white" : "black"
                        visible: text != ""
                        Connections {
                            target: dbus
                            onGotStatistics: folderCount.text = qsTr("Folders: ") + stats["folderCount"]
                        }
                    }

                    Text {
                        id: feedCount
                        width: parent.width
                        font.pointSize: 17
                        font.weight: Font.Light
                        textFormat: Text.PlainText
                        color: theme.inverted ? "white" : "black"
                        visible: text != ""
                        Connections {
                            target: dbus
                            onGotStatistics: feedCount.text = qsTr("Feeds: ") + stats["feedCount"]
                        }
                    }

                    Text {
                        id: itemCount
                        width: parent.width
                        font.pointSize: 17
                        font.weight: Font.Light
                        textFormat: Text.PlainText
                        color: theme.inverted ? "white" : "black"
                        visible: text != ""
                        Connections {
                            target: dbus
                            onGotStatistics: itemCount.text = qsTr("Posts: ") + stats["itemCount"]
                        }
                    }

                    Text {
                        id: unreadCount
                        width: parent.width
                        font.pointSize: 17
                        font.weight: Font.Light
                        textFormat: Text.PlainText
                        color: theme.inverted ? "white" : "black"
                        visible: text != ""
                        Connections {
                            target: dbus
                            onGotStatistics: unreadCount.text = qsTr("Unread: ") + stats["unreadCount"]
                        }
                    }

                    Text {
                        id: lastFullUpdate
                        width: parent.width
                        font.pointSize: 17
                        font.weight: Font.Light
                        textFormat: Text.StyledText
                        color: theme.inverted ? "white" : "black"
                        visible: text != ""
                        Connections {
                            target: dbus
                            onGotStatistics: lastFullUpdate.text = qsTr("Last full update:<br />") + Qt.formatDateTime(new Date(stats["lastFullUpdate"]), qsTr("d. MMMM yyyy, hh:mm"))
                        }
                    }
                }


                Column {
                    id: buttonCol
                    anchors { top: statCol.bottom; left: parent.left; leftMargin: 20; right: parent.right; rightMargin: 20; topMargin: 40 }
                    spacing: 20

                    Button {
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: qsTr("Query statistics")
                        visible: lastFullUpdate.text === ""
                        onClicked: { text = ""; qbi.visible = true; dbus.getStatistics(); enabled = false }

                        BusyIndicator {
                            id: qbi
                            platformStyle: BusyIndicatorStyle { id: headerBusyIndicatorStyle; size: "small"; inverted: theme.inverted }
                            anchors.centerIn: parent
                            visible: false
                            running: visible
                        }
                    }

                    Button {
                        id: resetConfigButton
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: qsTr("Reset configuration")
                        onClicked: resetConfigQuery.open()
                    }

                    Button {
                        id: deleteDBButton
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: qsTr("Delete database")
                        onClicked: deleteDBQuery.open()
                        Connections {
                            target: dbus
                            onGotStatistics: stats["feedCount"] + stats["folderCount"] + stats["itemCount"] == 0 ? deleteDBButton.enabled = false : deleteDBButton.enabled = true
                        }
                    }

                    Button {
                        id: deleteCertsButton
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: qsTr("Remove certificates")
                        onClicked: deleteCertsQuery.open()
                    }

                    Button {
                        id: deleteAllAccountsButton
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: qsTr("Remove accounts")
                        onClicked: removeAllAccountsQuery.open()
                    }

                    Item {
                        width: parent.width
                        height: createLog.height + createLogDesc.height

                        LabeledSwitch {
                            id: createLog
                            anchors { left: parent.left; leftMargin: 20; right: parent.right; rightMargin: 20; top: parent.top }
                            anchors.horizontalCenter: parent.horizontalCenter
                            text: qsTr("Create log files")
                            checked: config.createLogFile
                            onCheckedChanged: config.createLogFile = checked
                        }

                        Text {
                            id: createLogDesc
                            text: qsTr("This option needs a complete application restart. After that it will create log files in %1.").arg(logFilePath)
                            anchors { left: parent.left; leftMargin: 20; right: parent.right; rightMargin: 20; top: createLog.bottom }
                            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                            font.pointSize: 15
                            font.weight: Font.Light
                            color: theme.inverted ? "white" : "black"
                            textFormat: Text.PlainText
                        }
                    }
                }

                ScrollDecorator {
                    flickableItem: tab4Content
                    anchors { right: parent.right }
                }
            }
        }
    }



// ----------------- Toolbar Start --------------------

    ToolBarLayout {
        id: settingsTools
        visible: true
        ToolIcon {
            platformIconId: "toolbar-back"
            anchors.left: (parent === undefined) ? undefined : parent.left
            onClicked: { pageStack.pop(); }
        }
        ButtonRow {
            style: TabButtonStyle { }
            TabButton {
                iconSource: "image://theme/icon-m-toolbar-contact"
                tab: tab1
                onClicked: settingsPageHeader.text = qsTr("Account Settings")
            }
            TabButton {
                iconSource: "image://theme/icon-m-toolbar-settings"
                tab: tab2
                onClicked: settingsPageHeader.text = qsTr("Display & Update Settings")
            }
            TabButton {
                iconSource: "image://theme/icon-m-toolbar-list"
                tab: tab3
                onClicked: settingsPageHeader.text = qsTr("Event Feed Integration")
            }
            TabButton {
                iconSource: "image://theme/icon-m-toolbar-tools"
                tab: tab4
                onClicked: settingsPageHeader.text = qsTr("Maintenance")
            }
        }
    }
// ----------------- Toolbar End --------------------



// ----------------- Dialogs Start --------------------

    QueryDialog {
        id: resetConfigQuery
        icon: "image://theme/icon-m-bootloader-warning"
        acceptButtonText: qsTr("Reset")
        rejectButtonText: qsTr("Cancel")
        message: qsTr("Do you really want reset your configuration?")
        titleText: qsTr("Reset configuration?")
        onAccepted: dbus.resetConfig()
    }


    QueryDialog {
        id: deleteDBQuery
        icon: "image://theme/icon-m-bootloader-warning"
        acceptButtonText: qsTr("Delete")
        rejectButtonText: qsTr("Cancel")
        message: qsTr("Do you really want delete your complete database?")
        titleText: qsTr("Delete database?")
        onAccepted: dbus.cleanDatabase()
    }

    QueryDialog {
        id: deleteCertsQuery
        icon: "image://theme/icon-m-bootloader-warning"
        acceptButtonText: qsTr("Remove")
        rejectButtonText: qsTr("Cancel")
        message: qsTr("Do you really want remove your trustet SSL certificates (only for this application)?")
        titleText: qsTr("Remove trusted certificates?")
        onAccepted: dbus.cleanCertificates()
    }

    QueryDialog {
        id: disableSSLQuery
        icon: "image://theme/icon-m-bootloader-warning"
        acceptButtonText: qsTr("Disable")
        rejectButtonText: qsTr("Keep activated")
        message: qsTr("It is not a good idea to disable HTTPS (SSL), because your username and password then will be sent unencrypted with every request.")
        titleText: qsTr("Disable HTTPS?")
        onAccepted: sslCheck.checked = "false"
        onRejected: sslCheck.checked = "true"
    }

    QueryDialog {
        id: removeAllAccountsQuery
        icon: "image://theme/icon-m-bootloader-warning"
        acceptButtonText: qsTr("Remove all")
        rejectButtonText: qsTr("Cancel")
        message: qsTr("Are you really sure you want to remove all ocNews accounts? You should only use this option when there are issues with the account management.")
        titleText: qsTr("Remove all ocNews accounts?")
        onAccepted: dbus.removeAllAccounts()
    }

// ----------------- Dialogs End --------------------


// ----------------- Sheet Start --------------------

    PrivacySheet {
        id: settingsPrivacySheet
    }

// ----------------- Sheet End --------------------
}
