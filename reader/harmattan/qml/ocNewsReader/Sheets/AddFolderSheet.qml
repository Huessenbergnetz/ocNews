import QtQuick 1.1
import com.nokia.meego 1.0

Sheet {
    acceptButtonText: qsTr("Add")
    rejectButtonText: qsTr("Cancel")

    property alias folderNameText: folderName.text

    content: Flickable {
            anchors { fill: parent; leftMargin: 10; topMargin: 10; rightMargin: 10 }
            flickableDirection: Flickable.VerticalFlick
            Label {
                id: addFolderLabel
                text: qsTr("Add Folder")
                anchors.top: parent.top
                platformStyle: LabelStyle { fontPixelSize: 30 }
                textFormat: Text.PlainText
            }
            Rectangle {
                id: addFolderLabelLine
                height: 1
                width: parent.width
                color: "white"
                anchors { top: addFolderLabel.bottom; topMargin: 5 }
            }
            Label {
                id: addFolderName
                text: qsTr("Name")
                anchors { top: addFolderLabelLine.bottom; topMargin: 10 }
                textFormat: Text.PlainText
            }
            TextField {
                id: folderName
                anchors { top: addFolderName.bottom; topMargin: 10 }
                height: 50
                width: parent.width
                placeholderText: qsTr("Enter folder name")
            }
    }
    onAccepted: if (folderName.text !== "") folders.createFolder(folderName.text);
}

