import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.0
import QtMobility.feedback 1.1

import "../Common"
import "../Delegates"
import "../Sheets"
import "../JS/globals.js" as GLOBALS

Page {
    id: mainView
    Component.onCompleted: {
        if (dbus.isConfigSet() && dbus.isAccountEnabled()) {
            configState = 0
        } else if (!dbus.isConfigSet()) {
            configState = 1
        } else if (dbus.isConfigSet() && !dbus.isAccountEnabled()) {
            configState = 2
        }
    }

    property int configState

    tools: mainViewTools
    orientationLock: PageOrientation.LockPortrait

    function openFile(file, properties) {
             var component = Qt.createComponent(file)
             if (component.status == Component.Ready)
                 pageStack.push(component, properties);
             else
                 console.log("Error loading component:", component.errorString());
         }

    Connections {
        target: mainViewAddFeed
        onAccepted: if (mainViewAddFeed.feedAddressText !== "") operationRunning = true
    }
    Connections {
        target: mainViewAddFolder
        onAccepted: if (mainViewAddFolder.folderNameText !== "") operationRunning = true
    }
    Connections {
        target: mainViewRenameFolder
        onAccepted: if (mainViewRenameFolder.folderName !== mainViewRenameFolder.newFolderName && mainViewRenameFolder.newFolderName !== "") operationRunning = true
    }
    Connections {
        target: mainViewMoveFeed
        onAccepted: operationRunning = true
    }
    Connections {
        target: dbus
        onChangedConfig: {
            if (dbus.isConfigSet() && dbus.isAccountEnabled()) {
                configState = 0
            } else if (!dbus.isConfigSet()) {
                configState = 1
            } else if (dbus.isConfigSet() && !dbus.isAccountEnabled()) {
                configState = 2
            }
        }
        onSavedConfig: folderList.model.refresh()
        onCleanedDatabase: folderList.model.refresh()
    }
    Connections {
        target: updater
        onUpdateFinished: { GLOBALS.previousFlatContentY = folderList.contentY; folderList.model.refresh(); folderList.contentY = GLOBALS.previousFlatContentY }
    }
    Connections {
        target: folders
        onCreatedFolderSuccess: { GLOBALS.previousFlatContentY = folderList.contentY; folderList.model.refresh(); folderList.contentY = GLOBALS.previousFlatContentY }
        onDeletedFolderSuccess: { GLOBALS.previousFlatContentY = folderList.contentY; folderList.model.refresh(); folderList.contentY = GLOBALS.previousFlatContentY }
        onMarkedReadFolderSuccess: { GLOBALS.previousFlatContentY = folderList.contentY; folderList.model.refresh(); folderList.contentY = GLOBALS.previousFlatContentY }
        onRenamedFolderSuccess: { GLOBALS.previousFlatContentY = folderList.contentY; folderList.model.refresh(); folderList.contentY = GLOBALS.previousFlatContentY }
    }
    Connections {
        target: feeds
        onCreatedFeedSuccess: { GLOBALS.previousFlatContentY = folderList.contentY; folderList.model.refresh(); folderList.contentY = GLOBALS.previousFlatContentY }
        onDeletedFeedSuccess: { GLOBALS.previousFlatContentY = folderList.contentY; folderList.model.refresh(); folderList.contentY = GLOBALS.previousFlatContentY }
        onMarkedReadFeedSuccess: { GLOBALS.previousFlatContentY = folderList.contentY; folderList.model.refresh(); folderList.contentY = GLOBALS.previousFlatContentY }
        onMovedFeedSuccess: { GLOBALS.previousFlatContentY = folderList.contentY; folderList.model.refresh(); folderList.contentY = GLOBALS.previousFlatContentY }
        onRequestedFeedsSuccess: { GLOBALS.previousFlatContentY = folderList.contentY; folderList.model.refresh(); folderList.contentY = GLOBALS.previousFlatContentY }
    }
    Connections {
        target: items
        onMarkedAllItemsReadSuccess: { GLOBALS.previousFlatContentY = folderList.contentY; folderList.model.refresh(); folderList.contentY = GLOBALS.previousFlatContentY }
        onMarkedItemsSuccess: { GLOBALS.previousFlatContentY = folderList.contentY; folderList.model.refresh(); folderList.contentY = GLOBALS.previousFlatContentY }
        onStarredItemsSuccess: { GLOBALS.previousFlatContentY = folderList.contentY; folderList.model.refresh(); folderList.contentY = GLOBALS.previousFlatContentY }
        onUpdatedItemsSuccess: { GLOBALS.previousFlatContentY = folderList.contentY; folderList.model.refresh(); folderList.contentY = GLOBALS.previousFlatContentY}
        onRequestedItemsSuccess: { GLOBALS.previousFlatContentY = folderList.contentY; folderList.model.refresh(); folderList.contentY = GLOBALS.previousFlatContentY }
    }


// ------------- Header Start ----------------

    Header {
        id: mainViewHeader
        text: qsTr("ocNews")
    }

// ------------- Header End ----------------


// ------------- Welcome Text Start ---------------
// only shown when account configuration is incomplete

    Item {
        anchors { fill: parent; topMargin: 90; }
        visible: configState === 1
        width: parent.width - 20

        Text {
            id: welcomeText
            anchors { top: parent.top; horizontalCenter: parent.horizontalCenter }
            text: qsTr("<b>Welcome to ocNews</b><br />With these app you can synchronize and view your ownCloud News App content on your smartphone. Before you can start, you have to setup your ownCloud server account.")
            width: parent.width - 30
            wrapMode: Text.WordWrap
            font.pointSize: 17
            font.weight: Font.Light
            horizontalAlignment: Text.AlignHCenter
            color: theme.inverted ? "white" : "black"
            textFormat: Text.StyledText
        }

        Text {
            id: updateHint
            anchors { top: welcomeText.bottom; topMargin: 20; horizontalCenter: parent.horizontalCenter }
            text: qsTr("<i>If you have updated ocNews from a version prior to 1.3.0, you have to resetup your account and resync your data, because of changed storage locations.</i>")
            width: parent.width - 30
            wrapMode: Text.WordWrap
            font.pointSize: 17
            font.weight: Font.Light
            horizontalAlignment: Text.AlignHCenter
            color: theme.inverted ? "white" : "black"
            textFormat: Text.StyledText
        }

        Button {
            id: privacyPolicyButton
            text: qsTr("Privacy policy")
            anchors { top: updateHint.bottom; topMargin: 20; horizontalCenter: parent.horizontalCenter }
            onClicked: {
                mainViewPrivacySheet.policy = dbus.getSetting("display/privacypolicy", false) == "true" ? true : false
                mainViewPrivacySheet.open();
            }

        }

        Button {
            id: setupAccountButton
            text: qsTr("Setup account")
            anchors { top: privacyPolicyButton.bottom; topMargin: 15; horizontalCenter: parent.horizontalCenter }
            onClicked: launcher.launch("/usr/bin/invoker", "-w,--type=d,--single-instance,/usr/lib/AccountSetup/bin/ocnewsplugin")
        }

    }
// ------------- Welcome Text End ---------------


// ------------- Account not enabled Start   ----------

    Item {
        anchors { fill: parent; topMargin: 90; }
        visible: configState === 2
        width: parent.width - 20

        Text {
            id: accountNotSetText
            anchors { top: parent.top; horizontalCenter: parent.horizontalCenter }
            text: qsTr("Your ocNews account is currently disabled. In order to use ocNews you have to enable your account.")
            width: parent.width - 30
            wrapMode: Text.WordWrap
            font.pointSize: 17
            font.weight: Font.Light
            horizontalAlignment: Text.AlignHCenter
            color: theme.inverted ? "white" : "black"
            textFormat: Text.StyledText
        }

        Button {
            id: enableAccountButton
            text: qsTr("Setup account")
            anchors { top: accountNotSetText.bottom; topMargin: 15; horizontalCenter: parent.horizontalCenter }
            onClicked: launcher.launch("/usr/bin/invoker", "--single-instance,--type=m,--splash=/usr/share/themes/blanco/meegotouch/images/splash/meegotouch-accounts-splash.jpg,/usr/bin/accounts-ui")
        }
    }

// ------------- Account not enabled Start   ----------


// ------------- Main View Start ---------------
    ListView {
        id: folderList
        anchors { fill: parent; topMargin: 71; leftMargin: 20; rightMargin: 20 }
        model: viewMode === 0 ? folderModelSql : combinedModelSql
        visible: configState === 0
        delegate: FolderListDelegate {
            subtitleColor: "grey"
            onClicked: {
                if (type === "1")  {
                    openFile("FeedListView.qml", {folderId: id, folderName: title})
                } else if (type === "0") {
                    openFile("ItemListView.qml", {feedId: id, feedName: title})
                } else if (type === "-1" && id === "0") {
                    openFile("SpecialItemListView.qml", { pageName: title, feedType: "all" })
                } else if (type === "-1" && id === "1") {
                    openFile("SpecialItemListView.qml", { pageName: title, feedType: "starred" })
                }
            }
            onPressAndHold: {
                contextMenuEffect.play();
                if (type === "1") {
                    folderContextMenu.folderId = id
                    folderContextMenu.folderName = title
                    folderContextMenu.open()
                } else if (type === "0") {
                    feedsContextMenu.feedId = id
                    feedsContextMenu.feedName = title
                    feedsContextMenu.open()
                }
             }
        }
        section.property: "folderName"
        section.delegate: GroupHeader { text: section }
        onModelChanged: model.refresh()
    }

    ScrollDecorator {
        flickableItem: folderList
        anchors { right: parent.right; rightMargin: -folderList.anchors.rightMargin }
    }

    ThemeEffect { id: contextMenuEffect; effect: "LongPress" }


// ------------- Main View End ---------------



// ------------- ToolBar Start -------------
    ToolBarLayout {
        id: mainViewTools
        ToolIcon {
            id: addIcon
            platformIconId: "toolbar-add"
            anchors.left: (parent === undefined) ? undefined : parent.left
            onClicked: { (addMenu.status === DialogStatus.Closed) ? addMenu.open() : addMenu.close(); settingsMenu.close(); }
        }
        ToolIcon {
            id: updaterIcon
            platformIconId: operationRunning ? "toolbar-refresh-dimmed" : "toolbar-refresh"
            enabled: !operationRunning
            onClicked: { operationRunning = true; updater.startUpdate(); }
        }
        ToolIcon {
            platformIconId: "toolbar-view-menu"
            anchors.right: (parent === undefined) ? undefined : parent.right
            onClicked: { (settingsMenu.status === DialogStatus.Closed) ? settingsMenu.open() : settingsMenu.close(); addMenu.close(); }
        }
    }


    Menu {
        id: settingsMenu
        visualParent: pageStack
        MenuLayout {
            MenuItem {
                text: qsTr("Settings")
                onClicked: openFile("../Pages/Settings.qml")
            }
            MenuItem {
                text: qsTr("About")
                onClicked: openFile("../Pages/About.qml")
            }
            MenuItem {
                text: qsTr("Quit completely")
                onClicked: quitCompletely.open()
            }
        }
    }

    Menu {
        id: addMenu
        visualParent: pageStack
        MenuLayout {
            MenuItem {
                text: qsTr("Add Folder")
                enabled: !operationRunning
                onClicked: mainViewAddFolder.open()
            }
            MenuItem {
                text: qsTr("Add Feed")
                enabled: !operationRunning
                onClicked: {
                    mainViewAddFeed.buildFolderList()
                    mainViewAddFeed.open()
                }
            }
        }
    }
// ----------------- ToolBar End -------------



// ----------------- Context Menu Start --------------

    ContextMenu {
        id: folderContextMenu
        property string folderId
        property string folderName
        MenuLayout {
            MenuItem {
                text: qsTr("Mark folder as read")
                enabled: !operationRunning
                onClicked: {
                    markReadQuery.id = folderContextMenu.folderId
                    markReadQuery.name = folderContextMenu.folderName
                    markReadQuery.open()
                }
            }
            MenuItem {
                text: qsTr("Update folder")
                enabled: !operationRunning
                onClicked: {
                    items.updateItems("0", "1", folderContextMenu.folderId)
                    mainViewHeader.indicatorState = "RUNNING"
                }
            }
            MenuItem {
                text: qsTr("Rename folder")
                enabled: !operationRunning
                onClicked: {
                    mainViewRenameFolder.folderId = folderContextMenu.folderId
                    mainViewRenameFolder.folderName = folderContextMenu.folderName
                    mainViewRenameFolder.open()
                }
            }
            MenuItem {
                text: qsTr("Delete folder")
                enabled: !operationRunning
                onClicked: {
                    deleteQuery.id = folderContextMenu.folderId
                    deleteQuery.name = folderContextMenu.folderName
                    deleteQuery.type = "1"
                    deleteQuery.open()
                }
            }
        }
    }

    ContextMenu {
        id: feedsContextMenu
        property string feedId
        property string feedName
        MenuLayout {
            MenuItem {
                text: qsTr("Mark feed as read")
                enabled: !operationRunning
                onClicked: {
                    operationRunning = true
                    feeds.markFeedRead(feedsContextMenu.feedId)
                }
            }
            MenuItem {
                text: qsTr("Update feed")
                enabled: !operationRunning
                onClicked: {
                    operationRunning = true
                    items.updateItems("0", "0", feedsContextMenu.feedId)
                }
            }
            MenuItem {
                text: qsTr("Move feed")
                enabled: !operationRunning
                onClicked: {
                    mainViewMoveFeed.feedId = feedsContextMenu.feedId
                    mainViewMoveFeed.feedName = feedsContextMenu.feedName
                    mainViewMoveFeed.buildFolderList()
                    mainViewMoveFeed.open()
                }
            }
            MenuItem {
                text: qsTr("Delete feed")
                enabled: !operationRunning
                onClicked: {
                    deleteQuery.id = feedsContextMenu.feedId
                    deleteQuery.name = feedsContextMenu.feedName
                    deleteQuery.type = "0"
                    deleteQuery.open()
                }
            }
        }
    }

// ----------------- Context Menu End --------------

// ----------------- Sheets Start --------------------

    AddFolderSheet {
        id: mainViewAddFolder
    }

    AddFeedSheet {
        id: mainViewAddFeed
    }

    RenameFolderSheet {
        id: mainViewRenameFolder
    }

    MoveFeedSheet {
        id: mainViewMoveFeed
    }

    PrivacySheet {
        id: mainViewPrivacySheet
    }

// ----------------- Sheets End --------------------


// ----------------- Dialogs Start --------------------

    QueryDialog {
        id: deleteQuery
        property string id
        property string name
        property string type
        icon: "image://theme/icon-m-bootloader-warning"
        acceptButtonText: qsTr("Delete")
        rejectButtonText: qsTr("Cancel")
        message: type === "1" ? qsTr("Do you really want to delete this folder? All feeds and posts in this folder will then be deleted, too.") : ""
        titleText: type === "1" ? qsTr("Delete folder %1?").arg(name) : qsTr("Delete feed %1?").arg(name)
        onAccepted: {
            operationRunning = true
            type === "1" ? folders.deleteFolder(id) : feeds.deleteFeed(id)
        }
    }

    QueryDialog {
        id: markReadQuery
        property string id
        property string name
        icon: "image://theme/icon-m-bootloader-warning"
        acceptButtonText: qsTr("Yes")
        rejectButtonText: qsTr("No")
        message: qsTr("Do you really want to mark the whole content of this folder as read?")
        titleText: qsTr("Mark folder %1 as read?").arg(name)
        onAccepted: {
            operationRunning = true
            folders.markFolderRead(id)
        }
    }

    QueryDialog {
        id: quitCompletely
        icon: "image://theme/icon-m-bootloader-warning"
        acceptButtonText: qsTr("Yes")
        rejectButtonText: qsTr("No")
        message: qsTr("If you quit both, reader and daemon/engine, there are no updates in the background anymore.")
        titleText: qsTr("Quit completely?")
        onAccepted: {
            dbus.quitEngine()
            Qt.quit()
        }
    }

// ----------------- Dialogs End --------------------

}
