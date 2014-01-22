import QtQuick 2.0
import Sailfish.Silica 1.0

Item {
    id: countBubble

    property int value: 0

    function getBubbleWidth() {
        if (countBubble.value < 10)
            return 26;
        else if (countBubble.value < 100)
            return 35;
        else if (countBubble.value < 1000)
            return 48;
        else
            return 58;
    }

    width: countBubble.getBubbleWidth()
    height: 32

    Rectangle {
        id: backgroundRect
        width: parent.width
        height: parent.height
        color: Theme.secondaryColor
        border.color: Theme.primaryColor
        border.width: 1
        radius: 10
        opacity: 0.3
    }

    Text {
        id: count
        anchors.centerIn: backgroundRect
        text: countBubble.value
        font.pixelSize: Theme.fontSizeExtraSmall
        opacity: 1
        color: Theme.primaryColor
    }
}
