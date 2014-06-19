import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.0

import "../Common"

Sheet {
    acceptButtonText: qsTr("Add")
    rejectButtonText: qsTr("Cancel")

    property string folderId: ""
    property string folderName
    property alias feedAddressText: feedAddress.text

    ListModel {
        id: folderModelData
        ListElement { name: "Root";  value: "0" }
    }

    function buildFolderList()
    {
        var fol = folders.getFolders();

        folderModelData.clear()
        folderModelData.append({"name":qsTr("Root"), "value": "0"})


        for (var prop in fol)
        {
            folderModelData.append({"name":fol[prop], "value":prop})
        }
    }

    Component.onCompleted: buildFolderList();

    content: Flickable {
            anchors { fill: parent; leftMargin: 10; topMargin: 10; rightMargin: 10 }
            flickableDirection: Flickable.VerticalFlick
            Label {
                id: addFeedLabel
                text: folderId === "" ? qsTr("Add Feed") : qsTr("Add Feed to Folder<br />%1").arg(folderName)
                anchors { top: parent.top; leftMargin: 5 }
                platformStyle: LabelStyle { fontPixelSize: 30 }
                textFormat: Text.StyledText
                width: parent.width
            }

            Rectangle {
                id: addFeedLabelLine
                height: 1
                width: parent.width
                color: "white"
                anchors { top: addFeedLabel.bottom; topMargin: 10 }
            }

            Label {
                id: addFeedAddress
                text: qsTr("Address")
                anchors { top: addFeedLabelLine.bottom; topMargin: 10; leftMargin: 5 }
                textFormat: Text.PlainText
            }

            TextField {
                id: feedAddress
                anchors { top: addFeedAddress.bottom; topMargin: 10; leftMargin: 5 }
                height: 50
                width: parent.width
                placeholderText: qsTr("Enter new feed address")
            }


            SelectionItem {
                id: chooseFolder
                visible: folderId === "" ? true : false
                title: qsTr("Select folder")
                anchors { top: feedAddress.bottom; topMargin: 20 }
                model: folderModelData
                initialValue: "0"
            }

            LabeledSwitch {
                id: addToEventSwitch
                width: parent.width
                anchors { top: chooseFolder.bottom; horizontalCenter: parent.horizontalCenter }
                text: qsTr("Show feed in event view")
            }

    }
    onAccepted: if (feedAddress.text !== "") feeds.createFeed(feedAddress.text, (folderId === "") ? chooseFolder.currentValue : folderId, addToEventSwitch.checked)
}

