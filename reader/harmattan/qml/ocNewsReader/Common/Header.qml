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
        visible: operationRunning
        running: operationRunning
    }
}
