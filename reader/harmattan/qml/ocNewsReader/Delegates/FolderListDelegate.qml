import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.0
import "../Common"
import "../uiconstants.js" as UI


Item {
    id: folderListItem

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

    function getIconSource()
    {
        switch(model.type) {
        case 1:
            return theme.inverted ? "/opt/ocNewsReader/images/directory-inverse.png" : "image://theme/icon-m-common-directory"
        case 0:
        case 3:
            return "image://theme/icon-m-content-feed" + (theme.inverted? "-inverse" : "")
        case 2:
            return "image://theme/icon-m-content-favourites" + (theme.inverted? "-inverse" : "")
        }
    }

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
            visible: true
            width: 32
            height: 32
            sourceSize.width: model.iconSource ? parseInt(model.iconWidth, 10) : 64
            sourceSize.height: model.iconSource ? parseInt(model.iconHeight, 10) : 64
            source: model.iconSource ? model.iconSource : getIconSource()
        }

        Column {
            anchors.verticalCenter: parent.verticalCenter

            Label {
                id: mainText
                text: model.title
                width: folderListItem.width - 70
                font.family: folderListItem.titleFont
                font.weight: folderListItem.titleWeight
                font.pixelSize: folderListItem.titleSize
                color: if (model.unreadCount > 0) { mouseArea.pressed ? folderListItem.titleColorPressed : folderListItem.titleColor } else { mouseArea.pressed ? folderListItem.subtitleColorPressed : folderListItem.subtitleColor }
                maximumLineCount: 1
                elide: Text.ElideRight
                textFormat: Text.PlainText
            }

            Label {
                id: subText
                text: model.feedCount === 0 ? qsTr("Empty folder") : qsTr("%n unread post(s) in", "", model.unreadCount) + " " + qsTr("%n feed(s)", "", model.feedCount)
                font.family: folderListItem.subtitleFont
                font.weight: folderListItem.subtitleWeight
                font.pixelSize: folderListItem.subtitleSize
                color: mouseArea.pressed ? folderListItem.subtitleColorPressed : folderListItem.subtitleColor
                visible: model.type === 1 ? true : false
                textFormat: Text.PlainText
            }
        }
    }

    Image {
        source: "image://theme/icon-m-common-drilldown-arrow" + (theme.inverted ? "-inverse" : "")
        anchors { right: parent.right; rightMargin: -5; verticalCenter: parent.verticalCenter }
        visible: model.type === 1 ? true : false
    }

    OcCountBubble {
        value: model.unreadCount
        anchors { right: parent.right; verticalCenter: parent.verticalCenter }
        visible: model.type !== 1 ? true : false
    }

    MouseArea {
        id: mouseArea;
        anchors.fill: parent
        onClicked: {
            folderListItem.clicked();
        }
        onPressAndHold: {
            folderListItem.pressAndHold();
        }
    }

    Image {
       height: 2
       width: parent.width
       visible: config.viewMode === 0
       anchors.top: parent.bottom
       source: "image://theme/meegotouch-separator" + (theme.inverted? "-inverted" : "") + "-background-horizontal"
       fillMode: Image.TileHorizontally
    }
}


