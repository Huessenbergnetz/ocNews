import QtQuick 1.1
import com.nokia.meego 1.0

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
        source: image ? "images/" + image : "image://theme/icon-l-content-avatar-placeholder"
        sourceSize.height: image ? 86 : 80
        sourceSize.width: image ? 86 : 80
        smooth: image ? false : true
        width: 86
        height: 86
    }

    Column {
        spacing: 1

        Label {
            id: contribName
        }

        Label {
            id: contribRole
        }

        Row {
            id: imageLinks
            spacing: 10

            Image {
                id: browserIcon
                source: "image://theme/icon-l-browser"
                sourceSize.height: 80
                sourceSize.width: 80
                width: 48
                height: 48
                smooth: true
                visible: website ? true : false
                MouseArea {
                    anchors.fill: parent
                    onClicked: Qt.openUrlExternally(website)
                }
            }

            Image {
                id: twitterIcon
                source: "image://theme/icon-l-twitter"
                sourceSize.height: 80
                sourceSize.width: 80
                height: 48
                width: 48
                smooth: true
                visible: twitter ? true : false
                MouseArea {
                    anchors.fill: parent
                    onClicked: Qt.openUrlExternally("https://twitter.com/" + twitter)
                }
            }

            Image {
                id: githubIcon
                source: "icons/github.png"
                sourceSize.height: 50
                sourceSize.width: 50
                height: 48
                width: 48
                smooth: true
                visible: github ? true : false
                MouseArea {
                    anchors.fill: parent
                    onClicked: Qt.openUrlExternally("https://github.com/" + github)
                }
            }

            Image {
                id: linkedinIcon
                source: "icons/linkedin.png"
                sourceSize.height: 50
                sourceSize.width: 50
                height: 48
                width: 48
                smooth: true
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
                sourceSize.width: 48
                height: 48
                width: 48
                visible: weibo ? true : false
                MouseArea {
                    anchors.fill: parent
                    onClicked: Qt.openUrlExternally("http://www.weibo.com/" + weibo)
                }
            }

        }
    }
}
