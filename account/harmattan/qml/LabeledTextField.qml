import QtQuick 1.1
import com.nokia.meego 1.0

Item {
    id: container

    property alias text: edit.text
    property alias label: labeled.text
    property alias placeholderText: edit.placeholderText
    property alias inputMethodHints: edit.inputMethodHints
    property alias echoMode: edit.echoMode
    property alias clearbutton: button.visible

    height: labeled.height + edit.height

    Column {
        width: parent.width

        Label {
            id: labeled
            width: parent.width
            color: "#8c8c8c"
        }
        TextField {
            id: edit
            width: parent.width
            Image {
                id: button
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                smooth: true
                source: "image://theme/icon-m-input-clear"
                visible: false
                MouseArea {
                    anchors.centerIn: parent
                    height: parent.parent.height*2
                    width: parent.parent.height*2
                    onClicked: parent.parent.text = ""
                }
            }
        }
    }
}
