import QtQuick 2.2
import Sailfish.Silica 1.0

Dialog {
    id: renameFolder

    property string folderId
    property string folderName

    canAccept: folderNewName.text != folderName && folderNewName.text
    acceptDestinationAction: PageStackAction.Pop

    Column {
        width: parent.width

        DialogHeader {
            acceptText: qsTr("Rename folder")
        }

        TextField {
            id: folderNewName
            anchors { left: parent.left; right: parent.right }
            text: folderName
            focus: true; label: qsTr("Folder name"); placeholderText: label
            EnterKey.enabled: text != folderName && text || inputMethodComposing
            EnterKey.iconSource: "image://theme/icon-m-enter-accept"
            EnterKey.onClicked: renameFolder.accept()
        }
    }

    onAccepted: folders.renameFolder(renameFolder.folderId, folderNewName.text)

}
