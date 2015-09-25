import QtQuick 2.2
import Sailfish.Silica 1.0

Dialog {

    allowedOrientations: Orientation.Landscape | Orientation.Portrait

    property string link

    function isMailTo(url) {
        var string = url;
        if (string.indexOf("mailto") !== -1)
        {
            return true;
        } else {
            return false;
        }
    }

    function isImageLink(url) {
        var string = url;
        if (string.indexOf(".jpeg") !== -1 || string.indexOf(".JPEG") !== -1)
        {
            return true;
        } else if (string.indexOf(".jpg") !== -1 || string.indexOf(".JPG") !== -1) {
            return true;
        } else if (string.indexOf(".png") !== -1 || string.indexOf(".PNG") !== -1) {
            return true;
        } else if (string.indexOf(".gif") !== -1 || string.indexOf(".GIF") !== -1) {
            return true;
        } else {
            return false;
        }
    }

    function stripMailTo(url)
    {
        var string = url;
        return string.replace("mailto:","");
    }

    DialogHeader {
        dialog: parent
        defaultAcceptText: qsTr("Open")
        acceptText: isMailTo(link) ? qsTr("Write e-mail") : qsTr("Open in browser")
        cancelText: ""
    }

    IconButton {
        id: imgBut
        icon.source: "image://theme/icon-l-image"
        visible: isImageLink(link) && image.status != Image.Ready
        anchors { left: parent.left; right: parent.right; verticalCenter: parent.verticalCenter }
        onClicked: { image.visible = true; image.source = link; visible = false; txtBut.visible = false }
    }

    Text {
        id: txtBut
        text: qsTr("Click button to load image")
        color: Theme.secondaryColor
        anchors { top: imgBut.bottom; topMargin: 5; horizontalCenter: parent.horizontalCenter}
        visible: isImageLink(link) && image.status != Image.Ready
        horizontalAlignment: Text.AlignHCenter
    }

    Image {
        id: image
        anchors { left: parent.left; right: parent.right; verticalCenter: parent.verticalCenter }
        fillMode: Image.PreserveAspectFit
        visible: false
        opacity: status === Image.Ready ? 1 : 0
        Behavior on opacity { FadeAnimation {} }
    }

    ProgressBar {
        anchors { verticalCenter: parent.verticalCenter; horizontalCenter: parent.horizontalCenter }
        value: image.progress
        minimumValue: 0
        maximumValue: 1.0
        enabled: false
        width: parent.width
        visible: image.status != Image.Ready && image.visible
    }

    Label {
        anchors { left: parent.left; right: parent.right; leftMargin: Theme.horizontalPageMargin; rightMargin: Theme.horizontalPageMargin; verticalCenter: !isImageLink(link) ? parent.verticalCenter : ""; bottom: isImageLink(link) ? parent.bottom : ""; bottomMargin: isImageLink(link) ? Theme.paddingLarge : ""}
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: Theme.fontSizeMedium
        color: Theme.highlightColor
        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
        text: isMailTo(link) ? stripMailTo(link) : link
        opacity: image.status != Image.Ready
        Behavior on opacity { FadeAnimation {} }
    }

    onAccepted: {
        Qt.openUrlExternally(link);
    }

}
