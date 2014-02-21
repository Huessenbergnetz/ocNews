import QtQuick 2.0
import Sailfish.Silica 1.0

import "../Common"
import "../Views"
import "../JS/globals.js" as GLOBALS

ListItem {
    id: specialItemListItem

    property string feedId
    property string searchString
    property int handleRead
    property bool sortAsc
    property string feedType

    contentHeight: Theme.itemSizeExtraLarge

    menu: itemContextMenu

    onClicked: {
        if (model.unread) busyIndicator.state = "RUNNING"
        GLOBALS.previousContentY = ListView.view.contentY
        pageStack.push(Qt.resolvedUrl("../Views/SingleItemView.qml"), {itemId: model.itemId, handleRead: specialItemListItem.handleRead, sortAsc: specialItemListItem.sortAsc, searchString: specialItemListItem.searchString, feedType: specialItemListItem.feedType, parentFeedId: specialItemListItem.feedId})
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
            width: parent.width -iconCol.width

            Text {
                id: mainText
                text: Theme.highlightText(model.title, searchString, model.unread ? Theme.highlightColor : Theme.secondaryHighlightColor)
                width: mainRow.width - starImage.width
                color: if (model.unread) { specialItemListItem.highlighted ? Theme.highlightColor : Theme.primaryColor } else { specialItemListItem.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor }
                maximumLineCount: 2
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                elide: Text.ElideRight
                textFormat: Text.StyledText
                font.pixelSize: Theme.fontSizeSmall
            }

            Text {
                id: feedText
                text: model.feedName
                textFormat: Text.PlainText
                color: specialItemListItem.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
                font.pixelSize: Theme.fontSizeTiny
            }

            Text {
                id:dateText
                text: model.pubDate
                textFormat: Text.PlainText
                color: specialItemListItem.highlighted ? Theme.secondaryHighlightColor : Theme.secondaryColor
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
                onClicked: {
                    GLOBALS.previousContentY = specialItemListItem.ListView.view.contentY
                    busyIndicator.state = "RUNNING"
                    operationRunning = true
                    model.unread ? items.markItems("read", specialItemListItem.markParams()) : items.markItems("unread", specialItemListItem.markParams()) }
            }
            MenuItem {
                text: model.starred ? qsTr("Remove from favourites") : qsTr("Add to favourites")
                enabled: !operationRunning
                onClicked: {
                    GLOBALS.previousContentY = specialItemListItem.ListView.view.contentY
                    busyIndicator.state = "RUNNING"
                    operationRunning = true
                    model.starred ? items.starItems("unstar", specialItemListItem.starParams() ) : items.starItems("star", specialItemListItem.starParams() ) }
            }
        }
    }

}
