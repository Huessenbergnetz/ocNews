import QtQuick 2.0
import Sailfish.Silica 1.0

import "../Common"
import "../Views"

ListItem {
    id: itemListItem

    property string feedId
    property string searchString

    contentHeight: Theme.itemSizeLarge
    menu: itemContextMenu

    onClicked: {
        if (model.unread) busyIndicator.state = "RUNNING"
        pageStack.push(Qt.resolvedUrl("../Views/SingleItemView.qml"), {itemId: model.itemId})
    }

    function starParams() {
        var params={};
        params[feedId]=model.guidHash;
        return params;
    }

    function markParams() {
        var params = new Array();
        params[0]=model.itemId;
        return params;
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
                text: Theme.highlightText(model.title, searchString, model.unread ? Theme.highlightColor : Theme.secondaryHighlightColor)
                width: parent.width
                color: if (model.unread) { itemListItem.highlighted ? Theme.highlightColor : Theme.primaryColor } else { itemListItem.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor }
                maximumLineCount: 2
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                elide: Text.ElideRight
                textFormat: Text.StyledText
                font.pixelSize: Theme.fontSizeSmall
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
