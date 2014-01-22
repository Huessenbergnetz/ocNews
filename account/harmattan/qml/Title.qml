import QtQuick 1.1
import com.nokia.meego 1.0

Item {
    id: container

    property alias title: titleLabel.text
    property alias subtitle: subtitleLabel.text
    property alias showseparator: separator.visible
    property alias showcheckbox: checkbox.visible
    property alias image: imageElement.source
    property alias checked: checkbox.checked
    property alias extraheight: subtitleLabel.height

    height: 92 + subtitleLabel.height + separator.height

    Column {
        id: column
        width: container.width
        spacing: 10

        Item {
            width: parent.width
            height: 72

            Image {
                id: imageElement
                width: 64
                height: 64
                smooth: true
                visible: status == Image.Ready
            }

            Label {
                id: titleLabel
                anchors.verticalCenter: imageElement.verticalCenter
                anchors.right: checkbox.left
                anchors.left: imageElement.right
                anchors.leftMargin: 20

                text: container.title
                font.pixelSize: 32
                verticalAlignment: Text.AlignVCenter
                wrapMode: Text.NoWrap
            }

            Switch {
                id: checkbox
                anchors.verticalCenter: imageElement.verticalCenter
                anchors.right: parent.right
            }
        }

        Label {
            id: subtitleLabel
            text: container.subtitle
            width: parent.width
            wrapMode: Text.Wrap
            visible: text.length > 0
        }

        Image {
            id: separator
            width: parent.width
            source: "image://theme/meegotouch-separator-" + (theme.inverted ?  "inverted-" : "")+ "background-horizontal"
        }
    }
}
