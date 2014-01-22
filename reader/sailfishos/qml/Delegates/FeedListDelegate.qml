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

    onClicked: { type === "0" ? pageStack.push(Qt.resolvedUrl("../Views/ItemListView.qml"), {feedId: model.id, feedName: model.title}) :
                                pageStack.push(Qt.resolvedUrl("../Views/SpecialItemListView.qml"), { id: feedListItem.folderId, pageName: qsTr("All") + " - " + feedListItem.folderName, feedType: "folder" })
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
                color: feedListItem.highlighted ? Theme.highlightColor : Theme.primaryColor
                maximumLineCount: 1
                truncationMode: TruncationMode.Fade
                textFormat: Text.PlainText
                width: parent.width - 130
            }

            Text {
                id: subText
                text: model.feedCount == 0 ? qsTr("Empty folder") : qsTr("%1 unread posts in %2 feeds").arg(model.unreadCount).arg(model.feedCount);
                color: feedListItem.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
                visible: model.type === "1" ? true : false
                textFormat: Text.PlainText
                font.pixelSize: Theme.fontSizeSmall
            }

        }

    }

    CountBubble {
        id: unreadCount
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
                    var dialog = pageStack.push(Qt.resolvedUrl("../Dialogs/MoveFeed.qml"), {feedId: model.id, feedName: model.title, folderId: folderId})
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
        remorse.execute(feedListItem, qsTr("Deleting feed %1").arg(feedName), function() { busyIndicator.state = "RUNNING"; feeds.deleteFeed(feedId) } );
    }

    RemorseItem {
        id: remorse
    }
}
