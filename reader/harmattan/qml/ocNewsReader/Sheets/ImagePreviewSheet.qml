import QtQuick 1.1
import com.nokia.meego 1.0

Sheet {

    acceptButtonText: qsTr("Open in browser")
    rejectButtonText: qsTr("Close")

    property string link

    content: Item {
        anchors.fill: parent


        Image {
            id: icon
            anchors { bottom: openImg.top; bottomMargin: 20; horizontalCenter: parent.horizontalCenter }
            source: "image://theme/icon-l-image"
            width: 80; height: 80
        }

        Button {
            id: openImg
            text: qsTr("Load image")
            anchors.centerIn: parent
            onClicked: { image.visible = true; image.source = link; icon.visible = false; visible = false }
            platformStyle: ButtonStyle {}
        }

        Image {
            id: image
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            visible: false
            fillMode: Image.PreserveAspectFit
            opacity: status === Image.Ready ? 1 : 0
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
            visible: image.status != Image.Ready && image.visible
        }

    }

    onAccepted: { Qt.openUrlExternally(link); link = ""; image.source = ""; image.visible = false; icon.visible = true; openImg.visible = true }
    onRejected: { link = ""; image.source = ""; image.visible = false; icon.visible = true; openImg.visible = true }
}
