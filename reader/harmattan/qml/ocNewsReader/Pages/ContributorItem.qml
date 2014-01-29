import QtQuick 1.1
import com.nokia.meego 1.0
import "../Common"

Row {
    spacing: 10
    width: parent.width

    property string image
    property alias name: contribName.text
    property alias role: contribRole.text
    property string website
    property string twitter
    property string github
    property string linkedin
    property string weibo


    Image {
        id:contribImage
        source: image ? "images/" + image : "images/placeholder.png"
        sourceSize.height: image ? 86 : 80
        sourceSize.width: image ? 86 : 80
        smooth: image ? false : true
        width: 86
        height: 86
    }

    Column {
        spacing: 1
        width: parent.width - contribImage.width

        Label {
            id: contribName
            width: parent.width
            wrapMode: Text.WordWrap
        }

        Label {
            id: contribRole
            width: parent.width
            wrapMode: Text.WordWrap
        }

        Row {
            id: imageLinks
            spacing: 10

            ImageButton {
                id: browserIcon
                source: "image://theme/icon-l-browser"
                sourceSize.height: 80
                sourceSize.width: 80
                width: 48
                height: 48
                smooth: true
                visible: website ? true : false
                onClicked: Qt.openUrlExternally(website)
            }

            ImageButton {
                id: twitterIcon
                source: "image://theme/icon-l-twitter"
                sourceSize.height: 80
                sourceSize.width: 80
                height: 48
                width: 48
                smooth: true
                visible: twitter ? true : false
                onClicked: Qt.openUrlExternally("https://twitter.com/" + twitter)
            }

            ImageButton {
                id: githubIcon
                source: "icons/github.png"
                sourceSize.height: 50
                sourceSize.width: 50
                height: 48
                width: 48
                smooth: true
                visible: github ? true : false
                onClicked: Qt.openUrlExternally("https://github.com/" + github)
            }

            ImageButton {
                id: linkedinIcon
                source: "icons/linkedin.png"
                sourceSize.height: 50
                sourceSize.width: 50
                height: 48
                width: 48
                smooth: true
                visible: linkedin ? true : false
                onClicked: Qt.openUrlExternally("http://www.linkedin.com/profile/view?id=" + linkedin)
            }

            ImageButton {
                id: weiboIcon
                source: "icons/weibo.png"
                sourceSize.height: 48
                sourceSize.width: 48
                height: 48
                width: 48
                visible: weibo ? true : false
                onClicked: Qt.openUrlExternally("http://www.weibo.com/" + weibo)
            }

        }
    }
}
