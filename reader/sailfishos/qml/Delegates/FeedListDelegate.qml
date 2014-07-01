import QtQuick 2.0
import Sailfish.Silica 1.0

import "../Common"
import "../Views"

ListItem {
    id: feedListItem

    property string folderId
    property string folderName

    contentHeight: model.type === "1" ? Theme.itemSizeMedium : Theme.itemSizeSmall

    showMenuOnPressAndHold: model.type === "-1" ? false : true

    menu: feedContextMenu

    ListView.onAdd: AddAnimation { target: feedListItem }
    ListView.onRemove: animateRemoval(feedListItem)

    onClicked: {
        switch(model.type)
        {
        case 0:
            itemsModelSql.feedId = model.id
            pageStack.push(Qt.resolvedUrl("../Views/ItemListView.qml"), {feedName: model.title})
            break;
        case 1:
            specialItemsModelSql.type = model.type
            specialItemsModelSql.id = model.id
//            pageStack.push(Qt.resolvedUrl("../Views/SpecialItemListView.qml"), {pageName: feedListItem.folderName})
            pageStack.push(Qt.resolvedUrl("../Views/SpecialItemListView.qml"), {pageName: feedListItem.folderName, feedId: model.id, feedType: model.type})
            break;
        default:
            break;
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
                asynchronous: true
                visible: true
                width: model.type === "1" ? 64 : 32
                height: model.type === "1" ? 64 : 32
                sourceSize.width: model.iconSource ? parseInt(model.iconWidth, 10) : 64
                sourceSize.height: model.iconSource ? parseInt(model.iconHeight, 10) : 64
                source: model.iconSource ? model.iconSource : (model.type === "1") ? "image://theme/icon-m-folder" : "/usr/share/harbour-ocnews-reader/icons/icon-m-rss.png"
            }
        }

        Column {
            anchors.verticalCenter: parent.verticalCenter
            width: parent.width

            Label {
                id: itemTitle
                text: model.title
                color: if (model.unreadCount > 0) { feedListItem.highlighted ? Theme.highlightColor : Theme.primaryColor } else { feedListItem.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor }
                maximumLineCount: 1
                truncationMode: TruncationMode.Fade
                textFormat: Text.PlainText
                width: parent.width - 130
            }

        }

    }

    CountBubble {
        id: unreadCount
        value: model.unreadCount
        visible: !busyIndicator.visible
        anchors { right: parent.right; rightMargin: Theme.paddingLarge; verticalCenter: parent.verticalCenter }
        color: if (model.unreadCount > 0) { feedListItem.highlighted ? Theme.highlightColor : Theme.primaryColor } else { feedListItem.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor }
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
    }


    Component {
        id: feedContextMenu
        ContextMenu {
            MenuItem {
                text: qsTr("Update")
                enabled: !operationRunning
                onClicked: { busyIndicator.state = "RUNNING"; operationRunning = true; items.updateItems("0", "0", model.id) }
            }
            MenuItem {
                text: qsTr("Mark as read")
                enabled: !operationRunning
                onClicked: { busyIndicator.state = "RUNNING"; operationRunning = true; feeds.markFeedRead(model.id) }
            }
            MenuItem {
                text: qsTr("Move")
                enabled: !operationRunning
                onClicked: {
                    var dialog = pageStack.push(Qt.resolvedUrl("../Dialogs/MoveFeed.qml"), {feedId: model.id, feedName: model.title, folderId: folderId})
                    dialog.accepted.connect(function() { operationRunning = true; busyIndicator.state = "RUNNING"; })
                }
            }
            MenuItem {
                text: qsTr("Rename")
                enabled: !operationRunning
                onClicked: {
                    var dialog = pageStack.push(Qt.resolvedUrl("../Dialogs/RenameFeed.qml"), {feedId: model.id, feedName: model.title})
                    dialog.accepted.connect(function() { operationRunning = true })
                }
            }
            MenuItem {
                text: qsTr("Delete")
                enabled: !operationRunning
                onClicked: removeFeed(model.id, model.title)
            }
        }
    }

    function removeFeed(feedId, feedName) {
        remorse.execute(feedListItem, qsTr("Deleting feed %1").arg(feedName), function() { busyIndicator.state = "RUNNING"; operationRunning = true; feeds.deleteFeed(feedId) } );
    }

    RemorseItem {
        id: remorse
    }
}
