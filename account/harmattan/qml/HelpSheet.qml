import QtQuick 1.1
import com.nokia.meego 1.0

Sheet {
    rejectButtonText: qsTrId("qtn_help_close")

    content: Flickable {
            anchors { fill: parent; leftMargin: 10; topMargin: 10; rightMargin: 10 }
            contentHeight: contentCol.height
            flickableDirection: Flickable.VerticalFlick

            Column {
                id: contentCol
                spacing: 5
                width: parent.width

                Label {
                    text: qsTrId("qtn_help_username_label")
                    textFormat: Text.PlainText
                    width: parent.width
                    wrapMode: Text.WordWrap
                }

                Text {
                    text: qsTrId("qtn_help_username_text")
                    textFormat: Text.PlainText
                    font.weight: Font.Light
                    width: parent.width
                    wrapMode: Text.WordWrap
                    color: theme.inverted ? "white" : "black"
                    font.pointSize: 15
                }

                Label {
                    text: qsTrId("qtn_help_password_label")
                    textFormat: Text.PlainText
                    width: parent.width
                    wrapMode: Text.WordWrap
                }

                Text {
                    text: qsTrId("qtn_help_password_text")
                    textFormat: Text.PlainText
                    font.weight: Font.Light
                    width: parent.width
                    wrapMode: Text.WordWrap
                    color: theme.inverted ? "white" : "black"
                    font.pointSize: 15
                }

                Label {
                    text: qsTrId("qtn_help_server_label")
                    textFormat: Text.PlainText
                    width: parent.width
                    wrapMode: Text.WordWrap
                }

                Text {
                    text: qsTrId("qtn_help_server_text")
                    textFormat: Text.PlainText
                    font.weight: Font.Light
                    width: parent.width
                    wrapMode: Text.WordWrap
                    color: theme.inverted ? "white" : "black"
                    font.pointSize: 15
                }

                Label {
                    text: qsTrId("qtn_help_ssl_label")
                    textFormat: Text.PlainText
                    width: parent.width
                    wrapMode: Text.WordWrap
                }

                Text {
                    text: qsTrId("qtn_help_ssl_text")
                    textFormat: Text.PlainText
                    font.weight: Font.Light
                    width: parent.width
                    wrapMode: Text.WordWrap
                    color: theme.inverted ? "white" : "black"
                    font.pointSize: 15
                }

                Label {
                    text: qsTrId("qtn_help_ignoressl_label")
                    textFormat: Text.PlainText
                    width: parent.width
                    wrapMode: Text.WordWrap
                }

                Text {
                    text: qsTrId("qtn_help_ignoressl_text")
                    textFormat: Text.PlainText
                    font.weight: Font.Light
                    width: parent.width
                    wrapMode: Text.WordWrap
                    color: theme.inverted ? "white" : "black"
                    font.pointSize: 15
                }
            }
    }
}
