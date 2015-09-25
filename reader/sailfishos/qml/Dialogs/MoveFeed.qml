import QtQuick 2.2
import Sailfish.Silica 1.0

import "../BTComponents"

Dialog {
    id: moveFeed

    property string feedId
    property string feedName
    property string folderId

    canAccept: folderId != folderModelData.get(folderChooser.currentIndex).value
    acceptDestinationAction: PageStackAction.Pop

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

    Column {
        width: parent.width

        DialogHeader {
            acceptText: qsTr("Move feed")
        }

        Label {
            id: feed
            anchors { left: parent.left; leftMargin: Theme.paddingLarge; right: parent.right; rightMargin: Theme.paddingLarge }
            text: feedName
        }

        ComboBoxList {
            id: folderChooser
            anchors { left: parent.left; right: parent.right }
            label: qsTr("Move feed to folder")
            model: folderModelData
            initialValue: folderId
        }
    }

    onAccepted: feeds.moveFeed(feedId, folderModelData.get(folderChooser.currentIndex).value)

}
