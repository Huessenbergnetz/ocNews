import QtQuick 2.0
import Sailfish.Silica 1.0

import "../Common"
import "../Views"
import "../Dialogs"

ListItem {
    id: folderListItem

//    contentHeight: model.type === "1" ? Theme.itemSizeMedium : Theme.itemSizeSmall
    contentHeight: Theme.itemSizeSmall

    showMenuOnPressAndHold: model.type === "-1" ? false : true

    menu: model.type === "1" ? folderContextMenu : feedContextMenu

    onClicked: {
        if (type === "1")  {
            pageStack.push(Qt.resolvedUrl("../Views/FeedListView.qml"), {folderId: model.id, folderName: model.title})
        } else if (type === "0") {
            pageStack.push(Qt.resolvedUrl("../Views/ItemListView.qml"), {feedId: id, feedName: title})
        } else if (type === "-1" && id === "0") {
            pageStack.push(Qt.resolvedUrl("../Views/SpecialItemListView.qml"), {pageName: title, feedType: "all"})
        } else if (type === "-1" && id === "1") {
            pageStack.push(Qt.resolvedUrl("../Views/SpecialItemListView.qml"), {pageName: title, feedType: "starred"})
        }
    }

    Row {
        id: mainRow
        anchors.fill: parent
        anchors.leftMargin: Theme.paddingLarge
        anchors.rightMargin: Theme.paddingLarge
        spacing: 8

        Item {
            width: 64
            height: 64
            anchors.verticalCenter: parent.verticalCenter

            Image {
                anchors.centerIn: parent
                visible: true
//                width: model.iconSource ? (parseInt(model.iconWidth, 10) > 32) ? 64 : 32 : 64
//                height: model.iconSource ? (parseInt(model.iconHeight, 10) > 32) ? 64 : 32 : 64
//                width: model.type === "1" ? 64 : 32
//                height: model.type === "1" ? 64 : 32
                width: 32
                height: 32
                sourceSize.width: model.iconSource ? parseInt(model.iconWidth, 10) : 64
                sourceSize.height: model.iconSource ? parseInt(model.iconHeight, 10) : 64
                source: model.iconSource ? model.iconSource : (model.type === "1") ? "image://theme/icon-m-folder" : "/usr/share/harbour-ocnews-reader/icons/icon-m-rss.png"
            }
        }

        Column {
            anchors.verticalCenter: parent.verticalCenter

            Label {
                id: itemTitle
                text: model.title
                color: folderListItem.highlighted ? Theme.highlightColor : Theme.primaryColor
                maximumLineCount: 1
                truncationMode: TruncationMode.Fade
                textFormat: Text.PlainText
            }

            Text {
                id: subText
                text: model.feedCount == 0 ? qsTr("Empty folder") : qsTr("%1 unread posts in %2 feeds").arg(model.unreadCount).arg(model.feedCount);
                color: folderListItem.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
//                visible: model.type === "1" ? true : false
                visible: false
                textFormat: Text.PlainText
                font.pixelSize: Theme.fontSizeSmall
            }

        }

    }

    CountBubble {
        id: cntBub
        value: model.unreadCount
//        visible: model.type !== "1" ? true : false
        visible: !busyIndicator.visible
        anchors { right: parent.right; rightMargin: Theme.paddingLarge; verticalCenter: parent.verticalCenter }
    }

    BusyIndicator {
        id: busyIndicator
        size: BusyIndicatorSize.Small
        anchors { right: parent.right; rightMargin: Theme.paddingLarge; verticalCenter: parent.verticalCenter }
        state: "NORMAL"
        states: [
            State {
                name: "RUNNING"
                PropertyChanges { target: busyIndicator; visible: true; running: true; }
            },
            State {
                name: "NORMAL"
                PropertyChanges { target: busyIndicator; visible: false; running: false; }
            }
        ]
        Connections {
            target: feeds
            onMarkedReadFeedError: busyIndicator.state = "NORMAL"
            onMarkedReadFeedSuccess: busyIndicator.state = "NORMAL"
            onDeletedFeedError: busyIndicator.state = "NORMAL"
            onMovedFeedError: busyIndicator.state = "NORMAL"
        }
        Connections {
            target: items
            onUpdatedItemsError: busyIndicator.state = "NORMAL"
            onUpdatedItemsSuccess: busyIndicator.state = "NORMAL"
        }
        Connections {
            target: folders
            onMarkedReadFolderError: busyIndicator.state = "NORMAL"
            onMarkedReadFolderSuccess: busyIndicator.state = "NORMAL"
            onDeletedFolderError: busyIndicator.state = "NORMAL"
            onRenamedFolderError: busyIndicator.state = "NORMAL"
            onRenamedFolderSuccess: busyIndicator.state = "NORMAL"
        }
    }

    Component {
        id: folderContextMenu
        ContextMenu {
            MenuItem {
                text: qsTr("Update folder")
                onClicked: { busyIndicator.state = "RUNNING"; items.updateItems("0", "1", model.id) }
            }
            MenuItem {
                text: qsTr("Mark folder as read")
                onClicked: markFolderRead(model.id, model.title)
            }
            MenuItem {
                text: qsTr("Rename folder")
                onClicked: {
                    var dialog = pageStack.push(Qt.resolvedUrl("../Dialogs/RenameFolder.qml"), {folderId: model.id, folderName: model.title})
                    dialog.accepted.connect(function() { busyIndicator.state = "RUNNING"; })
                }
            }
            MenuItem {
                text: qsTr("Delete folder")
                onClicked: removeFolder(model.id, model.title)
            }
        }
    }

    Component {
        id: feedContextMenu
        ContextMenu {
            MenuItem {
                text: qsTr("Update feed")
                onClicked: { busyIndicator.state = "RUNNING"; items.updateItems("0", "0", model.id) }
            }
            MenuItem {
                text: qsTr("Mark feed as read")
                onClicked: { busyIndicator.state = "RUNNING"; feeds.markFeedRead(model.id) }
            }
            MenuItem {
                text: qsTr("Move feed")
                onClicked: {
                    var dialog = pageStack.push(Qt.resolvedUrl("../Dialogs/MoveFeed.qml"), {feedId: model.id, feedName: model.title, folderId: "0"})
                    dialog.accepted.connect(function() { busyIndicator.state = "RUNNING"; })
                }
            }
            MenuItem {
                text: qsTr("Delete feed")
                onClicked: removeFeed(model.id, model.title)
            }
        }
    }

    function removeFeed(feedId, feedName) {
        remorse.execute(folderListItem, qsTr("Deleting feed %1").arg(feedName), function() { busyIndicator.state = "RUNNING"; feeds.deleteFeed(feedId) } );
    }

    function removeFolder(folderId, folderName) {
        remorse.execute(folderListItem, qsTr("Deleting folder %1").arg(folderName), function() { busyIndicator.state = "RUNNING"; folders.deleteFolder(folderId) } );
    }

    function markFolderRead(folderId, folderName) {
        remorse.execute(folderListItem, qsTr("Mark folder %1 as read").arg(folderName), function() { busyIndicator.state = "RUNNING"; folders.markFolderRead(folderId) } );
    }

    RemorseItem {
        id: remorse
    }
}