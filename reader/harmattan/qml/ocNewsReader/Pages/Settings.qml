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

    function saveValues() {

        // get IDs of the selected feeds for the event view
        var selInd = feedSelectionDialog.selectedIndexes;
        var selFed = new Array();
        for (var i=0, tot=selInd.length; i < tot; i++) {
            selFed.push(feedSelectionDialog.model.get(feedSelectionDialog.selectedIndexes[i]).value);
        }

        var saveConf={
            maxitems:chooseMaxItems.currentValue,
            viewmode:chooseViewMode.currentValue,
            orderby:chooseOrderBy.currentValue,
            textformat:textFormatSelection.valueChoosen,
            themecolor:invertThemeSelection.choosenValue,
            showimgs:showImgByDefaultSelection.choosenValue,
            updatebehavior:updateBehavior.currentValue,
            updateinterval:updateInterval.currentValue,
            eventfeeds:selFed.toString(),
            handleread:handleRead.currentValue,
            sortasc:sortAsc.checked
        };

        return saveConf;
    }

    Component.onCompleted: {
        isConfigSet = dbus.isConfigSet();
        dbus.getConfig();
        if (dbus.getSetting("display/privacypolicy", false) == false) {
            settingsPrivacySheet.policy = dbus.getSetting("display/privacypolicy", false) == "true" ? true : false;
            settingsPrivacySheet.open();
        }
        dbus.getStatistics();
    }

    Component.onDestruction: dbus.saveConfig(saveValues());

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
                anchors { top: accountButton.bottom; topMargin: 20 }
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
                anchors { right: parent.right; left: parent.left; top: parent.top; bottom: parent.bottom; bottomMargin: 90 }
                contentWidth: parent.width
                contentHeight: chooseMaxItems.height + chooseViewMode.height + chooseOrderBy.height + updateBehavior.height + updateInterval.height + textFormatLabel.height + textFormatSelection.height + invertThemeLabel.height + invertThemeSelection.height + showImgByDefaultLabel.height + showImgByDefaultSelection.height + handleRead.height + sortAsc.height + 100

                Component.onCompleted: console.log(tab2Content.contentHeight)

                flickableDirection:  Flickable.VerticalFlick

                ListModel {
                    id: maxItemModel
                    ListElement { name: "50"; value: "50" }
                    ListElement { name: "100"; value: "100" }
                    ListElement { name: "200"; value: "200" }
                    ListElement { name: "500"; value: "500" }
                }

                SelectionItem {
                    id: chooseMaxItems
                    title: qsTr("Number of items to keep")
                    width: (parent.width - 40)
                    anchors { top: parent.top; topMargin: 10; horizontalCenter: parent.horizontalCenter }
                    model: maxItemModel
                    initialValue: "100"
                    Connections {
                        target: dbus
                        onGotConfig: chooseMaxItems.initialValue = config["maxitems"]
                    }
                }

                ListModel {
                    id: viewModeModel
                    ListElement { name: ""; value: 0 }
                    ListElement { name: ""; value: 1 }
                    Component.onCompleted: {
                        viewModeModel.get(0).name = qsTr("Folders")
                        viewModeModel.get(1).name = qsTr("Feeds")
                    }
                }

                SelectionItem {
                    id: chooseViewMode
                    title: qsTr("Main view layout")
                    width: (parent.width - 40)
                    anchors { top: chooseMaxItems.bottom; topMargin: 10; horizontalCenter: parent.horizontalCenter }
                    model: viewModeModel
                    initialValue: "0"
                    Connections {
                        target: dbus
                        onGotConfig: chooseViewMode.initialValue = config["viewmode"]
                    }
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

                SelectionItem {
                    id: chooseOrderBy
                    title: qsTr("Order feeds and folders by")
                    width: parent.width - 40
                    anchors { top: chooseViewMode.bottom; topMargin: 10; horizontalCenter: parent.horizontalCenter }
                    model: orderByModel
                    initialValue: "id"
                    Connections {
                        target: dbus
                        onGotConfig: chooseOrderBy.initialValue = config["orderby"]
                    }
                }

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

                SelectionItem {
                    id: updateBehavior
                    title: qsTr("Updating")
                    width: parent.width - 40
                    anchors { top: chooseOrderBy.bottom; topMargin: 10; horizontalCenter: parent.horizontalCenter }
                    model: updateBehaviorModel
                    initialValue: "0"
                    Connections {
                        target: dbus
                        onGotConfig: updateBehavior.initialValue = config["updatebehavior"]
                    }
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

                SelectionItem {
                   id: updateInterval
                   visible: updateBehavior.currentValue != "0"
                   title: qsTr("Update interval")
                   width: parent.width - 40
                   anchors { top: updateBehavior.bottom; topMargin: 10; horizontalCenter: parent.horizontalCenter }
                   model: updateIntervalModel
                   initialValue: "3600"
                   Connections {
                       target: dbus
                       onGotConfig: updateInterval.initialValue = config["updateinterval"]
                   }
                }

                Label {
                    id: textFormatLabel
                    anchors { top: updateBehavior.currentValue == "0" ? updateBehavior.bottom : updateInterval.bottom; topMargin: 10; left: parent.left; leftMargin: 20 }
                    text: qsTr("Display post content format")
                    textFormat: Text.PlainText
                }

                ButtonRow {
                    id: textFormatSelection
                    property string valueChoosen
                    anchors { top: textFormatLabel.bottom; horizontalCenter: parent.horizontalCenter }
                    Button {
                        id: rich;
                        text: qsTr("Full");
                        checked: textFormatSelection.valueChoosen == "rich";
                        onClicked: textFormatSelection.valueChoosen = "rich"
                    }
                    Button {
                        id: styled;
                        text: qsTr("Limited");
                        checked: textFormatSelection.valueChoosen == "styled";
                        onClicked: textFormatSelection.valueChoosen = "styled"
                    }
                    Connections {
                        target: dbus
                        onGotConfig: textFormatSelection.valueChoosen = config["textformat"]
                    }
                }


                Label {
                    id: invertThemeLabel
                    anchors { top: textFormatSelection.bottom; topMargin: 15; left: parent.left; leftMargin: 20 }
                    text: qsTr("Theme color")
                }

                ButtonRow {
                    id: invertThemeSelection
                    property string choosenValue
                    anchors { top: invertThemeLabel.bottom; horizontalCenter: parent.horizontalCenter }
                    Button {
                        id: white
                        text: qsTr("White")
                        checked: invertThemeSelection.choosenValue == "white"
                        onClicked: { invertThemeSelection.choosenValue = "white"; theme.inverted = false }
                    }
                    Button {
                        id: black
                        text: qsTr("Black")
                        checked: invertThemeSelection.choosenValue == "black"
                        onClicked: { invertThemeSelection.choosenValue = "black"; theme.inverted = true }
                    }
                    Connections {
                        target: dbus
                        onGotConfig: invertThemeSelection.choosenValue = config["themecolor"]
                    }
                }


                Label {
                    id: showImgByDefaultLabel
                    anchors { top: invertThemeSelection.bottom; topMargin: 15; left: parent.left; leftMargin: 20 }
                    text: qsTr("Show images by default")
                    visible: textFormatSelection.valueChoosen === "rich"
                }

                ButtonRow {
                    id: showImgByDefaultSelection
                    property string choosenValue
                    anchors { top: showImgByDefaultLabel.bottom; horizontalCenter: parent.horizontalCenter }
                    visible: textFormatSelection.valueChoosen === "rich"
                    Button {
                        id: hide
                        text: qsTr("Hide")
                        checked: showImgByDefaultSelection.choosenValue == "hide"
                        onClicked: { showImgByDefaultSelection.choosenValue = "hide" }
                    }
                    Button {
                        id: show
                        text: qsTr("Show")
                        checked: showImgByDefaultSelection.choosenValue == "show"
                        onClicked: { showImgByDefaultSelection.choosenValue = "show" }
                    }
                    Connections {
                        target: dbus
                        onGotConfig: showImgByDefaultSelection.choosenValue = config["showimgs"]
                    }
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

                SelectionItem {
                   id: handleRead
                   title: qsTr("Read articles")
                   width: parent.width - 40
                   anchors { top: showImgByDefaultSelection.bottom; topMargin: 10; horizontalCenter: parent.horizontalCenter }
                   model: handleReadModel
                   initialValue: "0"
                   Connections {
                       target: dbus
                       onGotConfig: handleRead.initialValue = config["handleread"]
                   }
                }

                LabeledSwitch {
                    id: sortAsc
                    width: parent.width - 40
                    anchors { top: handleRead.bottom; topMargin: 10; horizontalCenter: parent.horizontalCenter }
                    text: qsTr("Show oldest items on top")
                    Connections {
                        target: dbus
                        onGotConfig: sortAsc.checked = config["sortasc"]
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

            function buildFolderList()
            {
                var fee = feeds.getFeeds();

                feedModelData.clear()

                for (var prop in fee)
                {
                    feedModelData.append({"name":prop, "value":fee[prop]})
                }
            }

            Component.onCompleted: buildFolderList()

            ListModel {
                id: feedModelData
                ListElement { name: "Root";  value: "0"; checked: "false" }
            }

            MultiSelectionDialog {
                id: feedSelectionDialog
                titleText: qsTr("Select feeds for the event view")
                model: feedModelData
                acceptButtonText: "OK"

                Connections {
                    target: dbus
                    onGotConfig: {
                        // convert the string got from the settings into the selected model items
                        var loadedFeedIds = JSON.parse("[" + config["eventfeeds"] + "]");
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
            }

            Button {
                id: feedSelectionButton
                anchors { horizontalCenter: parent.horizontalCenter; top: parent.top; topMargin: 20 }
                text: qsTr("Select event view feeds")
                onClicked: feedSelectionDialog.open();
            }

            Text {
                id: feedSelectionDescription
                anchors { top: feedSelectionButton.bottom; topMargin: 20 }
                textFormat: Text.PlainText
                width: parent.width
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


            Label {
                id: statsLabel
                text: qsTr("Database statistics")
                anchors { top: parent.top; left: parent.left; topMargin: 20; leftMargin: 20 }
                textFormat: Text.PlainText
            }

            Text {
                id: folderCount
                anchors { top: statsLabel.bottom; left: parent.left; topMargin: 10; leftMargin: 20 }
                font.pointSize: 17
                font.weight: Font.Light
                textFormat: Text.PlainText
                color: theme.inverted ? "white" : "black"
                Connections {
                    target: dbus
                    onGotStatistics: folderCount.text = qsTr("Folders: ") + stats["folderCount"]
                }
            }

            Text {
                id: feedCount
                anchors { top: folderCount.bottom; left: parent.left; topMargin: 10; leftMargin: 20 }
                font.pointSize: 17
                font.weight: Font.Light
                textFormat: Text.PlainText
                color: theme.inverted ? "white" : "black"
                Connections {
                    target: dbus
                    onGotStatistics: feedCount.text = qsTr("Feeds: ") + stats["feedCount"]
                }
            }

            Text {
                id: itemCount
                anchors { top: feedCount.bottom; left: parent.left; topMargin: 10; leftMargin: 20 }
                font.pointSize: 17
                font.weight: Font.Light
                textFormat: Text.PlainText
                color: theme.inverted ? "white" : "black"
                Connections {
                    target: dbus
                    onGotStatistics: itemCount.text = qsTr("Posts: ") + stats["itemCount"]
                }
            }

            Text {
                id: unreadCount
                anchors { top: itemCount.bottom; left: parent.left; topMargin: 10; leftMargin: 20 }
                font.pointSize: 17
                font.weight: Font.Light
                textFormat: Text.PlainText
                color: theme.inverted ? "white" : "black"
                Connections {
                    target: dbus
                    onGotStatistics: unreadCount.text = qsTr("Unread: ") + stats["unreadCount"]
                }
            }

            Text {
                id: lastFullUpdate
                anchors { top: unreadCount.bottom; left: parent.left; topMargin: 10; leftMargin: 20 }
                font.pointSize: 17
                font.weight: Font.Light
                textFormat: Text.StyledText
                color: theme.inverted ? "white" : "black"
                Connections {
                    target: dbus
                    onGotStatistics: lastFullUpdate.text = qsTr("Last full update:<br />") + Qt.formatDateTime(new Date(stats["lastFullUpdate"]), qsTr("d. MMMM yyyy, hh:mm"))
                }
            }


            Button {
                id: resetConfigButton
                anchors { horizontalCenter: parent.horizontalCenter; top: lastFullUpdate.bottom; topMargin: 40 }
                text: qsTr("Reset configuration")
                onClicked: resetConfigQuery.open()
            }

            Button {
                id: deleteDBButton
                anchors { horizontalCenter: parent.horizontalCenter; top: resetConfigButton.bottom; topMargin: 20 }
                text: qsTr("Delete database")
                onClicked: deleteDBQuery.open()
                Connections {
                    target: dbus
                    onGotStatistics: stats["feedCount"] + stats["folderCount"] + stats["itemCount"] == 0 ? deleteDBButton.enabled = false : deleteDBButton.enabled = true
                }
            }

            Button {
                id: deleteCertsButton
                anchors { horizontalCenter: parent.horizontalCenter; top: deleteDBButton.bottom; topMargin: 20 }
                text: qsTr("Remove certificates")
                onClicked: deleteCertsQuery.open()
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

// ----------------- Dialogs End --------------------


// ----------------- Sheet Start --------------------

    PrivacySheet {
        id: settingsPrivacySheet
    }

// ----------------- Sheet End --------------------
}
