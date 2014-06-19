import QtQuick 1.1
import com.nokia.meego 1.0

Item {
    id: container

    property alias text: label.text
    property alias checked: checkbox.checked

    height: label.lineCount > 1 ? label.height+10 : checkbox.height+10

    Label {
        id: label
        anchors.verticalCenter: checkbox.verticalCenter
        width: parent.width - checkbox.width
//        wrapMode: Text.NoWrap
        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
        color: checkbox.checked ? theme.inverted? "white" : "black" : "#8c8c8c"
    }
    Switch {
        id: checkbox
        anchors.bottom: parent.bottom
        anchors.right: parent.right
    }
}
