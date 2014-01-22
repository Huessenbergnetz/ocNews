import QtQuick 1.1
import com.nokia.meego 1.0
import "../Common"
import "../uiconstants.js" as UI


Item {
    id: eventFeedListItem

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



    Row {
            anchors.fill: parent
            spacing: UI.LIST_ITEM_SPACING

            Switch{
                id: eventState
                checked: model.checked == "true"
                anchors.verticalCenter: parent.verticalCenter
            }

            Label {
                id: mainText
                anchors.verticalCenter: parent.verticalCenter
                width: eventFeedListItem.width
                text: model.name
                font.family: eventFeedListItem.titleFont
                font.weight: eventFeedListItem.titleWeight
                font.pixelSize: eventFeedListItem.titleSize
                color: eventFeedListItem.titleColor
                maximumLineCount: 1
                elide: Text.ElideRight
                textFormat: Text.PlainText
            }
    }
}


