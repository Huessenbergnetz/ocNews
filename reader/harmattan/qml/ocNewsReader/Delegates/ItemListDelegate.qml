import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.0
import "../uiconstants.js" as UI


Item {
    id: itemListItem

    signal clicked
    signal pressAndHold
    property alias pressed: mouseArea.pressed

    property int titleSize: 24
    property int titleWeight: Font.Bold
    property string titleFont: UI.FONT_FAMILY
    property color titleColor: theme.inverted ? UI.LIST_TITLE_COLOR_INVERTED : UI.LIST_TITLE_COLOR
    property color titleColorPressed: theme.inverted ? UI.LIST_TITLE_COLOR_PRESSED_INVERTED : UI.LIST_TITLE_COLOR_PRESSED

    property int subtitleSize: 15
    property int subtitleWeight: Font.Normal
    property string subtitleFont: UI.FONT_FAMILY_LIGHT
    property color subtitleColor: theme.inverted ? UI.LIST_SUBTITLE_COLOR_INVERTED : UI.LIST_SUBTITLE_COLOR
    property color subtitleColorPressed: theme.inverted ? UI.LIST_SUBTITLE_COLOR_PRESSED_INVERTED : UI.LIST_SUBTITLE_COLOR_PRESSED

//    height: 110
    height: Math.max(textCol.height, iconCol.height) + 12
    width: parent.width

    BorderImage {
        id: background
        anchors.fill: parent
        // Fill page porders
        anchors { leftMargin: -UI.MARGIN_XLARGE; rightMargin: -UI.MARGIN_XLARGE }
        visible: mouseArea.pressed
        source: theme.inverted ? "image://theme/meegotouch-panel-inverted-background-pressed" : "image://theme/meegotouch-panel-background-pressed"
    }

    Rectangle {
        anchors { verticalCenter: parent.verticalCenter; left: parent.left; leftMargin: 8 }
        height: itemListItem.height - 10
        width: 8
        color: "#f78500"
        visible: model.unread
    }

    Row {
        spacing: UI.LIST_ITEM_SPACING
        anchors { left: parent.left; leftMargin: 24; right: parent.right; rightMargin: 10; top: parent.top; topMargin: 10 }

        Column {
            id: textCol
            width: parent.width - iconCol.width

            Label {
                id: mainText
                text: model.title
                width: parent.width
                font.family: itemListItem.titleFont
                font.weight: itemListItem.titleWeight
                font.pixelSize: itemListItem.titleSize
                color: mouseArea.pressed ? itemListItem.titleColorPressed : itemListItem.titleColor
                maximumLineCount: 2
                elide: Text.ElideRight
                textFormat: Text.PlainText
            }

            Label {
                id: excerptText
                text: model.excerpt
                width: parent.width
                font.family: itemListItem.subtitleFont
                font.weight: itemListItem.subtitleWeight
                font.pixelSize: itemListItem.titleSize
                color: itemListItem.subtitleColor
                maximumLineCount: 3
                elide: Text.ElideRight
                textFormat: Text.PlainText
                visible: text !== ""
            }

            Label {
                id:dateText
                text: model.pubDate
                font.family: itemListItem.subtitleFont
                font.weight: itemListItem.subtitleWeight
                font.pixelSize: itemListItem.subtitleSize
                color: model.unread ? "#f78500" : itemListItem.subtitleColor
                textFormat: Text.PlainText
            }
        }

        Column {
            id: iconCol
            width: Math.max(articlePic.width, 32)

            Image {
                visible: model.starred
                width: 32
                height: 32
                source: "image://theme/icon-s-common-favorite-mark" + (theme.inverted? "-inverse" : "")
            }

            Image {
                visible: model.enclosureLink
                width: 32
                height: 32
                source: "image://theme/icon-m-content-attachment" + (theme.inverted? "-inverse" : "")
            }

            Image {
                id: articlePic
                visible: model.image
                source: model.image
                width: visible ? 80 : 0
                fillMode: Image.PreserveAspectCrop
            }
        }
    }

    MouseArea {
        id: mouseArea;
        anchors.fill: parent
        onClicked: itemListItem.clicked()
        onPressAndHold: itemListItem.pressAndHold()
    }

    Image {
       height: 2
       width: parent.width
       anchors { left: parent.left; leftMargin: 16; right: parent.right; top: parent.bottom }
       source: "image://theme/meegotouch-separator" + (theme.inverted? "-inverted" : "") + "-background-horizontal"
       fillMode: Image.TileHorizontally
    }

}


