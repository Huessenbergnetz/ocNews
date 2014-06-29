import QtQuick 2.0
import Sailfish.Silica 1.0

import "../Delegates"
import "../Common"
import "../BTComponents"
import "../JS/globals.js" as GLOBALS

Page {
    id: specialItemListView

    property int feedId
    property string pageName
    property int feedType

    property int handleRead: config.handleRead
    property bool sortAsc: config.sortAsc
    property string searchString: ""

    onSearchStringChanged: specialItemList.model.search = searchString
    onSortAscChanged: specialItemList.model.sortAsc = sortAsc
    onHandleReadChanged: specialItemList.model.handleRead = handleRead

    Component.onCompleted: {
        specialItemList.contentY = -headerContainer.height
        specialItemList.model.search = searchString
        specialItemList.model.sortAsc = sortAsc
        specialItemList.model.handleRead = handleRead
    }
    Component.onDestruction: specialItemsModelSql.clear()

    onStatusChanged: {
        if (status === PageStatus.Active) {
            specialItemsModelSql.type = feedType
            specialItemsModelSql.id = feedId
        }
    }

    BusyIndicator {
        size: BusyIndicatorSize.Large
        visible: true
        opacity: specialItemsModelSql.populating ? 1 : 0
        running: specialItemsModelSql.populating
        anchors.centerIn: parent
        Behavior on opacity { NumberAnimation { duration: 250; easing.type: Easing.InOutQuad } }
        onRunningChanged: console.log("RUNNING: " + running)
    }



    Column {
        id: headerContainer

        move: Transition { NumberAnimation { properties: "y"; easing.type: Easing.InOutQuad } }
        add: Transition { FadeAnimation {} }


        width: specialItemListView.width

        PageHeader { title: operationRunning ? qsTr("Update running...") : specialItemListView.pageName }

        SearchField {
            id: searchField
            width: parent.width
            visible: false
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


    SilicaListView {
        id: specialItemList
        anchors { top: parent.top; right: parent.right; left: parent.left; bottom: specialItemListFetchIndicator.visible ? specialItemListFetchIndicator.top : sortingPanel.open ? sortingPanel.top : parent.bottom }
        currentIndex: -1
        clip: true

        header: Item {
            id: header
            width: headerContainer.width
            height: headerContainer.height
            Component.onCompleted: headerContainer.parent = header
        }


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
                    searchField.focus = false;
                    sortingPanel.open = !sortingPanel.open
                }
            }
            MenuItem {
                id: markAsRead
                enabled: !operationRunning
                visible: specialItemsModelSql.type !== 2
                text: specialItemsModelSql.type === 1 ? qsTr("Mark folder as read") : qsTr("Mark all as read")
                onClicked: { operationRunning = true; specialItemsModelSql.type === 1 ?  markFolderRead(specialItemsModelSql.id, specialItemListView.pageName) : markAllAsRead() }
            }
            MenuItem {
                id: update
                enabled: !operationRunning
                visible: specialItemsModelSql.type !== 2
                text: specialItemsModelSql.type === 1 ? qsTr("Update folder") : qsTr("Update all")
                onClicked: { operationRunning = true; specialItemsModelSql.type === 1 ? items.updateItems("0", "1", specialItemsModelSql.id) : updater.startUpdate() }
            }
            MenuItem {
                id: showSearch
                text: searchField.visible ? qsTr("Hide search") : qsTr("Show search")
                onClicked: {
                    searchField.visible = !searchField.visible
                    if (!searchField.visible) {specialItemListView.searchString = ""; searchField.focus = false } else {searchField.focus = true }
                }
            }
        }

        model: specialItemsModelFilter

        delegate: SpecialItemListDelegate { feedId: specialItemListView.feedId; searchString: searchField.text; handleRead: specialItemListView.handleRead; sortAsc: specialItemListView.sortAsc; feedType: specialItemListView.feedType }

        VerticalScrollDecorator {}

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
        id: sortingPanel
        width: parent.width
        height: actCol.height
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
