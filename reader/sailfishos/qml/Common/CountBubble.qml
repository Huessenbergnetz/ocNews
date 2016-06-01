import QtQuick 2.2
import Sailfish.Silica 1.0

Item {
    id: countBubble

    property int value: 0
    property alias color: count.color

    property int fontSize: Screen.sizeCategory >= Screen.Large
                          ? Theme.fontSizeSmall
                          : Theme.fontSizeExtraSmall

    function getBubbleWidth() {
        if (countBubble.value < 10)
            return fontSize*0.625*(1+0.5);
        else if (countBubble.value < 100)
            return fontSize*0.625*(2+0.5);
        else if (countBubble.value < 1000)
            return fontSize*0.625*(3+0.5);
        else
            return fontSize*0.625*(4+0.5);
    }

    width: countBubble.getBubbleWidth()
    height: fontSize*1.2

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
        font.pixelSize: fontSize
        opacity: 1
        color: Theme.primaryColor
    }
}
