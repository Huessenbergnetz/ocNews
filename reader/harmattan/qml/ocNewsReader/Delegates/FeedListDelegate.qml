import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.0
import "../Common"
import "../uiconstants.js" as UI


Item {
    id: feedListItem

    signal clicked
    signal pressAndHold
    property alias pressed: mouseArea.pressed

    property int titleSize: UI.LIST_TILE_SIZE
    property int titleWeight: Font.Bold
    property string titleFont: UI.FONT_FAMILY
    property color titleColor: theme.inverted ? UI.LIST_TITLE_COLOR_INVERTED : UI.LIST_TITLE_COLOR
    property color titleColorPressed: theme.inverted ? UI.LIST_TITLE_COLOR_PRESSED_INVERTED : UI.LIST_TITLE_COLOR_PRESSED

    property int subtitleSize: UI.LIST_SUBTILE_SIZE
    property int subtitleWeight: Font.Normal
    property string subtitleFont: UI.FONT_FAMILY_LIGHT
    property color subtitleColor: theme.inverted ? UI.LIST_SUBTITLE_COLOR_INVERTED : UI.LIST_SUBTITLE_COLOR
    property color subtitleColorPressed: theme.inverted ? UI.LIST_SUBTITLE_COLOR_PRESSED_INVERTED : UI.LIST_SUBTITLE_COLOR_PRESSED

    height: UI.LIST_ITEM_HEIGHT
    width: parent.width

    BorderImage {
            id: background
            anchors.fill: parent
            // Fill page porders
            anchors { leftMargin: -UI.MARGIN_XLARGE; rightMargin: -UI.MARGIN_XLARGE }
            visible: mouseArea.pressed
            source: theme.inverted ? "image://theme/meegotouch-panel-inverted-background-pressed" : "image://theme/meegotouch-panel-background-pressed"
        }

    Row {
            anchors.fill: parent
            spacing: UI.LIST_ITEM_SPACING

            Image {
                anchors.verticalCenter: parent.verticalCenter
                asynchronous: true
                width: 32
                height: 32
                sourceSize.width: model.iconSource ? parseInt(model.iconWidth, 10) : 64
                sourceSize.height: model.iconSource ? parseInt(model.iconHeight, 10) : 64
                source: model.iconSource ? model.iconSource : "image://theme/icon-m-content-feed" + (theme.inverted? "-inverse" : "")
            }

            Label {
                id: mainText
                anchors.verticalCenter: parent.verticalCenter
                width: feedListItem.width - 80
                text: model.title
                font.family: feedListItem.titleFont
                font.weight: feedListItem.titleWeight
                font.pixelSize: feedListItem.titleSize
                color: if (model.unreadCount > 0) { mouseArea.pressed ? feedListItem.titleColorPressed : feedListItem.titleColor } else { mouseArea.pressed ? feedListItem.subtitleColorPressed : feedListItem.subtitleColor }
                maximumLineCount: 1
                elide: Text.ElideRight
                textFormat: Text.PlainText
            }
    }

    Column {
        anchors { verticalCenter: parent.verticalCenter; right: parent.right }

        OcCountBubble {
            value: model.unreadCount
            anchors.right: parent.right
        }


        Label {
            id: subText
            anchors.right: parent.right
            text: model.subtitle ? model.subtitle : ""
            font.family: feedListItem.subtitleFont
            font.weight: feedListItem.subtitleWeight
            font.pixelSize: feedListItem.subtitleSize
            color: mouseArea.pressed ? feedListItem.subtitleColorPressed : feedListItem.subtitleColor
            visible: text != ""
            textFormat: Text.PlainText
        }
    }

    MouseArea {
        id: mouseArea;
        anchors.fill: parent
        onClicked: {
            feedListItem.clicked();
        }
        onPressAndHold: {
            feedListItem.pressAndHold();
        }
    }

    Image {
       height: 2
       width: parent.width
       anchors.top: parent.bottom
       source: "image://theme/meegotouch-separator" + (theme.inverted? "-inverted" : "") + "-background-horizontal"
       fillMode: Image.TileHorizontally
    }
}


