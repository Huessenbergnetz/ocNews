import QtQuick 2.0
import Sailfish.Silica 1.0

Item {
//    property int itemsToFetchImages: items.isFetchImagesRunning() + updater.isFetchImagesRunning()
    property int itemsToFetchImages: imageFetcher.isFetchImagesRunning()
    property int currentItemFetching: 0

//    Connections {
//        target: updater
//        onStartedFetchingImages: itemsToFetchImages = numberOfItems
//        onFinishedFetchingImages: itemsToFetchImages = 0
//        onFetchingImages: currentItemFetching = currentItem
//    }
//    Connections {
//        target: items
//        onStartedFetchingImages: itemsToFetchImages = numberOfItems
//        onFinishedFetchingImages: itemsToFetchImages = 0
//        onFetchingImages: currentItemFetching = currentItem
//    }

    Connections {
        target: imageFetcher
        onStartedFetchingImages: itemsToFetchImages = numberOfItems
        onFinishedFetchingImages: itemsToFetchImages = 0
        onFetchingImages: currentItemFetching = currentItem
    }

    width: parent.width
    height: itemsToFetchImages > 0 ? 110 : 0
    visible: height > 0
    anchors.bottom: parent.bottom

    Behavior on height {
        NumberAnimation { duration: 300; easing.type: Easing.InOutQuad }
    }

    ProgressBar {
        id: fetchImagesSlider
        value: currentItemFetching
        minimumValue: 0
        maximumValue: itemsToFetchImages
        enabled: false
        width: parent.width
        label: qsTr("Fetching images")
    }
}
