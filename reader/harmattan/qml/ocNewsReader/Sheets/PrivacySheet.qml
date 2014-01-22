import QtQuick 1.1
import com.nokia.meego 1.0

Sheet {
    property bool policy

    property string _RICHTEXT_STYLESHEET_PREAMBLE: "<html><style>a { text-decoration: none; color: '" + theme.selectionColor + "' }</style><body>";
    property string _RICHTEXT_STYLESHEET_APPENDIX: "</body></html>";

    acceptButtonText: policy ? "" : qsTr("Accept")
    rejectButtonText: policy ? qsTr("Close") : qsTr("Reject")

    content: Flickable {
            anchors { fill: parent; leftMargin: 10; topMargin: 10; rightMargin: 10 }
            contentHeight: policyHeader.height + policyHeaderLine.height + label1.height + text1.height + label2.height + text2.height + label3.height + text3.height + label4.height + text4.height + label5.height + text5.height + label6.height + text6.height + label7.height + text7.height + 200
            flickableDirection: Flickable.VerticalFlick
            Label {
                id: policyHeader
                text: qsTr("Privacy Policy")
                anchors { top: parent.top; leftMargin: 5 }
                platformStyle: LabelStyle { fontPixelSize: 30 }
                width: parent.width
                textFormat: Text.PlainText
            }

            Rectangle {
                id: policyHeaderLine
                height: 1
                width: parent.width
                color: "white"
                anchors { top: policyHeader.bottom; topMargin: 10 }
            }

            Label {
                id: label1
                text: qsTr("What information does ocNews collect?")
                anchors { top: policyHeaderLine.bottom; topMargin: 10 }
                textFormat: Text.PlainText
                width: parent.width
                wrapMode: Text.WordWrap
            }

            Text {
                id: text1
                text: qsTr("In order to log in to your ownCloud server, ocNews collects the server address, your user name and your password. These data is stored in a protected/private folder on your device. The data is not given to the author of ocNews or other third parties.")
                anchors { top: label1.bottom; topMargin: 10 }
                textFormat: Text.PlainText
                font.weight: Font.Light
                width: parent.width
                wrapMode: Text.WordWrap
                color: theme.inverted ? "white" : "black"
                font.pointSize: 15
            }

            Label {
                id: label2
                text: qsTr("What does ocNews use your information for?")
                anchors { top: text1.bottom; topMargin: 10 }
                textFormat: Text.PlainText
                width: parent.width
                wrapMode: Text.WordWrap
            }

            Text {
                id: text2
                text: qsTr("ocNews needs your log in data to connect to your ownCloud server. Your log in data is used to log in to your ownCloud server to perform the tasks of synchronizing your ownCloud News App data with your device.")
                anchors { top: label2.bottom; topMargin: 10 }
                textFormat: Text.PlainText
                width: parent.width
                wrapMode: Text.WordWrap
                color: theme.inverted ? "white" : "black"
                font.pointSize: 15
                font.weight: Font.Light
            }

            Label {
                id: label3
                text: qsTr("How does ocNews protects your data?")
                anchors { top: text2.bottom; topMargin: 10 }
                textFormat: Text.PlainText
                width: parent.width
                wrapMode: Text.WordWrap
            }

            Text {
                id: text3
                text: qsTr("Your data is stored in the protected private folder in your Home directory. Only ocNews has the rights to access these data.")
                anchors { top: label3.bottom; topMargin: 10 }
                textFormat: Text.PlainText
                width: parent.width
                wrapMode: Text.WordWrap
                color: theme.inverted ? "white" : "black"
                font.pointSize: 15
                font.weight: Font.Light
            }

            Label {
                id: label4
                text: qsTr("Does ocNews disclose any information to outside parties?")
                anchors { top: text3.bottom; topMargin: 10 }
                textFormat: Text.PlainText
                width: parent.width
                wrapMode: Text.WordWrap
            }

            Text {
                id: text4
                text: qsTr("ocNews only uses your data for the log in process into the server that you configure in the settings. ocNews does not send these data to the author of ocNews or other third parties. For questions concerning the privacy on the server side, you have to contact your ownCloud service provider.")
                anchors { top: label4.bottom; topMargin: 10 }
                textFormat: Text.PlainText
                width: parent.width
                wrapMode: Text.WordWrap
                color: theme.inverted ? "white" : "black"
                font.pointSize: 15
                font.weight: Font.Light
            }

            Label {
                id: label5
                text: qsTr("Third party links")
                anchors { top: text4.bottom; topMargin: 10 }
                textFormat: Text.PlainText
                width: parent.width
                wrapMode: Text.WordWrap
            }

            Text {
                id: text5
                text: qsTr("ocNews is a cloud based RSS/Atom feed reader. It has no preinstalled third party links other than the own application website link on the About page. Showing feed content and/or opening feed link addresses is in the user's responsibility. For questions concerning the privacy of the used feed content and/or website content you have to contact the feed provider.")
                anchors { top: label5.bottom; topMargin: 10 }
                textFormat: Text.PlainText
                width: parent.width
                wrapMode: Text.WordWrap
                color: theme.inverted ? "white" : "black"
                font.pointSize: 15
                font.weight: Font.Light
            }

            Label {
                id: label6
                text: qsTr("Your consent")
                anchors { top: text5.bottom; topMargin: 10 }
                textFormat: Text.PlainText
                width: parent.width
                wrapMode: Text.WordWrap
            }

            Text {
                id: text6
                text: qsTr("By using ocNews, you consent to this privacy policy.")
                anchors { top: label6.bottom; topMargin: 10 }
                textFormat: Text.PlainText
                width: parent.width
                wrapMode: Text.WordWrap
                color: theme.inverted ? "white" : "black"
                font.pointSize: 15
                font.weight: Font.Light
            }

            Label {
                id: label7
                text: qsTr("Contact")
                anchors { top: text6.bottom; topMargin: 10 }
                textFormat: Text.PlainText
                width: parent.width
                wrapMode: Text.WordWrap
            }

            Text {
                id: text7
                text: _RICHTEXT_STYLESHEET_PREAMBLE + "<a href='http://ocnews.buschmann23.de'>ocnews.buschmann23.de</a>" + _RICHTEXT_STYLESHEET_APPENDIX
                anchors { top: label7.bottom; topMargin: 10 }
                textFormat: Text.RichText
                width: parent.width
                wrapMode: Text.WordWrap
                font.pointSize: 15
                font.weight: Font.Light
                onLinkActivated: { Qt.openUrlExternally(link) }
            }
    }

    onAccepted: dbus.setSetting("display/privacypolicy", true);
    onRejected: policy ? "" : Qt.quit();
}
