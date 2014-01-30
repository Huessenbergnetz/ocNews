import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.0
import QtMobility.feedback 1.1

import "../Common"
import "../Delegates"
import "../Sheets"

Page {
    id: feedListView
    tools: feedListViewTools
    orientationLock: PageOrientation.LockPortrait

    property string folderId
    property string folderName

    function openFile(file, properties) {
             var component = Qt.createComponent(file)
             if (component.status == Component.Ready)
                 pageStack.push(component, properties);
             else
                 console.log("Error loading component:", component.errorString());
         }

    Component.onCompleted: feedsModelSql.refresh(folderId)

    Connections {
        target: feeds
        onMovedFeedSuccess: feedsModelSql.refresh(folderId)
        onCreatedFeedSuccess: feedsModelSql.refresh(folderId)
        onDeletedFeedSuccess: feedsModelSql.refresh(folderId)
        onMarkedReadFeedSuccess: feedsModelSql.refresh(folderId)
    }
    Connections {
        target: items
        onUpdatedItemsSuccess: feedsModelSql.refresh(folderId)
        onRequestedItemsSuccess: feedsModelSql.refresh(folderId)
        onStarredItemsSuccess: feedsModelSql.refresh(folderId)
        onMarkedItemsSuccess: feedsModelSql.refresh(folderId)
    }
    Connections {
        target: folders
        onDeletedFolderSuccess: pageStack.pop()
        onMarkedReadFolderSuccess: feedsModelSql.refresh(folderId)
    }
    Connections {
        target: updater
        onUpdateFinished: feedsModelSql.refresh(folderId)
    }
    Connections {
        target: feedListViewAddFeed
        onAccepted: if (feedListViewAddFeed.feedAddressText !== "") operationRunning = true
    }
    Connections {
        target: feedListViewMoveFeed
        onAccepted: operationRunning = true
    }
    Connections {
        target: feedListViewRenameFolder
        onAccepted: if (feedListViewRenameFolder.folderName !== feedListViewRenameFolder.newFolderName && feedListViewRenameFolder.newFolderName !== "") operationRunning = true
    }

// ------------- Header Start ----------------

    Header {
        id: feedListViewHeader
        text: feedListView.folderName
        Connections {
            target: folders
            onRenamedFolderSuccess: feedListViewHeader.text = feedListViewRenameFolder.newFolderName
        }
    }

// ------------- Header End ----------------

    ListView {
        id: feedList
        anchors { fill: parent; topMargin: 71; leftMargin: 20; rightMargin: 20 }
        model: feedsModelSql
        delegate: FeedListDelegate {
                 subtitleColor: "grey"
                 onClicked: {
                     type === "0" ?
                                 openFile("ItemListView.qml", {feedId: id, feedName: title}) :
                                 openFile("SpecialItemListView.qml", { id: folderId, pageName: qsTr("All") + " - " + folderName, feedType: "folder" })
                 }
                 onPressAndHold: {
                     contextMenuEffect.play()
                     if (type === "0") {
                        feedsContextMenu.feedId = id
                        feedsContextMenu.feedName = title
                        feedsContextMenu.open()
                     }
                 }
             }
    }

    ScrollDecorator {
        flickableItem: feedList
        anchors { right: parent.right; rightMargin: -feedList.anchors.rightMargin }
    }

    ThemeEffect { id: contextMenuEffect; effect: "LongPress" }


// ------------- ToolBar Start -------------
    ToolBarLayout {
        id: feedListViewTools
        visible: true
        ToolIcon {
            platformIconId: "toolbar-back"
            anchors.left: (parent === undefined) ? undefined : parent.left
            onClicked: { pageStack.pop(); }
        }
        ToolIcon {
            id: updaterIcon
            platformIconId: operationRunning ? "toolbar-refresh-dimmed" : "toolbar-refresh"
            enabled: !operationRunning
            onClicked: {
                operationRunning = true
                items.updateItems("0", "1", feedListView.folderId);
            }
        }
        ToolIcon {
            platformIconId: "toolbar-view-menu"
            anchors.right: (parent === undefined) ? undefined : parent.right
            onClicked: { (foldersMenu.status === DialogStatus.Closed) ? foldersMenu.open() : foldersMenu.close(); }
        }
    }

    Menu {
        id: foldersMenu
        visualParent: pageStack
        MenuLayout {
            MenuItem {
                text: qsTr("Add feed")
                enabled: !operationRunning
                onClicked: {
                    feedListViewAddFeed.folderId = feedListView.folderId
                    feedListViewAddFeed.folderName = feedListView.folderName
                    feedListViewAddFeed.open()
                }
            }
            MenuItem {
                text: qsTr("Mark folder as read")
                enabled: !operationRunning
                onClicked: feedListViewMarkFolderReadQuery.open()
            }
            MenuItem {
                text: qsTr("Rename folder ")
                enabled: !operationRunning
                onClicked: {
                    feedListViewRenameFolder.folderId = feedListView.folderId
                    feedListViewRenameFolder.folderName = feedListView.folderName
                    feedListViewRenameFolder.open()
                }
            }
            MenuItem {
                text: qsTr("Delete folder")
                enabled: !operationRunning
                onClicked: feedListViewDeleteFolderQuery.open()
            }
        }
    }


// ----------------- ToolBar End -------------



// ----------------- Context Menu Start --------------

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
                    feeds.markFeedRead(feedsContextMenu.feedId);
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
                    feedListViewMoveFeed.feedId = feedsContextMenu.feedId
                    feedListViewMoveFeed.feedName = feedsContextMenu.feedName
                    feedListViewMoveFeed.buildFolderList()
                    feedListViewMoveFeed.open()
                }
            }
            MenuItem {
                text: qsTr("Delete feed")
                enabled: !operationRunning
                onClicked: {
                    feedListViewDeleteFeedQuery.feedId = feedsContextMenu.feedId
                    feedListViewDeleteFeedQuery.feedName = feedsContextMenu.feedName
                    feedListViewDeleteFeedQuery.open()
                }
            }
        }
    }

// ----------------- Context Menu End --------------



// ----------------- Sheets Start --------------------

    AddFeedSheet {
        id: feedListViewAddFeed
    }

    MoveFeedSheet {
        id: feedListViewMoveFeed
    }

    RenameFolderSheet {
        id: feedListViewRenameFolder
    }

// ----------------- Sheets End --------------------


// ----------------- Dialogs Start --------------------

    QueryDialog {
        id: feedListViewDeleteFolderQuery
        icon: "image://theme/icon-m-bootloader-warning"
        acceptButtonText: qsTr("Delete")
        rejectButtonText: qsTr("Cancel")
        message: qsTr("Do you really want to delete this folder? All feeds and posts in this folder will then be deleted, too.")
        titleText: qsTr("Delete folder %1?").arg(folderName)
        onAccepted: {
            operationRunning = true
            folders.deleteFolder(folderId)
        }
    }

    QueryDialog {
        id: feedListViewMarkFolderReadQuery
        icon: "image://theme/icon-m-bootloader-warning"
        acceptButtonText: qsTr("Yes")
        rejectButtonText: qsTr("No")
        message: qsTr("Do you really want to mark the whole content of this folder as read?")
        titleText: qsTr("Mark folder %1 as read?").arg(folderName)
        onAccepted: {
            operationRunning = true
            folders.markFolderRead(folderId)
        }
    }

    QueryDialog {
        id: feedListViewDeleteFeedQuery
        property string feedName
        property string feedId
        icon: "image://theme/icon-m-bootloader-warning"
        acceptButtonText: qsTr("Delete")
        rejectButtonText: qsTr("Cancel")
        titleText: qsTr("Delete feed %1?").arg(feedName)
        onAccepted: {
            operationRunning = true
            feeds.deleteFeed(feedId)
        }
    }

    // ----------------- Dialogs End --------------------


}
