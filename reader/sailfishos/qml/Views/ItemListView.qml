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
    property string searchString

    onSearchStringChanged: { itemsModelSql.refresh(feedId, handleRead, sortAsc, searchString) }

    Component.onCompleted: { console.log(handleRead); itemsModelSql.refresh(feedId, handleRead, sortAsc, searchString) }
    Component.onDestruction: GLOBALS.previousContentY = 0

    Connections {
        target: feeds
        onMarkedReadFeedSuccess: { itemsModelSql.refresh(feedId, handleRead, sortAsc, searchString) }
        onDeletedFeedSuccess: pageStack.pop()
    }
    Connections {
        target: items
        onUpdatedItemsSuccess: { GLOBALS.previousContentY = itemList.contentY; itemsModelSql.refresh(feedId, handleRead, sortAsc, searchString); itemList.contentY = GLOBALS.previousContentY }
        onRequestedItemsSuccess: { GLOBALS.previousContentY = itemList.contentY; itemsModelSql.refresh(feedId, handleRead, sortAsc, searchString); itemList.contentY = GLOBALS.previousContentY; }
        onStarredItemsSuccess: { GLOBALS.previousContentY = itemList.contentY; itemsModelSql.refresh(feedId, handleRead, sortAsc, searchString); itemList.contentY = GLOBALS.previousContentY; }
        onMarkedItemsSuccess: { GLOBALS.previousContentY = itemList.contentY; itemsModelSql.refresh(feedId, handleRead, sortAsc, searchString); itemList.contentY = GLOBALS.previousContentY; }
    }
    Connections {
        target: updater
        onUpdateFinished: { GLOBALS.previousContentY = itemList.contentY; itemsModelSql.refresh(feedId, handleRead, sortAsc, searchString); itemList.contentY = GLOBALS.previousContentY }
    }

    onSortAscChanged: itemsModelSql.refresh(feedId, handleRead, sortAsc, searchString)
    onHandleReadChanged: itemsModelSql.refresh(feedId, handleRead, sortAsc, searchString)


    SilicaListView {
        id: itemList
        anchors { top: parent.top; right: parent.right; left: parent.left; bottom: itemListFetchIndicator.visible ? itemListFetchIndicator.top : parent.bottom; bottomMargin: sortingPanel.open || searchPanel.open ? sortingPanel.open ? sortingPanel.height * 1.5 : searchPanel.height * 1.5 : 0 }
        currentIndex: -1

        header: PageHeader { title: operationRunning ? qsTr("Update running...") : itemListView.feedName }

        Behavior on anchors.bottomMargin {
            NumberAnimation { duration: 300; easing.type: Easing.InOutQuad }
        }

        Behavior on height {
            NumberAnimation { duration: 300; easing.type: Easing.InOutQuad }
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
                onClicked: {
                    searchPanel.open = false;
                    searchField.focus = false;
                    sortingPanel.open = !sortingPanel.open
                }
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
                text: searchPanel.open ? qsTr("Hide search") : qsTr("Show search")
                onClicked: {
                    sortingPanel.open = false
                    searchPanel.open = !searchPanel.open
//                    searchField.forceActiveFocus();
                    if (!searchPanel.open) {specialItemListView.searchString = ""; searchField.focus = false } else {searchField.focus = true }
                }
            }
        }

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

    FetchImagesIndicator {
        id: itemListFetchIndicator
    }

    function removeFeed(feedId, feedName)
    {
        remorsePop.execute(qsTr("Deleting feed %1").arg(feedName), function() { operationRunning = true; feeds.deleteFeed(feedId) } );
    }

    RemorsePopup {
        id: remorsePop
    }

    DockedPanel {
        id: searchPanel
        width: parent.width
        height: searchField.height
        visible: open
        dock: Dock.Bottom

        SearchField {
            id: searchField
            width: parent.width
            placeholderText: qsTr("Search")
            EnterKey.onClicked: searchField.focus = false
            EnterKey.iconSource: "image://theme/icon-m-enter-close"

            Binding {
                target: itemListView
                property: "searchString"
                value: searchField.text
            }
        }
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
