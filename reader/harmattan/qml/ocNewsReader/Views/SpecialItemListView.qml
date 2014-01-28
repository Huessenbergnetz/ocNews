import QtQuick 1.1
import com.nokia.meego 1.0
//import com.nokia.extras 1.0
import QtMobility.feedback 1.1

import "../JS/globals.js" as GLOBALS

import "../Common"
import "../Delegates"
import "../Sheets"

Page {
    id: specialItemListView
    tools: specialItemListViewTools
    orientationLock: PageOrientation.LockPortrait

    property string id
    property string pageName
    property string feedType

    property int handleRead: dbus.getSetting("display/handleread", 0)
    property bool sortAsc: dbus.getSetting("display/sortasc", false) == "true"

    function openFile(file, properties) {
             var component = Qt.createComponent(file)
             if (component.status == Component.Ready)
                 pageStack.push(component, properties);
             else
                 console.log("Error loading component:", component.errorString());
         }

    Component.onCompleted: specialItemsModelSql.refresh(feedType, id, handleRead, sortAsc)
    Component.onDestruction: GLOBALS.previousContentY = 0

    Connections {
        target: folders
        onMarkedReadFolderSuccess: specialItemsModelSql.refresh(feedType, id, handleRead, sortAsc);
    }
    Connections {
        target: items
        onUpdatedItemsSuccess: { specialItemsModelSql.refresh(feedType, id, handleRead, sortAsc); itemsList.contentY = GLOBALS.previousContentY; }
        onRequestedItemsSuccess: { specialItemsModelSql.refresh(feedType, id, handleRead, sortAsc); itemsList.contentY = GLOBALS.previousContentY; }
        onStarredItemsSuccess: { specialItemsModelSql.refresh(feedType, id, handleRead, sortAsc); itemsList.contentY = GLOBALS.previousContentY; }
        onMarkedItemsSuccess: { specialItemsModelSql.refresh(feedType, id, handleRead, sortAsc); itemsList.contentY = GLOBALS.previousContentY; }
        onMarkedAllItemsReadSuccess: { specialItemsModelSql.refresh(feedType, id, handleRead, sortAsc); itemsList.contentY = GLOBALS.previousContentY; }
    }
    Connections {
        target: updater
        onUpdateStarted: if (GLOBALS.previousContentY === 0) GLOBALS.previousContentY = itemsList.contentY
        onUpdateFinished: { GLOBALS.previousContentY = itemsList.contentY; specialItemsModelSql.refresh(feedType, id, handleRead, sortAsc); itemsList.contentY = GLOBALS.previousContentY }
    }

    onHandleReadChanged: specialItemsModelSql.refresh(feedType, id, handleRead, sortAsc)
    onSortAscChanged: specialItemsModelSql.refresh(feedType, id, handleRead, sortAsc)

// ------------- Header Start ----------------

    Header {
        id: specialItemListViewHeader
        text: specialItemListView.pageName
    }

// ------------- Header End ----------------



// ------------- Start Search Field ------------

    Rectangle {
        id: searchFieldBox
        anchors { top: parent.top; topMargin: 71 }
        width: parent.width; height: 0; z: 1
        color: theme.inverted ? "black" : "#E0E1E2"
        clip: true

        Behavior on height { NumberAnimation { duration: 200 } }

        Rectangle {
            id: srect
            anchors { fill: searchFieldBox; leftMargin: 10; rightMargin: 10; top: searchFieldBox.top }
            color: "transparent"
            opacity: searchFieldBox.height === 0 ? 0 : 1

            Behavior on opacity { NumberAnimation { duration: 400 } }



            TextField {
                id: searchTextField
                width: parent.width
                placeholderText: qsTr("Search...")
                anchors { left: srect.left; top: srect.top; topMargin: 9 }
                platformStyle: TextFieldStyle { paddingRight: clearButton.width }
                inputMethodHints: Qt.ImhNoPredictiveText

                onTextChanged: { specialItemsModelSql.refresh(feedType, id, handleRead, sortAsc, text); searchFieldTimer.restart(); }

                Image {
                    id: clearButton
                    anchors { right: parent.right; verticalCenter: parent.verticalCenter }
                    source: searchTextField.text === "" ? "image://theme/icon-m-common-search" : "image://theme/icon-m-input-clear"
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            searchTextField.text = ""
                            specialItemsModelSql.refresh(feedType, id, handleRead, sortAsc)
                        }
                    }
                }
            }
        }

        Timer {
            id: searchFieldTimer; running: false; interval: 7000; repeat: false
            onTriggered: { if (searchTextField.text === "") searchFieldBox.height = 0 }
        }
    }

// ------------- End Search Field ------------


    ListView {
        id: itemsList
        anchors { fill: parent; topMargin: searchFieldBox.height + 71; leftMargin: 0; rightMargin: 16 }
        model: specialItemsModelSql
        delegate: SpecialItemListDelegate {
                 onClicked: { GLOBALS.previousContentY = itemsList.contentY; openFile("SingleItemView.qml", {itemId: itemId}) }
                 onPressAndHold: {
                     contextMenuEffect.play()
                     itemContextMenu.itId = itemId
                     itemContextMenu.itemTitle = title
                     itemContextMenu.itemUrl = url
                     itemContextMenu.feedName = feedName
                     itemContextMenu.feedId = feedId
                     itemContextMenu.itemGuidHash = guidHash
                     itemContextMenu.starred = starred
                     itemContextMenu.unread = unread
                     itemContextMenu.open()
                 }
        }

        onContentYChanged: {
            if (contentY < -100) {
                searchFieldBox.height = 71;
                searchFieldTimer.running = true;
            }
        }
    }

    ScrollDecorator {
        flickableItem: itemsList
        anchors { right: parent.right; rightMargin: -itemsList.anchors.rightMargin }
    }

    ThemeEffect { id: contextMenuEffect; effect: "LongPress" }


// ------------- ToolBar Start -------------
    ToolBarLayout {
        id: specialItemListViewTools
        visible: true
        ToolIcon {
            platformIconId: "toolbar-back"
            anchors.left: (parent === undefined) ? undefined : parent.left
            onClicked: { pageStack.pop(); }
        }
//        ToolIcon {
//            id: updaterIcon
//            platformIconId: "toolbar-refresh"
//            state: feedType != "starred" ? "NORMAL" : "RUNNING"
//            states: [
//                State {
//                    name: "NORMAL"
//                    PropertyChanges { target: updaterIcon; enabled: true; visible: true; }
//                },
//                State {
//                    name: "RUNNING"
//                    PropertyChanges { target: updaterIcon; enabled: false; visible: false; }
//                }
//            ]
//            onClicked: {
//                GLOBALS.previousContentY = itemsList.contentY;
//                specialItemListViewHeader.indicatorState = "RUNNING"
//                updaterIcon.state = "RUNNING"
//                feedType === "folder" ? items.updateItems("0", "1", specialItemListView.id) : dbus.initConnection()
//            }
//            Connections {
//                target: items
//                onUpdatedItemsSuccess: updaterIcon.state = "NORMAL"
//                onUpdatedItemsError: updaterIcon.state = "NORMAL"
//            }
//            Connections {
//                target: dbus
//                onInitSuccess: updater.startUpdate();
//            }
//            Connections {
//                target: updater
//                onUpdateFinished: updaterIcon.state = "NORMAL"
//                onUpdateError: updaterIcon.state = "NORMAL"
//                onUpdateStarted: updaterIcon.state = "RUNNING";
//            }
//        }
        ToolIcon {
            platformIconId: "toolbar-view-menu"
            anchors.right: (parent === undefined) ? undefined : parent.right
            onClicked: { (folderMenu.status === DialogStatus.Closed) ? folderMenu.open() : folderMenu.close(); }
        }
    }

    Menu {
        id: folderMenu
        visualParent: pageStack
        MenuLayout {
            MenuItem {
                text: feedType === "folder" ? qsTr("Mark folder as read") : qsTr("Mark all as read")
                visible: feedType != "starred"
                enabled: !operationRunning
                onClicked: {
                    operationRunning = true
                    GLOBALS.previousContentY = itemsList.contentY;
                    feedType === "folder" ? folders.markFolderRead(id) : items.markAllItemsRead()
                }
            }
            MenuItem {
                text: sortAsc ? qsTr("Show newest on top") : qsTr("Show oldest on top")
                onClicked: sortAsc = !sortAsc
            }
            MenuItem {
                SelectionItem {
                    id: handleReadSelect
                    anchors.fill: parent
                    anchors.leftMargin: 25
                    anchors.rightMargin: 25
                    title: qsTr("Read articles")
                    model: handleReadModel
                    initialValue: handleRead
                    onCurrentValueChanged: handleRead = currentValue
                }
            }
        }
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


// ----------------- ToolBar End -------------



// ----------------- Context Menu Start --------------

    ContextMenu {
        id: itemContextMenu
        property string itId
        property string itemTitle
        property string itemUrl
        property string feedName
        property bool starred
        property string itemGuidHash
        property string feedId
        property bool unread
        function starParams() {
            var feedId=itemContextMenu.feedId;
            var params={};
            params[feedId]=itemContextMenu.itemGuidHash;
            return params;
        }
        function markParams() {
            var params = new Array();
            params[0]=itemContextMenu.itId;
            return params;
        }
        MenuLayout {
            MenuItem {
                text: itemContextMenu.starred === false ? qsTr("Add to favourites") : qsTr("Remove from favourites")
                enabled: !operationRunning
                onClicked: {
                    operationRunning = true
                    GLOBALS.previousContentY = itemsList.contentY;
                    itemContextMenu.starred === false ?
                                    items.starItems("star", itemContextMenu.starParams() ) :
                                    items.starItems("unstar", itemContextMenu.starParams() )
                }
            }
            MenuItem {
                text: itemContextMenu.unread ? qsTr("Mark as read") : qsTr("Mark as unread")
                enabled: !operationRunning
                onClicked: {
                    operationRunning = true
                    GLOBALS.previousContentY = itemsList.contentY;
                    itemContextMenu.unread ?
                                items.markItems("read", itemContextMenu.markParams()) :
                                items.markItems("unread", itemContextMenu.markParams());
                }
            }
            MenuItem {
                text: qsTr("Share")
                onClicked: shareUi.share(itemContextMenu.itemUrl, itemContextMenu.itemTitle, itemContextMenu.feedName)
            }
        }
    }

// ----------------- Context Menu End --------------



}
