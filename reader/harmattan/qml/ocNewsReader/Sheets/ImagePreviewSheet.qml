import QtQuick 1.1
import com.nokia.meego 1.0

Sheet {

    acceptButtonText: qsTr("Open in browser")
    rejectButtonText: qsTr("Close")

    property string link

    content: Item {
        anchors.fill: parent

        Image {
            id: image
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            fillMode: Image.PreserveAspectFit
            opacity: status === Image.Ready ? 1 : 0
            source: link
            Behavior on opacity {
                NumberAnimation {
                    duration: 200
                    easing.type: Easing.InOutQuad
                    property: "opacity"
                }
            }
        }

        ProgressBar {
            value: image.progress
            minimumValue: 0
            maximumValue: 1.0
            width: parent.width - 40
            anchors { verticalCenter: parent.verticalCenter; horizontalCenter: parent.horizontalCenter }
            visible: image.status != Image.Ready
        }

    }

    onAccepted: Qt.openUrlExternally(link)
}
