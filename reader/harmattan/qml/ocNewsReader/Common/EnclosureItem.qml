import QtQuick 1.1
import com.nokia.meego 1.0

Item {
    id: root
    width: parent.width
    height: header.height + encRow.height

    signal clicked

    property alias name: name.text
    property alias host: host.text
    property alias showHost: host.visible
    property alias exists: exists.visible
    property alias inQueue: inQueue.visible
    property alias showProgress: downloadBar.visible
    property alias value: downloadBar.value
    property alias maxValue: downloadBar.maximumValue
    property int encType

    function getContentIcon()
    {
        var ret;

        switch(encType) {
        case 1:
            ret = "image://theme/icon-m-content-audio";
            break;
        case 2:
            ret = "image://theme/icon-m-content-videos"
            break;
        case 3:
            ret = "image://theme/icon-m-content-pdf"
            break;
        case 4:
            ret = "image://theme/icon-m-content-image"
            break;
        default:
            ret = "image://theme/icon-m-content-attachment";
            break;
        }

        ret += (theme.inverted? "-inverse" : "");

        return ret;
    }

    GroupHeader { id: header; text: qsTr("Enclosure") }

    Item {
        width: parent.width
        height: encRow.height
        anchors { top: header.bottom; topMargin: 5 }

        BorderImage {
            id: background
            anchors.fill: parent
            visible: mouseArea.pressed
            source: theme.inverted ? "image://theme/meegotouch-panel-inverted-background-pressed" : "image://theme/meegotouch-panel-background-pressed"
        }

        Row {
            id: encRow
            spacing: 10
            width: parent.width


            Image {
                id: encIcon
                height: 64; width: 64
                source: root.getContentIcon()
            }

            Column {
                width: parent.width - encIcon.width

                Text {
                    id: name
                    text: enclosureName
                    width: parent.width
                    elide: Text.ElideRight
                    font.pointSize: 17
                    font.weight: Font.Light
                    textFormat: Text.PlainText
                    color: theme.inverted ? "white" : "black"
                }

                Text {
                    id: host
                    text: enclosureHost
                    width: parent.width
                    elide: Text.ElideRight
                    font.pointSize: 17
                    font.weight: Font.Light
                    textFormat: Text.PlainText
                    color: theme.inverted ? "white" : "black"
                }

                Text {
                    id: exists
                    text: qsTr("Download finished")
                    width: parent.width
                    elide: Text.ElideRight
                    font.pointSize: 17
                    font.weight: Font.Light
                    textFormat: Text.PlainText
                    color: theme.inverted ? "white" : "black"
                }

                Text {
                    id: inQueue
                    text: qsTr("In queue")
                    width: parent.width
                    elide: Text.ElideRight
                    font.pointSize: 17
                    font.weight: Font.Light
                    textFormat: Text.PlainText
                    color: theme.inverted ? "white" : "black"
                }

                ProgressBar {
                    id: downloadBar
                    minimumValue: 0
                    width: parent.width - 20
                }
            }

            MouseArea {
                id: mouseArea
                anchors.fill: parent
                onClicked: root.clicked()
            }
        }
    }
}
