import QtQuick 2.2
import Sailfish.Silica 1.0

Dialog {
    id: createFolder

    canAccept: folderName.text
    acceptDestinationAction: PageStackAction.Pop

    Column {
        width: parent.width

        DialogHeader {
            acceptText: qsTr("Add folder")
        }

        TextField {
            id: folderName
            anchors { left: parent.left; right: parent.right }
            focus: true; label: qsTr("Folder name"); placeholderText: label
            EnterKey.enabled: text || inputMethodComposing
            EnterKey.iconSource: "image://theme/icon-m-enter-accept"
            EnterKey.onClicked: createFolder.accept()
        }
    }

    onAccepted: folders.createFolder(folderName.text)

}
