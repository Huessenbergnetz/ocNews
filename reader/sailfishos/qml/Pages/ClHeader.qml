import QtQuick 2.0
import Sailfish.Silica 1.0

BackgroundItem {
    id: root

    signal expand()

    property bool expanded: false
    property string version
    property real time


    width: parent.width
    height: Theme.itemSizeSmall
    Row {
        anchors { left: parent.left; right: parent.right; leftMargin: Theme.paddingLarge; rightMargin: Theme.paddingLarge; verticalCenter: parent.verticalCenter }
        Label {
            anchors { verticalCenter: parent.verticalCenter }
            width: parent.width - icon.width
            text: qsTr("Version") + " " + version + " - " + Qt.formatDateTime(new Date(time), Qt.DefaultLocaleShortDate)
            color: parent.pressed ? Theme.highlightColor : Theme.primaryColor
            truncationMode: TruncationMode.Fade
            maximumLineCount: 1
        }
        Image {
            id: icon
            anchors { verticalCenter: parent.verticalCenter }
            width: 32; height: 32
            source: "image://theme/icon-m-left"
            state: expanded ? "expanded" : "unexpanded"
            states: [
                State {
                    name: "expanded"
                    PropertyChanges { target: icon; rotation: 270 }
                },
                State {
                    name: "unexpanded"
                    PropertyChanges { target: icon; rotation: 0 }
                }
            ]
            transitions: Transition { RotationAnimation {duration: 250; direction: icon.state === "expanded" ? RotationAnimation.Counterclockwise : RotationAnimation.Clockwise} }
        }
    }
    onClicked: { expanded = !expanded; root.expand() }
}
