import QtQuick 1.1
import com.nokia.meego 1.0

Rectangle {
    width: parent.width
    height: imageFetcher.total > 0 ? 70 : 0
    visible: height > 0
    anchors.bottom: parent.bottom
    color: theme.inverted ? "black" : "#E0E1E2"

    Behavior on height {
        NumberAnimation { duration: 300; easing.type: Easing.InOutQuad }
    }

    Column {
        width: parent.width
        spacing: 2

        Label {
            textFormat: Text.PlainText
            text: qsTr("Fetching images")
            anchors.horizontalCenter: parent.horizontalCenter;
        }

        ProgressBar {
            id: fetchImagesSlider
            value: imageFetcher.current
            minimumValue: 0
            maximumValue: imageFetcher.total
            width: parent.width - 40
            anchors.horizontalCenter: parent.horizontalCenter;
        }
    }
}
