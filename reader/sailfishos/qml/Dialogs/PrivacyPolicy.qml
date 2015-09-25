import QtQuick 2.2
import Sailfish.Silica 1.0

Dialog {
    id: privacyPolicy

    property string _RICHTEXT_STYLESHEET_PREAMBLE: "<html><style>a { text-decoration: none; color: '" + Theme.secondaryHighlightColor + "' }</style><body>";
    property string _RICHTEXT_STYLESHEET_APPENDIX: "</body></html>";

    acceptDestinationAction: PageStackAction.Pop

    SilicaFlickable {
        id: policyFlick
        anchors.fill: parent
        contentHeight: policyCol.height + Theme.paddingLarge + dHeader.height

        DialogHeader { id: dHeader; acceptText: config.privacyShown ? qsTr("Close") : qsTr("Accept") }

        Column {
            id: policyCol
            anchors { left: parent.left; right: parent.right; top: dHeader.bottom; }

            Label {
                id: policyHeader
                text: qsTr("Privacy Policy")
                anchors { left: parent.left; leftMargin: Theme.paddingLarge; right: parent.right; rightMargin: Theme.paddingLarge }
                textFormat: Text.PlainText
                font.pixelSize: Theme.fontSizeLarge
                wrapMode: Text.WordWrap
            }

            Label {
                text: qsTr("What information does ocNews collect?")
                anchors { left: parent.left; leftMargin: Theme.paddingLarge; right: parent.right; rightMargin: Theme.paddingLarge }
                color: Theme.highlightColor
                wrapMode: Text.WordWrap
            }


            Text {
                id: text1
                text: qsTr("In order to log in to your ownCloud server, ocNews collects the server address, your user name and your password. These information is not given to the author of ocNews or other third parties.")
                anchors { left: parent.left; leftMargin: Theme.paddingLarge; right: parent.right; rightMargin: Theme.paddingLarge }
                textFormat: Text.PlainText
                wrapMode: Text.WordWrap
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeSmall
                font.weight: Font.Light
            }

            Label {
                text: qsTr("What does ocNews use your information for?")
                anchors { left: parent.left; leftMargin: Theme.paddingLarge; right: parent.right; rightMargin: Theme.paddingLarge }
                color: Theme.highlightColor
                wrapMode: Text.WordWrap
            }

            Text {
                id: text2
                text: qsTr("ocNews needs your log in data to connect to your ownCloud server. Your log in data is used to log in to your ownCloud server to perform the tasks of synchronizing your ownCloud News App data with your device.")
                anchors { left: parent.left; leftMargin: Theme.paddingLarge; right: parent.right; rightMargin: Theme.paddingLarge }
                textFormat: Text.PlainText
                wrapMode: Text.WordWrap
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeSmall
                font.weight: Font.Light
            }

            Label {
                text: qsTr("Does ocNews disclose any information to outside parties?")
                anchors { left: parent.left; leftMargin: Theme.paddingLarge; right: parent.right; rightMargin: Theme.paddingLarge }
                color: Theme.highlightColor
                wrapMode: Text.WordWrap
            }

            Text {
                id: text4
                text: qsTr("ocNews only uses your data for the log in process into the server that you configure in the settings. ocNews does not send these data to the author of ocNews or other third parties. For questions concerning the privacy on the server side, you have to contact your ownCloud service provider.")
                anchors { left: parent.left; leftMargin: Theme.paddingLarge; right: parent.right; rightMargin: Theme.paddingLarge }
                textFormat: Text.PlainText
                wrapMode: Text.WordWrap
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeSmall
                font.weight: Font.Light
            }

            Label {
                text: qsTr("Third party links")
                anchors { left: parent.left; leftMargin: Theme.paddingLarge; right: parent.right; rightMargin: Theme.paddingLarge }
                color: Theme.highlightColor
                wrapMode: Text.WordWrap
            }

            Text {
                id: text5
                text: qsTr("ocNews is a cloud based RSS/Atom feed reader. It has no preinstalled third party links other than the own application website links on the About page and on this page. Showing feed content and/or opening feed link addresses is in the user's responsibility. For questions concerning the privacy of the used feed content and/or website content you have to contact the feed provider.")
                anchors { left: parent.left; leftMargin: Theme.paddingLarge; right: parent.right; rightMargin: Theme.paddingLarge }
                textFormat: Text.PlainText
                wrapMode: Text.WordWrap
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeSmall
                font.weight: Font.Light
            }

            Label {
                text: qsTr("Your consent")
                anchors { left: parent.left; leftMargin: Theme.paddingLarge; right: parent.right; rightMargin: Theme.paddingLarge }
                color: Theme.highlightColor
                wrapMode: Text.WordWrap
            }

            Text {
                id: text6
                text: qsTr("By using ocNews, you consent to this privacy policy.")
                anchors { left: parent.left; leftMargin: Theme.paddingLarge; right: parent.right; rightMargin: Theme.paddingLarge }
                textFormat: Text.PlainText
                wrapMode: Text.WordWrap
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeSmall
                font.weight: Font.Light
            }

            Label {
                text: qsTr("Contact")
                anchors { left: parent.left; leftMargin: Theme.paddingLarge; right: parent.right; rightMargin: Theme.paddingLarge }
                color: Theme.highlightColor
                wrapMode: Text.WordWrap
            }

            Text {
                id: text7
                text: _RICHTEXT_STYLESHEET_PREAMBLE + "<a href='http://ocnews.buschmann23.de'>http://ocnews.buschmann23.de</a>" + _RICHTEXT_STYLESHEET_APPENDIX
                anchors { left: parent.left; leftMargin: Theme.paddingLarge; right: parent.right; rightMargin: Theme.paddingLarge }
                textFormat: Text.RichText
                wrapMode: Text.WordWrap
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeSmall
                font.weight: Font.Light
                onLinkActivated: { Qt.openUrlExternally(link) }
            }

            VerticalScrollDecorator {}
        }
    }

    onAccepted: config.privacyShown = true
    onRejected: config.privacyShown ? null : Qt.quit();
}
