import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: about

    property string _RICHTEXT_STYLESHEET_PREAMBLE: "<html><style>a { text-decoration: none; color: '" + Theme.secondaryHighlightColor + "' }</style><body>";
    property string _RICHTEXT_STYLESHEET_APPENDIX: "</body></html>";

    SilicaFlickable {
        id: singleItem
        anchors.fill: parent
        contentHeight: imgCol.height + aboutCol.height + Theme.paddingLarge
        VerticalScrollDecorator {}

        PullDownMenu {
            MenuItem {
                text: qsTr("Homepage")
                onClicked: Qt.openUrlExternally("http://ocnews.buschmann23.de")
            }
            MenuItem {
                text: qsTr("Privacy Policy")
                onClicked: { var dialog = pageStack.push(Qt.resolvedUrl("../Dialogs/PrivacyPolicy.qml")) }
            }
            MenuItem {
                text: qsTr("Changelog")
                onClicked: pageStack.push(Qt.resolvedUrl("Changelog.qml"))
            }
            MenuItem {
                text: qsTr("Contributors")
                onClicked: pageStack.push(Qt.resolvedUrl("Contributors.qml"))
            }
        }

        Column {
            id: imgCol
            anchors { left: parent.left; right: parent.right }
            PageHeader { title: qsTr("About") }

            Image {
                visible: true
                width: parent.width
                smooth: true
                sourceSize.width: 540
                sourceSize.height: 270
                source: "/usr/share/harbour-ocnews-reader/icons/ocNews-Sailfish-Cover.jpg"
            }

        }


        Column {
            id: aboutCol
            anchors { left: parent.left; right: parent.right; leftMargin: Theme.paddingLarge; top: imgCol.bottom; topMargin: 10 }
            spacing: 5

            Label {
                id: labelName
                textFormat: Text.PlainText
                text: "ocNews " + versionString
                font.pixelSize: Theme.fontSizeLarge
                color: Theme.highlightColor
            }

            Text {
                id: description
                text: qsTr("The ownCloud News App client for Sailfish OS")
                width: parent.width
                wrapMode: Text.WordWrap
                textFormat: Text.PlainText
                color: Theme.secondaryHighlightColor
            }

            Text {
                id: copyright
                text: "© 2013-2014, Buschtrommel"
                width: parent.width
                wrapMode: Text.WordWrap
                textFormat: Text.PlainText
                color: Theme.primaryColor
            }

            Text {
                id: license
                text: _RICHTEXT_STYLESHEET_PREAMBLE + qsTr("Licensed under the <a href='http://www.gnu.org/licenses/gpl-2.0.en.html'>GNU GPL v2</a>") + _RICHTEXT_STYLESHEET_APPENDIX
                width: parent.width
                textFormat: Text.RichText
                color: Theme.primaryColor
                onLinkActivated: { Qt.openUrlExternally(link) }
            }

            SectionHeader { text: qsTr("Contribute") }

            Row {
                width: parent.width - Theme.paddingLarge

                Button {
                    width: parent.width/2
                    text: qsTr("Translate")
                    onClicked: Qt.openUrlExternally("https://www.transifex.com/projects/p/ocnews/")
                }

                Button {
                    width: parent.width/2
                    text: qsTr("Report bugs")
                    onClicked: Qt.openUrlExternally("https://github.com/Buschtrommel/ocNews/issues")
                }
            }

            Button {
                id: donationLink
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("Make a donation")
                onClicked: Qt.openUrlExternally("https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=RDZAG64WD34PL")
            }
        }

    }
}
