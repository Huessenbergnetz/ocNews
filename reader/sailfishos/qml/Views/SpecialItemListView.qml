import QtQuick 2.0
import Sailfish.Silica 1.0

import "../Delegates"
import "../Common"
import "../JS/globals.js" as GLOBALS

Page {
    id: specialItemListView

    property string id
    property string pageName
    property string feedType

    property int handleRead: dbus.getSetting("display/handleread", 0)
    property bool sortAsc: dbus.getSetting("display/sortasc", false) == "true"

    Component.onCompleted: specialItemsModelSql.refresh(feedType, id, handleRead, sortAsc)
    Component.onDestruction: GLOBALS.previousContentY = 0;

    Connections {
        target: folders
        onMarkedReadFolderSuccess: { specialItemListViewPully.busy = false; specialItemsModelSql.refresh(feedType, id, handleRead, sortAsc) }
    }
    Connections {
        target: items
        onUpdatedItemsSuccess: { specialItemListViewPully.busy = false; specialItemsModelSql.refresh(feedType, id, handleRead, sortAsc); specialItemList.contentY = GLOBALS.previousContentY; update.enabled = true }
        onUpdatedItemsError: { specialItemListViewPully.busy = false; update.enabled = true }
        onRequestedItemsSuccess: { specialItemsModelSql.refresh(feedType, id, handleRead, sortAsc); specialItemList.contentY = GLOBALS.previousContentY }
        onStarredItemsSuccess: { specialItemsModelSql.refresh(feedType, id, handleRead, sortAsc); specialItemList.contentY = GLOBALS.previousContentY }
        onMarkedItemsSuccess: { specialItemsModelSql.refresh(feedType, id, handleRead, sortAsc); specialItemList.contentY = GLOBALS.previousContentY }
        onMarkedAllItemsReadSuccess: { specialItemListViewPully.busy = false; specialItemsModelSql.refresh(feedType, id, handleRead, sortAsc); specialItemList.contentY = GLOBALS.previousContentY }
        onMarkedAllItemsReadError: { pecialItemListViewPully.busy = false; }
    }
    Connections {
        target: updater
        onUpdateFinished: { specialItemsModelSql.refresh(feedType, id, handleRead, sortAsc); specialItemListViewPully.busy = false; update.enabled = true }
        onUpdateError: { specialItemListViewPully.busy = false; update.enabled = true }
        onUpdateStarted: { specialItemListViewPully.busy = true; update.enabled = false }
    }

    onHandleReadChanged: specialItemsModelSql.refresh(feedType, id, handleRead, sortAsc)
    onSortAscChanged: specialItemsModelSql.refresh(feedType, id, handleRead, sortAsc)

    Column {
        id: headerContainer

        width: specialItemListView.width

        PageHeader {
            id: pHeader
            title: updater.isUpdateRunning() ? "Update running..." : specialItemListView.pageName
        }

        SearchField {
            id: searchField
            width: parent.width
            visible: false
            onTextChanged: { specialItemsModelSql.refresh(feedType, id, handleRead, sortAsc, text); searchFieldTimer.restart(); searchField.forceActiveFocus(); searchField.focus = true }
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
        id: specialItemList
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
            id: specialItemListViewPully
            busy: updater.isUpdateRunning() ? true : false
            MenuItem {
                id: menuSort
                text: sortingPanel.open ? qsTr("Hide sorting options") : qsTr("Show sorting options")
                onClicked: sortingPanel.open = !sortingPanel.open
            }
            MenuItem {
                id: markAsRead
                visible: feedType !== "starred"
                text: feedType === "folder" ? qsTr("Mark folder as read") : qsTr("Mark all as read")
                onClicked: feedType === "folder" ?  markFolderRead(specialItemListView.id, specialItemListView.pageName) : markAllAsRead()
            }
            MenuItem {
                id: update
                visible: feedType !== "starred"
                text: feedType === "folder" ? qsTr("Update folder") : qsTr("Update all")
                onClicked: { update.enabled = false; specialItemListViewPully.busy = true; feedType === "folder" ? items.updateItems("0", "1", specialItemListView.id) : updater.startUpdate() }
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

        model: specialItemsModelSql

        delegate: SpecialItemListDelegate { searchString: searchField.text }

        VerticalScrollDecorator {}

        FancyScroller {}

        PushUpMenu {
            id: specialItemListViewPushy
            enabled: specialItemList.contentHeight >= specialItemList.height
            visible: specialItemList.contentHeight >= specialItemList.height
            MenuItem {
                id: goToTop
                text: qsTr("Scroll to top")
                onClicked: specialItemList.scrollToTop()
            }
        }

    }

    function markFolderRead(folderId, folderName)
    {
        remorsePop.execute(qsTr("Marking %1 as read").arg(folderName), function() { specialItemListViewPully.busy = true; folders.markFolderRead(folderId); } );
    }

    function markAllAsRead()
    {
        remorsePop.execute(qsTr("Marking all entries as read"), function() { specialItemListViewPully.busy = true; items.markAllItemsRead() } );
    }

    RemorsePopup {
        id: remorsePop
    }

    DockedPanel {
        id: sortingPanel
        width: parent.width
        height: actCol.height + Theme.paddingLarge
        z: 10

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