import QtQuick 2.0
import Sailfish.Silica 1.0

import "../Delegates"
import "../Common"
import "../JS/globals.js" as GLOBALS

Page {
    id: itemListView

    property string feedId
    property string feedName

    property int handleRead: dbus.getSetting("display/handleread", 0)
    property bool sortAsc: dbus.getSetting("display/sortasc", false) == "true"

    Component.onCompleted: { console.log(handleRead); itemsModelSql.refresh(feedId, handleRead, sortAsc) }
    Component.onDestruction: GLOBALS.previousContentY = 0

    Connections {
        target: feeds
        onMarkedReadFeedSuccess: { itemsModelSql.refresh(feedId, handleRead, sortAsc) }
        onDeletedFeedSuccess: pageStack.pop()
    }
    Connections {
        target: items
        onUpdatedItemsSuccess: { itemsModelSql.refresh(feedId, handleRead, sortAsc); itemList.contentY = GLOBALS.previousContentY }
        onRequestedItemsSuccess: { itemsModelSql.refresh(feedId, handleRead, sortAsc); itemList.contentY = GLOBALS.previousContentY; }
        onStarredItemsSuccess: { itemsModelSql.refresh(feedId, handleRead, sortAsc); itemList.contentY = GLOBALS.previousContentY; }
        onMarkedItemsSuccess: { itemsModelSql.refresh(feedId, handleRead, sortAsc); itemList.contentY = GLOBALS.previousContentY; }
    }

    onSortAscChanged: itemsModelSql.refresh(feedId, handleRead, sortAsc)
    onHandleReadChanged: itemsModelSql.refresh(feedId, handleRead, sortAsc)

    Column {
        id: headerContainer

        width: itemListView.width

        PageHeader {
            id: pHeader
            title: operationRunning ? qsTr("Update running...") : itemListView.feedName
        }

        SearchField {
            id: searchField
            width: parent.width
            visible: false
            onTextChanged: { itemsModelSql.refresh(feedId, handleRead, sortAsc, text); searchFieldTimer.restart(); searchField.forceActiveFocus(); searchField.focus = true }
            placeholderText: qsTr("Search")
            EnterKey.onClicked: searchField.focus = false
            EnterKey.iconSource: "image://theme/icon-m-enter-close"

            Timer {
                id: searchFieldTimer; running: false; interval: 7000; repeat: false
                onTriggered: { if (searchField.text === "") { searchField.focus = false; searchField.visible = false; } }
            }
        }
    }

    SilicaListView {
        id: itemList
        anchors.fill: parent
        anchors.bottomMargin: sortingPanel.open ? sortingPanel.height * 1.5 : 0
        currentIndex: -1

        header: Item {
            id: header
            width: headerContainer.width
            height: headerContainer.height
            Component.onCompleted: headerContainer.parent = header
        }

        PullDownMenu {
            id: itemListViewPully
            busy: operationRunning
            MenuItem {
                id: deleteFeed
                enabled: !operationRunning
                text: qsTr("Delete feed")
                onClicked: removeFeed(itemListView.feedId, itemListView.feedName)
            }
            MenuItem {
                id: menuSort
                text: sortingPanel.open ? qsTr("Hide sorting options") : qsTr("Show sorting options")
                onClicked: sortingPanel.open = !sortingPanel.open
            }
            MenuItem {
                id: markFeedAsRead
                enabled: !operationRunning
                text: qsTr("Mark feed as read")
                onClicked: { operationRunning = true; feeds.markFeedRead(itemListView.feedId) }
            }
            MenuItem {
                id: updateFeed
                enabled: !operationRunning
                text: qsTr("Update feed")
                onClicked: { operationRunning = true; items.updateItems("0", "0", itemListView.feedId) }
            }
            MenuItem {
                id: showSearch
                text: searchField.visible ? qsTr("Hide search") : qsTr("Show search")
                onClicked: {
                    searchField.visible = !searchField.visible;
                    searchFieldTimer.running = !searchFieldTimer.running;
                    if (!searchField.visible) {searchField.text = ""; searchField.focus = false} else {searchField.focus = true}
                }
            }
        }

//        header: PageHeader { id: header; title: updater.isUpdateRunning() ? qsTr("Update running...") : itemListView.feedName }

        model: itemsModelSql

        delegate: ItemListDelegate { feedId: itemListView.feedId; searchString: searchField.text }

        VerticalScrollDecorator {}

        FancyScroller {}

        PushUpMenu {
            id: itemListViewPushy
            enabled: itemList.contentHeight >= itemList.height
            visible: itemList.contentHeight >= itemList.height
            MenuItem {
                id: goToTop
                text: qsTr("Scroll to top")
                onClicked: itemList.scrollToTop()
            }
        }
    }

    function removeFeed(feedId, feedName)
    {
        remorsePop.execute(qsTr("Deleting feed %1").arg(feedName), function() { operationRunning = true; feeds.deleteFeed(feedId) } );
    }

    RemorsePopup {
        id: remorsePop
    }

    DockedPanel {
        id: sortingPanel
        width: parent.width
        height: actCol.height + Theme.paddingLarge
        visible: open

        dock: Dock.Bottom

        Column {
            id: actCol
            width: parent.width

            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                text: sortAsc ? qsTr("Show newest on top") : qsTr("Show oldest on top")
                onClicked: sortAsc = !sortAsc
            }

            ListModel {
                id: handleReadModel
                ListElement { name: ""; value: 0 }
                ListElement { name: ""; value: 1 }
                ListElement { name: ""; value: 2 }
                Component.onCompleted: {
                    handleReadModel.get(0).name = qsTr("Show")
                    handleReadModel.get(1).name = qsTr("Hide")
                    handleReadModel.get(2).name = qsTr("Show after unread")
                }
            }

            ComboBoxList {
                id: handleReadSelection
                anchors.horizontalCenter: parent.horizontalCenter
                label: qsTr("Read articles")
                model: handleReadModel
                initialValue: handleRead
                onCurrentIndexChanged: handleRead = handleReadModel.get(handleReadSelection.currentIndex).value
            }

        }
    }
}
