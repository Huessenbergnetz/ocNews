import QtQuick 2.2
import Sailfish.Silica 1.0

Item {
    id: root

    property Item contextMenu
    property bool menuOpen: contextMenu != null && contextMenu.parent === bgItem


    property string enclosureItemId
    property string encSrc
    property int encType
    property string encMime

    property alias name: name.text
    property alias host: host.text

    property bool enclosureDownloading: downloads.getCurrentItem() === enclosureItemId
    property bool enclosureInQueue: downloads.itemInQueue(enclosureItemId)
    property bool enclosureExists: downloads.itemExists(encSrc, encMime) !== "" && !enclosureDownloading && !enclosureInQueue

    Connections {
        target: downloads
        onStarted: if(startedFileId === enclosureItemId) { enclosureDownloading = true; enclosureInQueue = false }
        onFinishedFile: if(finishedFileId === enclosureItemId) { enclosureDownloading = false; enclosureExists = downloads.itemExists(encSrc, encMime) !== "" }
    }

    width: parent.width
    height: header.height + encRow.height

    function getContentIcon()
    {
        var ret;

        switch(encType) {
        case 1:
            ret = "image://theme/icon-m-music";
            break;
        case 2:
            ret = "image://theme/icon-m-video"
            break;
        case 3:
            ret = "image://theme/icon-m-document"
            break;
        case 4:
            ret = "image://theme/icon-m-image"
            break;
        default:
            ret = "image://theme/icon-m-attach";
            break;
        }

        return ret;
    }

    function downloadEnclosure()
    {
        enclosureInQueue = true;
        downloads.append(enclosureItemId);
    }

    function abortEnclosureDownload()
    {
        enclosureInQueue = false;
        downloads.abortDownload(enclosureItemId);
    }

    function removeFile(src, mime) {
        remorse.execute(bgItem, qsTr("Deleting %1").arg(name.text),  function() { enclosureExists = !downloads.deleteFile(src, mime) } );
    }

    SectionHeader { id: header; text: qsTr("Enclosure") }

    BackgroundItem {
        id: bgItem
        height: menuOpen ? encRow.height + contextMenu.height : encRow.height
        width: parent.width
        anchors { top: header.bottom; topMargin: 5 }
        onClicked: {
            if (!contextMenu) contextMenu = contextMenuComp.createObject(root)
            contextMenu.show(bgItem)
        }

        Row {
            id: encRow
            spacing: 10
            width: parent.width
            anchors { left: parent.left; right: parent.right; leftMargin: Theme.horizontalPageMargin; rightMargin: Theme.horizontalPageMargin }


            Image {
                id: encIcon
                height: 64; width: 64
                source: root.getContentIcon()
            }

            Column {
                width: parent.width - encIcon.width

                Text {
                    id: name
                    width: parent.width
                    elide: Text.ElideRight
                    font.pixelSize: Theme.fontSizeSmall
                    font.weight: Font.Light
                    textFormat: Text.PlainText
                    color: Theme.primaryColor
                    visible: !downloadBar.visible
                }

                Text {
                    id: host
                    width: parent.width
                    elide: Text.ElideRight
                    font.pixelSize: Theme.fontSizeSmall
                    font.weight: Font.Light
                    textFormat: Text.PlainText
                    color: Theme.secondaryColor
                    visible: !enclosureExists && !enclosureInQueue && !downloadBar.visible
                }

                Text {
                    id: exists
                    text: qsTr("Download finished")
                    width: parent.width
                    elide: Text.ElideRight
                    font.pixelSize: Theme.fontSizeSmall
                    font.weight: Font.Light
                    textFormat: Text.PlainText
                    color: Theme.secondaryColor
                    visible: enclosureExists
                }

                Text {
                    id: inQueue
                    text: qsTr("In queue")
                    width: parent.width
                    elide: Text.ElideRight
                    font.pixelSize: Theme.fontSizeSmall
                    font.weight: Font.Light
                    textFormat: Text.PlainText
                    color: Theme.secondaryColor
                    visible: enclosureInQueue
                }

                ProgressBar {
                    id: downloadBar
                    minimumValue: 0
                    enabled: false
                    width: parent.width
                    visible: enclosureDownloading
                    label: name.text
                    Connections {
                        target: downloads
                        onProgress: if(enclosureDownloading) { downloadBar.maximumValue = tot; downloadBar.value = rec }
                    }
                }
            }
        }

        RemorseItem {
            id: remorse
        }
    }

    Component {
        id: contextMenuComp
        ContextMenu {
            MenuItem {
//                text: enclosureExists ? qsTr("Open enclosure") : qsTr("Open enclosure directly")
                text: qsTr("Open enclosure")
                enabled: !enclosureDownloading && !enclosureInQueue && enclosureExists
//                onClicked: enclosureExists ? Qt.openUrlExternally(downloads.itemExists(encSrc, encMime)) : ""
                onClicked: Qt.openUrlExternally(downloads.itemExists(encSrc, encMime))
            }
            MenuItem {
                text: enclosureDownloading || enclosureInQueue ? qsTr("Abort download") : enclosureExists ? qsTr("Delete enclosure") : qsTr("Download enclosure")
                onClicked: enclosureDownloading || enclosureInQueue ? root.abortEnclosureDownload() : enclosureExists ? root.removeFile(encSrc, encMime) : root.downloadEnclosure()
            }
        }
    }
}
