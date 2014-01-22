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
        onCreatedFeedSuccess: { feedsModelSql.refresh(folderId); feedListViewPully.busy = false }
        onCreatedFeedError: { feedListViewPully.busy = false }
        onDeletedFeedSuccess: feedsModelSql.refresh(folderId)
        onMarkedReadFeedSuccess: feedsModelSql.refresh(folderId)
    }
    Connections {
        target: items
        onUpdatedItemsSuccess: { feedsModelSql.refresh(folderId); feedListViewPully.busy = false; updateFolder.enabled = true }
        onUpdatedItemsError: { feedListViewPully.busy = false; updateFolder.enabled = true }
        onRequestedItemsSuccess: { feedsModelSql.refresh(folderId); feedListViewPully.busy = false; updateFolder.enabled = true }
        onStarredItemsSuccess: feedsModelSql.refresh(folderId)
        onMarkedItemsSuccess: feedsModelSql.refresh(folderId)
    }
    Connections {
        target: folders
        onDeletedFolderSuccess: pageStack.pop()
        onDeletedFolderError: feedListViewPully.busy = false
        onMarkedReadFolderSuccess: { feedsModelSql.refresh(folderId); feedListViewPully.busy = false }
        onMarkedReadFolderError: feedListViewPully.busy = false
        onRenamedFolderSuccess: { feedListViewPully.busy = false; feedListView.folderName = newfoldername }
        onRenamedFolderError: feedListViewPully.busy = false
    }
    Connections {
        target: updater
        onUpdateError: { feedListViewPully.busy = false; updateFolder.enabled = true }
        onUpdateFinished: { feedListViewPully.busy = false; updateFolder.enabled = true }
        onUpdateStarted: { feedListViewPully.busy = true; updateFolder.enabled = false }
    }

    SilicaListView {
        id: feedList

        PullDownMenu {
            id: feedListViewPully
            busy: updater.isUpdateRunning() ? true : false
            MenuItem {
                id: deleteFolder
                text: qsTr("Delete Folder")
                onClicked: removeFolder(feedListView.folderId, feedListView.folderName)
            }
            MenuItem {
                id: rename
                text: qsTr("Rename folder")
                onClicked: {
                    var dialog = pageStack.push(Qt.resolvedUrl("../Dialogs/RenameFolder.qml"), {folderId: folderId, folderName: folderName})
                    dialog.accepted.connect(function() { feedListViewPully.busy = true; })
                }
            }
            MenuItem {
                id: add
                text: qsTr("Add feed")
                onClicked: {
                    var dialog = pageStack.push(Qt.resolvedUrl("../Dialogs/CreateFeed.qml"), {folderId: folderId, folderName: folderName})
                    dialog.accepted.connect(function() { feedListViewPully.busy = true; })
                }
            }
            MenuItem {
                id: markAsRead
                text: qsTr("Mark folder as read")
                onClicked: markFolderRead(feedListView.folderId, feedListView.folderName)
            }
            MenuItem {
                id: updateFolder
                text: qsTr("Update folder")
                onClicked: { items.updateItems("0", "1", feedListView.folderId); feedListViewPully.busy = true; updateFolder.enabled = false }
            }
        }

        anchors.fill: parent

        header: PageHeader { id: header; title: updater.isUpdateRunning() ? "Update running..." : feedListView.folderName }

        model: feedsModelSql

        delegate: FeedListDelegate { folderId: feedListView.folderId; folderName: feedListView.folderName }

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
        remorsePop.execute(qsTr("Deleting folder %1").arg(folderName), function() { feedListViewPully.busy = true; folders.deleteFolder(folderId) } );
    }

    function markFolderRead(folderId, folderName)
    {
        remorsePop.execute(qsTr("Marking folder %1 as read").arg(folderName), function() { feedListViewPully.busy = true; folders.markFolderRead(folderId); } );
    }

    RemorsePopup {
        id: remorsePop
    }
}