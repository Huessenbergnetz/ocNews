import QtQuick 2.0
import Sailfish.Silica 1.0

import "../Common"

Dialog {
    id: createFeed

    property string folderId
    property string folderName

    canAccept: feedAddress.text
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
            acceptText: qsTr("Add feed")
        }

        TextField {
            id: feedAddress
            anchors { left: parent.left; right: parent.right }
            focus: false; label: qsTr("Feed address"); placeholderText: label
            EnterKey.enabled: text || inputMethodComposing
            EnterKey.onClicked: feedAddress.focus = false
            EnterKey.iconSource: "image://theme/icon-m-enter-close"
        }

//        ComboBox {
//            id: folderChooser
//            visible: folderId === ""
//            anchors { left: parent.left; right: parent.right }
//            label: qsTr("Add feed to folder")
//            menu: ContextMenu {
//                  Repeater {
//                       model: folderModelData
//                       MenuItem { text: model.name }
//                  }
//            }
//        }

        ComboBoxList {
            id: folderChooser
            anchors { left: parent.left; right: parent.right }
            label: qsTr("Add feed to folder")
            model: folderModelData
            initialValue: folderId
        }

//        Label {
//            id: addToFolder
//            visible: !folderChooser.visible
//            anchors { left: parent.left; leftMargin: Theme.paddingLarge; right: parent.right; rightMargin: Theme.paddingLarge }
//            text: qsTr("Add feed to folder<br />%1").arg(folderName)
//            textFormat: Text.StyledText
//        }

        TextSwitch {
            id: addToEventSwitch
            text: qsTr("Show feed in event view")
            visible: false
            anchors { left: parent.left; right: parent.right }
//            description: qsTr("Only ignore SSL errors when you really know what you are doing.")
            checked: false
        }
    }

    onAccepted: feeds.createFeed(feedAddress.text, folderModelData.get(folderChooser.currentIndex).value, addToEventSwitch.checked)

}
