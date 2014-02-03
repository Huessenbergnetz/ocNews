import QtQuick 1.1
import com.nokia.meego 1.0

Rectangle {
    property int itemsToFetchImages: items.isFetchImagesRunning() + updater.isFetchImagesRunning()
    property int currentItemFetching: 0

    Connections {
        target: updater
        onStartedFetchingImages: itemsToFetchImages = numberOfItems
        onFinishedFetchingImages: itemsToFetchImages = 0
        onFetchingImages: currentItemFetching = currentItem
    }
    Connections {
        target: items
        onStartedFetchingImages: itemsToFetchImages = numberOfItems
        onFinishedFetchingImages: itemsToFetchImages = 0
        onFetchingImages: currentItemFetching = currentItem
    }

    width: parent.width
    height: itemsToFetchImages > 0 ? 70 : 0
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
            value: currentItemFetching
            minimumValue: 0
            maximumValue: itemsToFetchImages
            width: parent.width - 40
            anchors.horizontalCenter: parent.horizontalCenter;
        }
    }
}
