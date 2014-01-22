import QtQuick 1.1
import com.nokia.meego 1.0

Item {
    property alias text: button.text
    signal clicked()
    id: buttonElement
    height: button.height + 32
    width: 322
    Button {
        id: button
        width: parent.width
        height: 51
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        onClicked: buttonElement.clicked()
        enabled: buttonElement.enabled
    }
}
