import QtQuick 1.1
import com.nokia.meego 1.0
import "../uiconstants.js" as UI

Item {
    id: root
    width: parent.width
    height: expanded ? header.height + entryText.height : header.height

    property bool expanded: false
    property string version
    property real time
    property string text

    Item {
        id: header

        width: parent.width
        height: 70

        BorderImage {
                id: background
                anchors.fill: parent
                // Fill page porders
                anchors { leftMargin: -15; rightMargin: -15 }
                visible: mouseArea.pressed
                source: theme.inverted ? "image://theme/meegotouch-panel-inverted-background-pressed" : "image://theme/meegotouch-panel-background-pressed"
        }

        Row {
            anchors { left: parent.left; right: parent.right; verticalCenter: parent.verticalCenter }

            Label {
                anchors { verticalCenter: parent.verticalCenter }
                width: parent.width - icon.width
                text: qsTr("Version") + " " + root.version + " - " + Qt.formatDateTime(new Date(root.time), Qt.DefaultLocaleShortDate)
                color: theme.inverted ? "white" : "black"
                elide: Text.ElideRight
                maximumLineCount: 1
                font.pointSize: 20
            }

            Image {
                id: icon
                anchors { verticalCenter: parent.verticalCenter }
                width: 32; height: 32
                source: "image://theme/icon-m-common-drilldown-arrow" + (theme.inverted? "-inverse" : "")
                state: root.expanded ? "expanded" : "unexpanded"
                states: [
                    State {
                        name: "expanded"
                        PropertyChanges { target: icon; rotation: 90 }
                    },
                    State {
                        name: "unexpanded"
                        PropertyChanges { target: icon; rotation: 0 }
                    }
                ]
                transitions: Transition { RotationAnimation {duration: 200; direction: icon.state !== "expanded" ? RotationAnimation.Counterclockwise : RotationAnimation.Clockwise} }
            }
        }

        MouseArea {
            id: mouseArea
            anchors.fill: parent
            onClicked: root.expanded = !root.expanded
        }
    }

    Text {
        id: entryText
        visible: root.expanded
        opacity: root.expanded ? 1 : 0
        clip: true
        text: root.text
        anchors { left: parent.left; right: parent.right; top: header.bottom }
        textFormat: Text.RichText
        color: theme.inverted ? "white" : "black"
        wrapMode: Text.WordWrap
        font.pointSize: 17
        onLinkActivated: { Qt.openUrlExternally(link) }
        Behavior on opacity { NumberAnimation { duration: 250; easing.type: Easing.InQuart } }
    }

}
