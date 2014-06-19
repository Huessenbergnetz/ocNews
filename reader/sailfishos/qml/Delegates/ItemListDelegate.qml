import QtQuick 2.0
import Sailfish.Silica 1.0

import "../Common"
import "../Views"

ListItem {
    id: itemListItem

    property string feedId
    property string searchString
    property int handleRead
    property bool sortAsc
    property string feedType

    contentHeight: Math.max(textCol.height, iconCol.height) + Theme.paddingSmall
    contentWidth: parent.width
    menu: itemContextMenu

    onClicked: {
        if (model.unread) busyIndicator.state = "RUNNING"
        pageStack.push(Qt.resolvedUrl("../Views/SingleItemView.qml"), {itemId: model.itemId, searchString: itemListItem.searchString, handleRead: itemListItem.handleRead, sortAsc: itemListItem.sortAsc, feedType: itemListItem.feedType, parentFeedId: itemListItem.feedId })
    }

    function starParams() {
        var params={};
        params[feedId]=model.guidHash;
        return params;
    }

    function markParams() {
        var params = [];
        params[0]=model.itemId;
        return params;
    }

    Item {
        width: gi.width
        height: gi.height
        x: -(width / 2)
        y: -(height / 4)

        GlassItem {
            id: gi
            width: Theme.itemSizeExtraSmall
            height: width
            color: Theme.highlightColor
            opacity: model.unread ? 1 : 0
            Behavior on opacity { NumberAnimation { duration: 250; easing.type: Easing.InOutQuad } }
        }
    }

    Row {
        id: mainRow
        anchors.fill: parent
        anchors.leftMargin: Theme.paddingLarge
        anchors.rightMargin: Theme.paddingLarge
        spacing: 8

        Column {
            id: textCol
            width: parent.width - iconCol.width

            Text {
                id: mainText
                text: Theme.highlightText(model.title, searchString, Theme.highlightColor)
                width: parent.width
                color: itemListItem.highlighted ? Theme.highlightColor : Theme.primaryColor
                maximumLineCount: 2
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                elide: Text.ElideRight
                textFormat: Text.StyledText
                font.pixelSize: Theme.fontSizeSmall
            }

            Text {
                id: excerptText
                text: model.excerpt
                textFormat: Text.PlainText
                color: if (model.unread) { itemListItem.highlighted ? Theme.highlightColor : Theme.primaryColor } else { itemListItem.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor }
                font.pixelSize: Theme.fontSizeExtraSmall
                width: parent.width
                maximumLineCount: 3
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                elide: Text.ElideRight
                visible: text !== ""
            }

            Text {
                id:dateText
                text: model.pubDate
                textFormat: Text.PlainText
                color: itemListItem.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
                font.pixelSize: Theme.fontSizeTiny
            }
        }

        Column {
            id: iconCol
            width: 32

            Image {
                id: starImage
                visible: model.starred && !busyIndicator.visible
                width: 32
                height: 32
                sourceSize.width: 32
                sourceSize.height: 32
                source: "image://theme/icon-s-favorite"
            }

            BusyIndicator {
                id: busyIndicator
                size: BusyIndicatorSize.Small
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
                    target: items
                    onMarkedItemsError: busyIndicator.state = "NORMAL"
                    onMarkedItemsSuccess: busyIndicator.state = "NORMAL"
                    onStarredItemsError: busyIndicator.state = "NORMAL"
                    onStarredItemsSuccess: busyIndicator.state = "NORMAL"
                }
            }

            Image {
                id: enclosureImage
                visible: model.enclosureLink !== ""
                width: 32
                height: 32
                sourceSize.width: 32
                sourceSize.height: 32
                source: "image://theme/icon-s-attach"
            }
        }
    }

    Component {
        id: itemContextMenu
        ContextMenu {
            MenuItem {
                text: model.unread ? qsTr("Mark as read") : qsTr("Mark as unread")
                enabled: !operationRunning
                onClicked: { operationRunning = true; busyIndicator.state = "RUNNING"; model.unread ? items.markItems("read", itemListItem.markParams()) : items.markItems("unread", itemListItem.markParams()) }
            }
            MenuItem {
                text: model.starred ? qsTr("Remove from favorites") : qsTr("Add to favorites")
                enabled: !operationRunning
                onClicked: { operationRunning = true; busyIndicator.state = "RUNNING"; model.starred ? items.starItems("unstar", itemListItem.starParams() ) : items.starItems("star", itemListItem.starParams() ) }
            }
            MenuItem {
                text: qsTr("Mark as read up to this point")
                enabled: !operationRunning
                onClicked: { operationRunning = true; busyIndicator.state = "RUNNING"; items.markItemsTillThis("read", model.pubDateInt, itemListItem.feedId) }
            }
            MenuItem {
                text: qsTr("Mark as unread up to this point")
                enabled: !operationRunning
                onClicked: { operationRunning = true; busyIndicator.state = "RUNNING"; items.markItemsTillThis("unread", model.pubDateInt, itemListItem.feedId) }
            }
        }
    }
}
