import QtQuick 2.0
import Sailfish.Silica 1.0

import "../Delegates"
import "../Dialogs"
import "../Common"

Page {
    id: feedListView

    property string folderId
    property string folderName

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
        onRenamedFolderSuccess: feedListView.folderName = newfoldername
    }
    Connections {
        target: updater
        onUpdateFinished: feedsModelSql.refresh(folderId)
    }

    SilicaListView {
        id: feedList

        PullDownMenu {
            id: feedListViewPully
            busy: operationRunning
            MenuItem {
                id: deleteFolder
                text: qsTr("Delete Folder")
                enabled: !operationRunning
                onClicked: removeFolder(feedListView.folderId, feedListView.folderName)
            }
            MenuItem {
                id: rename
                text: qsTr("Rename folder")
                enabled: !operationRunning
                onClicked: {
                    var dialog = pageStack.push(Qt.resolvedUrl("../Dialogs/RenameFolder.qml"), {folderId: folderId, folderName: folderName})
                    dialog.accepted.connect(function() { operationRunning = true })
                }
            }
            MenuItem {
                id: add
                text: qsTr("Add feed")
                enabled: !operationRunning
                onClicked: {
                    var dialog = pageStack.push(Qt.resolvedUrl("../Dialogs/CreateFeed.qml"), {folderId: folderId, folderName: folderName})
                    dialog.accepted.connect(function() { operationRunning = true })
                }
            }
            MenuItem {
                id: markAsRead
                text: qsTr("Mark folder as read")
                enabled: !operationRunning
                onClicked: markFolderRead(feedListView.folderId, feedListView.folderName)
            }
            MenuItem {
                id: updateFolder
                text: qsTr("Update folder")
                enabled: !operationRunning
                onClicked: { items.updateItems("0", "1", feedListView.folderId); operationRunning = true }
            }
        }

        ViewPlaceholder {
            enabled: feedList.count <= 1
            text: qsTr("This folder is empty. Add some feeds to it.")
        }

        anchors.fill: parent

        header: PageHeader { id: header; title: operationRunning ? qsTr("Update running...") : feedListView.folderName }

        model: feedsModelSql

        delegate: FeedListDelegate { folderId: feedListView.folderId; folderName: feedListView.folderName; }

        VerticalScrollDecorator {}

        FancyScroller {}

        PushUpMenu {
            id: feedListViewPushy
            enabled: feedList.contentHeight >= feedList.height
            visible: feedList.contentHeight >= feedList.height
            MenuItem {
                id: goToTop
                text: qsTr("Scroll to top")
                onClicked: feedList.scrollToTop()
            }
        }

    }

    function removeFolder(folderId, folderName)
    {
        remorsePop.execute(qsTr("Deleting folder %1").arg(folderName), function() { operationRunning = true; folders.deleteFolder(folderId) } );
    }

    function markFolderRead(folderId, folderName)
    {
        remorsePop.execute(qsTr("Marking folder %1 as read").arg(folderName), function() { operationRunning = true; folders.markFolderRead(folderId); } );
    }

    RemorsePopup {
        id: remorsePop
    }
}
