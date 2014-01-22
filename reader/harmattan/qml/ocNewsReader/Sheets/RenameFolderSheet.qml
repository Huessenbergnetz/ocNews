import QtQuick 1.1
import com.nokia.meego 1.0

Sheet {
    id: renameFolderSheet
    property string folderId
    property string folderName
    property alias newFolderName: folderNewName.text
    acceptButtonText: qsTr("Rename")
    rejectButtonText: qsTr("Cancel")

    content: Flickable {
            anchors { fill: parent; leftMargin: 10; topMargin: 10; rightMargin: 10 }
            flickableDirection: Flickable.VerticalFlick
            Label {
                id: renameFolderLabel
                text: qsTr("Rename Folder<br />%1").arg(renameFolderSheet.folderName)
                anchors.top: parent.top
                platformStyle: LabelStyle { fontPixelSize: 30 }
                textFormat: Text.StyledText
            }
            Rectangle {
                id: renameFolderLabelLine
                height: 1
                width: parent.width
                color: "white"
                anchors { top: renameFolderLabel.bottom; topMargin: 5 }
            }
            Label {
                id: renameFolderName
                text: qsTr("Name")
                anchors { top: renameFolderLabelLine.bottom; topMargin: 10 }
                textFormat: Text.PlainText
            }
            TextField {
                id: folderNewName
                anchors { top: renameFolderName.bottom; topMargin: 10 }
                height: 50
                width: parent.width
                text: renameFolderSheet.folderName
                placeholderText: qsTr("Enter new folder name")
            }
    }
    onAccepted: {
        if (folderName !== folderNewName.text && folderNewName.text !== "") folders.renameFolder(renameFolderSheet.folderId, folderNewName.text)
    }
}

