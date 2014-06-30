import QtQuick 1.1
import com.nokia.meego 1.0
//import com.nokia.extras 1.0
import QtMobility.feedback 1.1

import "../Common"
import "../Delegates"
import "../Sheets"

Page {
    id: specialItemListView
    tools: specialItemListViewTools
    orientationLock: PageOrientation.LockPortrait

    property int feedId
    property string pageName
    property int feedType

    property int handleRead: config.handleRead
    property bool sortAsc: config.sortAsc
    property string searchString: ""

    onSearchStringChanged: itemsList.model.search = searchString
    onHandleReadChanged: itemsList.model.handleRead = handleRead
    onSortAscChanged: itemsList.model.sortAsc = sortAsc

    Component.onCompleted: {
        itemsList.model.search = searchString
        itemsList.model.handleRead = handleRead
        itemsList.model.sortAsc = sortAsc
    }

    Component.onDestruction: specialItemsModelSql.clear()

//    onStatusChanged: {
//        if (status === PageStatus.Active) {
//            specialItemsModelSql.type = feedType
//            specialItemsModelSql.id = feedId
//        }
//    }

    function openFile(file, properties) {
             var component = Qt.createComponent(file)
             if (component.status === Component.Ready)
                 pageStack.push(component, properties);
             else
                 console.log("Error loading component:", component.errorString());
         }

// ------------- Header Start ----------------

    Header {
        id: specialItemListViewHeader
        text: specialItemListView.pageName
    }

// ------------- Header End ----------------



    BusyIndicator {
        platformStyle: BusyIndicatorStyle { id: headerBusyIndicatorStyle; size: "large"; inverted: theme.inverted }
        anchors.centerIn: parent
        visible: specialItemsModelSql.populating
        running: visible
    }

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

                Binding {
                    target: specialItemListView
                    property: "searchString"
                    value: searchTextField.text
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
        anchors { top: parent.top; topMargin: searchFieldBox.height + 71; left: parent.left; leftMargin: 0; right: parent.right; rightMargin: 16; bottom: sivFetchImagesIndicator.visible ? sivFetchImagesIndicator.top : parent.bottom }
        model: specialItemsModelFilter
        delegate: SpecialItemListDelegate {
                 onClicked: {
                     item.searchString = specialItemListView.searchString
                     item.handleRead = specialItemListView.handleRead
                     item.sortAsc = specialItemListView.sortAsc
                     item.feedType = specialItemListView.feedType
                     item.parentFeedId = specialItemListView.feedId
                     item.showImg = config.handleImgs > 0
                     item.itemId = itemId
                     openFile("SingleItemView.qml") }
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

        Behavior on height {
            NumberAnimation { duration: 300; easing.type: Easing.InOutQuad }
        }
    }

    FetchImagesIndicator {
        id: sivFetchImagesIndicator
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
                    tsBasename: "SpecialItemListView"
                }
            }
        }
    }

    ListModel {
        id: handleReadModel
        ListElement { name: QT_TR_NOOP("Show"); value: 0 }
        ListElement { name: QT_TR_NOOP("Hide"); value: 1 }
        ListElement { name: QT_TR_NOOP("Show after unread"); value: 2 }
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
