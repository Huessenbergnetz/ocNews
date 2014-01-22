import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.0

Rectangle {
    id: header
    property alias text: text.text
    property alias color: header.color
    property alias indicatorState: headerBusyIndicator.state
    property alias indicatorInverted: headerBusyIndicatorStyle.inverted

    height: 70
    width: parent.width
//    color: "#1d2d44"
    color: theme.inverted ? "black" : "#f78500"
    anchors.top: parent.top
    z: 1

    Label {
        id: text
        text: qsTr("ocNews")
        width: parent.width - 15
        maximumLineCount: 1
        elide: Text.ElideRight
        anchors { verticalCenter: parent.verticalCenter; left: parent.left; leftMargin: 15 }
        platformStyle: LabelStyle { textColor: theme.inverted ? theme.selectionColor : "white"; fontPixelSize: 30 }
        textFormat: Text.PlainText
    }

    Image {
       height: 2
       width: parent.width
       anchors { left: parent.left; right: parent.right; top: parent.bottom; }
       source: "image://theme/meegotouch-separator" + (theme.inverted? "-inverted" : "") + "-background-horizontal"
       fillMode: Image.TileHorizontally
    }

    BusyIndicator {
        id: headerBusyIndicator
        platformStyle: BusyIndicatorStyle { id: headerBusyIndicatorStyle; size: "small"; inverted: true }
        anchors { verticalCenter: parent.verticalCenter; right: parent.right; rightMargin: 20 }
        state: updater.isUpdateRunning() ? "RUNNING" : "NORMAL"
        states: [
            State {
                name: "NORMAL"
                PropertyChanges { target: headerBusyIndicator; running: false; visible: false }
            },
            State {
                name: "RUNNING"
                PropertyChanges { target: headerBusyIndicator; running: true; visible: true }
            }
        ]
        Connections {
            target: updater
            onUpdateStarted: headerBusyIndicator.state = "RUNNING"
            onUpdateFinished: headerBusyIndicator.state = "NORMAL"
            onUpdateError: headerBusyIndicator.state = "NORMAL"
        }
        Connections {
            target: items
            onUpdatedItemsSuccess: headerBusyIndicator.state = "NORMAL"
            onUpdatedItemsError: headerBusyIndicator.state = "NORMAL"
            onStarredItemsError: headerBusyIndicator.state = "NORMAL"
            onStarredItemsSuccess: headerBusyIndicator.state = "NORMAL"
            onMarkedItemsSuccess: headerBusyIndicator.state = "NORMAL"
            onMarkedItemsError: headerBusyIndicator.state = "NORMAL"
            onMarkedAllItemsReadSuccess: headerBusyIndicator.state = "NORMAL"
            onMarkedAllItemsReadError: headerBusyIndicator.state = "NORMAL"
        }
        Connections {
            target: folders
            onMarkedReadFolderSuccess: headerBusyIndicator.state = "NORMAL"
            onMarkedReadFolderError: headerBusyIndicator.state = "NORMAL"
            onDeletedFolderSuccess: headerBusyIndicator.state = "NORMAL"
            onDeletedFolderError: headerBusyIndicator.state = "NORMAL"
            onRenamedFolderSuccess: headerBusyIndicator.state = "NORMAL"
            onRenamedFolderError: headerBusyIndicator.state = "NORMAL"
            onCreatedFolderSuccess: headerBusyIndicator.state = "NORMAL"
            onCreatedFolderError: headerBusyIndicator.state = "NORMAL"
        }
        Connections {
            target: feeds
            onDeletedFeedSuccess: headerBusyIndicator.state = "NORMAL"
            onDeletedFeedError: headerBusyIndicator.state = "NORMAL"
            onCreatedFeedSuccess: headerBusyIndicator.state = "NORMAL"
            onCreatedFeedError: headerBusyIndicator.state = "NORMAL"
            onMovedFeedSuccess: headerBusyIndicator.state = "NORMAL"
            onMovedFeedError: headerBusyIndicator.state = "NORMAL"
            onMarkedReadFeedSuccess: headerBusyIndicator.state = "NORMAL"
            onMarkedReadFeedError: headerBusyIndicator.state = "NORMAL"
        }
        Connections {
            target: dbus
            onInitError: headerBusyIndicator.state = "NORMAL";
        }
    }
}
