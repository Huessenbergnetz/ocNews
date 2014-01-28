import QtQuick 2.0
import Sailfish.Silica 1.0

Row {
    spacing: 10

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
        sourceSize.height: 86
        sourceSize.width: 86
        width: 86
        height: 86
    }

    Column {
        spacing: 1

        Label {
            id: contribName
            font.pixelSize: Theme.fontSizeSmall
        }

        Label {
            id: contribRole
            font.pixelSize: Theme.fontSizeExtraSmall
        }

        Row {
            id: imageLinks
            spacing: 10

            Image {
                id: browserIcon
                source: "icons/browser.png"
                sourceSize.height: 48
                sourceSize.width: 48
                width: 48
                height: 48
                visible: website ? true : false
                MouseArea {
                    anchors.fill: parent
                    onClicked: Qt.openUrlExternally(website)
                }
            }

            Image {
                id: twitterIcon
                source: "icons/twitter.png"
                sourceSize.height: 48
                sourceSize.width: 59
                height: 48
                width: 59
                visible: twitter ? true : false
                MouseArea {
                    anchors.fill: parent
                    onClicked: Qt.openUrlExternally("https://twitter.com/" + twitter)
                }
            }

            Image {
                id: githubIcon
                source: "icons/github.png"
                sourceSize.height: 48
                sourceSize.width: 48
                height: 48
                width: 48
                visible: github ? true : false
                MouseArea {
                    anchors.fill: parent
                    onClicked: Qt.openUrlExternally("https://github.com/" + github)
                }
            }

            Image {
                id: linkedinIcon
                source: "icons/linkedin.png"
                sourceSize.height: 48
                sourceSize.width: 48
                height: 48
                width: 48
                visible: linkedin ? true : false
                MouseArea {
                    anchors.fill: parent
                    onClicked: Qt.openUrlExternally("http://www.linkedin.com/profile/view?id=" + linkedin)
                }
            }

            Image {
                id: weiboIcon
                source: "icons/weibo.png"
                sourceSize.height: 48
                sourceSize.width: 60
                height: 48
                width: 60
                visible: weibo ? true : false
                MouseArea {
                    anchors.fill: parent
                    onClicked: Qt.openUrlExternally("http://www.weibo.com/" + weibo)
                }
            }

        }
    }
}
