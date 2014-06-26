import QtQuick 2.0
import Sailfish.Silica 1.0

import "../Delegates"
import "../Common"
import "../BTComponents"

Page {
    id: itemListView

    property string feedName

    property int handleRead: config.handleRead
    property bool sortAsc: config.sortAsc
    property string searchString: ""

    onSearchStringChanged: itemList.model.search = searchString
    onSortAscChanged: itemList.model.sortAsc = sortAsc
    onHandleReadChanged: itemList.model.handleRead = handleRead

    Component.onCompleted: {
        itemList.contentY = -headerContainer.height
        itemList.model.search = searchString
        itemList.model.sortAsc = sortAsc
        itemList.model.handleRead = handleRead
    }
    Component.onDestruction: itemsModelSql.startCleanUpTimer()

    Connections {
        target: feeds
        onDeletedFeedSuccess: pageStack.pop()
        onRenamedFeedSuccess: itemListView.feedName = newName
    }

    Column {
        id: headerContainer

        move: Transition { NumberAnimation { properties: "y"; easing.type: Easing.InOutQuad } }
        add: Transition { FadeAnimation {} }


        width: itemListView.width

        PageHeader { title: operationRunning ? qsTr("Update running...") : itemListView.feedName }

        SearchField {
            id: searchField
            width: parent.width
            visible: false
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


    SilicaListView {
        id: itemList
        anchors { top: parent.top; right: parent.right; left: parent.left; bottom: itemListFetchIndicator.visible ? itemListFetchIndicator.top : sortingPanel.open ? sortingPanel.top : parent.bottom }
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
            id: itemListViewPully
            busy: operationRunning
            MenuItem {
                id: deleteFeed
                enabled: !operationRunning
                text: qsTr("Delete feed")
                onClicked: removeFeed(itemsModelSql.feedId, itemListView.feedName)
            }
            MenuItem {
                id: renameFeed
                enabled: !operationRunning
                text: qsTr("Rename feed")
                onClicked: {
                    var dialog = pageStack.push(Qt.resolvedUrl("../Dialogs/RenameFeed.qml"), {feedId: itemsModelSql.feedId, feedName: feedName})
                    dialog.accepted.connect(function() { operationRunning = true })
                }
            }
            MenuItem {
                id: menuSort
                text: sortingPanel.open ? qsTr("Hide sorting options") : qsTr("Show sorting options")
                onClicked: {
                    searchField.focus = false
                    sortingPanel.open = !sortingPanel.open
                }
            }
            MenuItem {
                id: markFeedAsRead
                enabled: !operationRunning
                text: qsTr("Mark feed as read")
                onClicked: { operationRunning = true; feeds.markFeedRead(itemsModelSql.feedId) }
            }
            MenuItem {
                id: updateFeed
                enabled: !operationRunning
                text: qsTr("Update feed")
                onClicked: { operationRunning = true; items.updateItems("0", "0", itemsModelSql.feedId) }
            }
            MenuItem {
                id: showSearch
                text: searchField.visible ? qsTr("Hide search") : qsTr("Show search")
                onClicked: {
                    searchField.visible = !searchField.visible
                    if (!searchField.visible) {searchField.text = ""; searchField.focus = false } else {searchField.focus = true }
                }
            }
        }

        model: itemsModelFilter

        delegate: ItemListDelegate { feedId: itemsModelSql.feedId; searchString: searchField.text; handleRead: itemListView.handleRead; sortAsc: itemListView.sortAsc; feedType: "0" }

        VerticalScrollDecorator {}

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
        id: sortingPanel
        width: parent.width
        height: actCol.height
        visible: open

        dock: Dock.Bottom

        Column {
            id: actCol
            width: parent.width

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

            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                text: sortAsc ? qsTr("Show newest on top") : qsTr("Show oldest on top")
                onClicked: sortAsc = !sortAsc
            }
        }
    }
}
