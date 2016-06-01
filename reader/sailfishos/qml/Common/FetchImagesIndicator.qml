import QtQuick 2.2
import Sailfish.Silica 1.0

Item {
    width: parent.width
    height: imageFetcher.total > 0 ? 3*Theme.fontSizeMedium : 0
    visible: height > 0
    anchors.bottom: parent.bottom

    Behavior on height {
        NumberAnimation { duration: 300; easing.type: Easing.InOutQuad }
    }

    ProgressBar {
        id: fetchImagesSlider
        value: imageFetcher.current
        minimumValue: 0
        maximumValue: imageFetcher.total
        enabled: false
        width: parent.width
        label: qsTr("Fetching images")
    }
}
