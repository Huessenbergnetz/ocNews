import QtQuick 2.0
import Sailfish.Silica 1.0
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
    property string bitbucket


    Image {
        id:contribImage
        source: image ? "images/" + image : "images/placeholder.png"
        sourceSize.height: 86
        sourceSize.width: 86
        width: 86
        height: 86
    }

    Column {
        spacing: 1
        width: parent.width - contribImage.width

        Label {
            id: contribName
            font.pixelSize: Theme.fontSizeSmall
            width: parent.width
            wrapMode: Text.WordWrap
        }

        Label {
            id: contribRole
            font.pixelSize: Theme.fontSizeExtraSmall
            width: parent.width
            wrapMode: Text.WordWrap
        }

        Row {
            id: imageLinks
            spacing: 10

            ImageButton {
                id: browserIcon
                source: "icons/icon-m-browser.png"
                visible: website ? true : false
                onClicked: Qt.openUrlExternally(website)
            }

            ImageButton {
                id: twitterIcon
                source: "icons/icon-m-twitter.png"
                visible: twitter ? true : false
                onClicked: Qt.openUrlExternally("https://twitter.com/" + twitter)
            }

            ImageButton {
                id: githubIcon
                source: "icons/icon-m-github.png"
                visible: github ? true : false
                onClicked: Qt.openUrlExternally("https://github.com/" + github)
            }

            ImageButton {
                id: bitbucketIcon
                source: "icons/icon-m-bitbucket.png"
                visible: bitbucket ? true : false
                onClicked: Qt.openUrlExternally("https://bitbucket.org/" + bitbucket)
            }

            ImageButton {
                id: linkedinIcon
                source: "icons/icon-m-linkedin.png"
                visible: linkedin ? true : false
                onClicked: Qt.openUrlExternally("http://www.linkedin.com/profile/view?id=" + linkedin)
            }

            ImageButton {
                id: weiboIcon
                source: "icons/icon-m-weibo.png"
                visible: weibo ? true : false
                onClicked: Qt.openUrlExternally("http://www.weibo.com/" + weibo)
            }
        }
    }
}
