import QtQuick 1.1
import com.nokia.meego 1.0

Sheet {
    id: renameFeedSheet
    property string feedId
    property string feedName
    property alias newfeedName: feedNewName.text
    acceptButtonText: qsTr("Rename")
    rejectButtonText: qsTr("Cancel")

    content: Flickable {
            anchors { fill: parent; leftMargin: 10; topMargin: 10; rightMargin: 10 }
            flickableDirection: Flickable.VerticalFlick
            Label {
                id: renameFeedLabel
                text: qsTr("Rename feed<br />%1").arg(renameFeedSheet.feedName)
                anchors.top: parent.top
                platformStyle: LabelStyle { fontPixelSize: 30 }
                textFormat: Text.StyledText
            }
            Rectangle {
                id: renameFeedLabelLine
                height: 1
                width: parent.width
                color: "white"
                anchors { top: renameFeedLabel.bottom; topMargin: 5 }
            }
            Label {
                id: renamefeedName
                text: qsTr("Name")
                anchors { top: renameFeedLabelLine.bottom; topMargin: 10 }
                textFormat: Text.PlainText
            }
            TextField {
                id: feedNewName
                anchors { top: renamefeedName.bottom; topMargin: 10 }
                height: 50
                width: parent.width
                text: renameFeedSheet.feedName
                placeholderText: qsTr("Enter new feed name")
            }
    }
    onAccepted: {
        if (feedName !== feedNewName.text && feedNewName.text !== "") feeds.renameFeed(renameFeedSheet.feedId, feedNewName.text)
    }
}

