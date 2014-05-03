import QtQuick 2.0
import Sailfish.Silica 1.0

Dialog {
    id: renameFeed

    property string feedId
    property string feedName

    canAccept: feedNewName.text !== feedName && feedNewName.text
    acceptDestinationAction: PageStackAction.Pop

    Column {
        width: parent.width

        DialogHeader {
            acceptText: qsTr("Rename feed")
        }

        TextField {
            id: feedNewName
            anchors { left: parent.left; right: parent.right }
            text: feedName
            focus: true; label: qsTr("Folder name"); placeholderText: label
            EnterKey.enabled: text !== feedName && text || inputMethodComposing
            EnterKey.iconSource: "image://theme/icon-m-enter-accept"
            EnterKey.onClicked: renameFeed.accept()
        }
    }

    onAccepted: feeds.renameFeed(renameFeed.feedId, feedNewName.text)

}
