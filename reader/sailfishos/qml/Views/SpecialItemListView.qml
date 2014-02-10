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
    property string searchString

    onSearchStringChanged: { specialItemsModelSql.refresh(feedType, id, handleRead, sortAsc, searchString) }

    Component.onCompleted: specialItemsModelSql.refresh(feedType, id, handleRead, sortAsc, searchString)
    Component.onDestruction: GLOBALS.previousContentY = 0;

    Connections {
        target: folders
        onMarkedReadFolderSuccess: specialItemsModelSql.refresh(feedType, id, handleRead, sortAsc, searchString)
    }
    Connections {
        target: items
        onUpdatedItemsSuccess: { specialItemsModelSql.refresh(feedType, id, handleRead, sortAsc, searchString); specialItemList.contentY = GLOBALS.previousContentY }
        onRequestedItemsSuccess: { specialItemsModelSql.refresh(feedType, id, handleRead, sortAsc, searchString); specialItemList.contentY = GLOBALS.previousContentY }
        onStarredItemsSuccess: { specialItemsModelSql.refresh(feedType, id, handleRead, sortAsc, searchString); specialItemList.contentY = GLOBALS.previousContentY }
        onMarkedItemsSuccess: { specialItemsModelSql.refresh(feedType, id, handleRead, sortAsc, searchString); specialItemList.contentY = GLOBALS.previousContentY }
        onMarkedAllItemsReadSuccess: { specialItemsModelSql.refresh(feedType, id, handleRead, sortAsc, searchString); specialItemList.contentY = GLOBALS.previousContentY }
    }
    Connections {
        target: updater
        onUpdateFinished: { GLOBALS.previousContentY = specialItemList.contentY; specialItemsModelSql.refresh(feedType, id, handleRead, sortAsc, searchString); specialItemList.contentY = GLOBALS.previousContentY }
    }

    onHandleReadChanged: specialItemsModelSql.refresh(feedType, id, handleRead, sortAsc, searchString)
    onSortAscChanged: specialItemsModelSql.refresh(feedType, id, handleRead, sortAsc, searchString)



    SilicaListView {
        id: specialItemList
        anchors { top: parent.top; right: parent.right; left: parent.left; bottom: specialItemListFetchIndicator.visible ? specialItemListFetchIndicator.top : parent.bottom; bottomMargin: sortingPanel.open || searchPanel.open ? sortingPanel.open ? sortingPanel.height * 1.5 : searchPanel.height * 1.5 : 0 }
        currentIndex: -1

        header: PageHeader { title: operationRunning ? qsTr("Update running...") : specialItemListView.pageName }


        Behavior on anchors.bottomMargin {
            NumberAnimation { duration: 300; easing.type: Easing.InOutQuad }
        }

        Behavior on height {
            NumberAnimation { duration: 300; easing.type: Easing.InOutQuad }
        }

        PullDownMenu {
            id: specialItemListViewPully
            busy: operationRunning
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
                id: markAsRead
                enabled: !operationRunning
                visible: feedType !== "starred"
                text: feedType === "folder" ? qsTr("Mark folder as read") : qsTr("Mark all as read")
                onClicked: feedType === "folder" ?  markFolderRead(specialItemListView.id, specialItemListView.pageName) : markAllAsRead()
            }
            MenuItem {
                id: update
                enabled: !operationRunning
                visible: feedType !== "starred"
                text: feedType === "folder" ? qsTr("Update folder") : qsTr("Update all")
                onClicked: { operationRunning = true; feedType === "folder" ? items.updateItems("0", "1", specialItemListView.id) : updater.startUpdate() }
            }
            MenuItem {
                id: showSearch
                text: searchPanel.open ? qsTr("Hide search") : qsTr("Show search")
                onClicked: {
                    sortingPanel.open = false
                    searchPanel.open = !searchPanel.open
                    searchField.forceActiveFocus();
                    if (!searchPanel.open) {specialItemListView.searchString = ""; searchField.focus = false } else {searchField.focus = true }
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

    FetchImagesIndicator {
        id: specialItemListFetchIndicator
    }

    function markFolderRead(folderId, folderName)
    {
        remorsePop.execute(qsTr("Marking %1 as read").arg(folderName), function() { operationRunning = true; folders.markFolderRead(folderId); } );
    }

    function markAllAsRead()
    {
        remorsePop.execute(qsTr("Marking all entries as read"), function() { operationRunning = true; items.markAllItemsRead() } );
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
                target: specialItemListView
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
