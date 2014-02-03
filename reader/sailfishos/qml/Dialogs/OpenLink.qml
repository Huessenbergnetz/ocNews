import QtQuick 2.0
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
        title: isMailTo(link) ? qsTr("Write e-mail") : qsTr("Open in browser")
    }

    Image {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        fillMode: Image.PreserveAspectFit
        visible: isImageLink(link)
        source: isImageLink(link) ? link : ""
    }

    Label {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: Theme.paddingLarge
        anchors.rightMargin: Theme.paddingLarge
        anchors.verticalCenter: !isImageLink(link) ? parent.verticalCenter : ""
        anchors.bottom: isImageLink(link) ? parent.bottom : ""
        anchors.bottomMargin: isImageLink(link) ? Theme.paddingLarge : ""
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: Theme.fontSizeMedium
        color: Theme.highlightColor
        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
        text: isMailTo(link) ? stripMailTo(link) : link
    }

    onAccepted: {
        Qt.openUrlExternally(link);
    }

}
