import QtQuick 2.0
import Sailfish.Silica 1.0

Text {
    visible: false
    clip: true
    anchors { left: parent.left; right: parent.right; rightMargin: Theme.paddingLarge }
    textFormat: Text.RichText
    color: Theme.primaryColor
    wrapMode: Text.WordWrap
    font.pixelSize: Theme.fontSizeSmall
    onLinkActivated: { Qt.openUrlExternally(link) }
}
