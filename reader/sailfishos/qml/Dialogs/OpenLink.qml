import QtQuick 2.0
import Sailfish.Silica 1.0

Dialog {

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

    function stripMailTo(url)
    {
        var string = url;
        return string.replace("mailto:","");
    }

    DialogHeader {
        title: isMailTo(link) ? qsTr("Write e-mail") : qsTr("Open in browser")
    }

    Label {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: Theme.paddingLarge
        anchors.rightMargin: Theme.paddingLarge
        anchors.verticalCenter: parent.verticalCenter
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
