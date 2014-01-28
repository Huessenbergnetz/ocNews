import QtQuick 2.0

Image {
    id: root
    signal clicked()
    property alias pressed: mouseArea.pressed
    opacity: pressed ? 0.5 : 1
    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: root.clicked()
    }
}
