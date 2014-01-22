import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.0

import "../Common"

Sheet {
    acceptButtonText: qsTr("Move")
    rejectButtonText: qsTr("Cancel")

    property string feedId
    property string feedName

    ListModel {
        id: folderModelData
        ListElement { name: "Root"; value: "0" }
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
                id: moveFeedLabel
                text: qsTr("Move feed to another folder")
                anchors { top: parent.top; leftMargin: 5 }
                platformStyle: LabelStyle { fontPixelSize: 30 }
                textFormat: Text.PlainText
            }

            Rectangle {
                id: moveFeedLabelLine
                height: 1
                width: parent.width
                color: "white"
                anchors { top: moveFeedLabel.bottom; topMargin: 10 }
            }

            Label {
                id:feedNameLabel
                text: feedName
                anchors { top: moveFeedLabelLine.bottom; topMargin: 20 }
                platformStyle: LabelStyle { fontPixelSize: 24 }
                textFormat: Text.PlainText
            }


            SelectionItem {
                id: chooseFolder
                visible: true
                title: qsTr("Select folder")
                anchors { top: feedNameLabel.bottom; topMargin: 20 }
                model: folderModelData
                initialValue: "0"
            }

    }
    onAccepted: feeds.moveFeed(feedId, chooseFolder.currentValue)
}

