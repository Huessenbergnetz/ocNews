// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.0

import "../Common"

Sheet {
    id: root

    property string version
    property string name
    property string description
    property string helpPage
    property string paypalOrganization
    property string paypalItem
    property string paypalEmail
    property string paypalMessage

    property string _RICHTEXT_STYLESHEET_PREAMBLE: "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\"><style>a { text-decoration: none; color: '" + theme.selectionColor + "' }</style></head><body>";
    property string _RICHTEXT_STYLESHEET_APPENDIX: "</body></html>";


//    acceptButtonText: helpPage ? qsTr("Help") : ""
//    rejectButtonText: qsTr("Close")

    content: Flickable {
        id: infoFlick
        anchors.fill: parent
        contentHeight: infoCol.height + 70

        Column {
            id: infoCol
            anchors { left: parent.left; right: parent.right; top: parent.top; topMargin: 20 }

            ChangelogModel {
                id: changelogModel
            }

            Label {
                id: title
                anchors { left: parent.left; right: parent.right; leftMargin: 20; rightMargin:20 }
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                text: qsTr("Welcome to %1 %2", "First is app name, second is version number").arg(root.name).arg(root.version)
                font.pointSize: 32
            }

            Text {
                id: description
                anchors { left: parent.left; right: parent.right; leftMargin: 20; rightMargin: 20 }
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                textFormat: Text.RichText
                color: theme.inverted ? "white" : "black"
                font.pointSize: 17
                font.weight: Font.Light
                visible: root.description !== ""
                text: _RICHTEXT_STYLESHEET_PREAMBLE + root.description + _RICHTEXT_STYLESHEET_APPENDIX
            }


            GroupHeader { text: qsTr("Last changes"); visible: newInThisVersion.visible; anchors.leftMargin: 20 }


            Text {
                id: newInThisVersion
                anchors { left: parent.left; right: parent.right; rightMargin: 20 }
                textFormat: Text.RichText
                wrapMode: Text.WordWrap
                color: theme.inverted ? "white" : "black"
                font.pointSize: 17
                font.weight: Font.Light
                onLinkActivated: { Qt.openUrlExternally(link) }
                text: changelogModel.count > 0 ? _RICHTEXT_STYLESHEET_PREAMBLE + changelogModel.get(0).text + _RICHTEXT_STYLESHEET_APPENDIX : ""
                visible: changelogModel.get(0).text !== ""
            }

            GroupHeader { text: qsTr("Donate"); visible: root.paypalEmail; anchors.leftMargin: 20 }

            Text {
                anchors { left: parent.left; leftMargin: 20; right: parent.right; rightMargin: 20 }
                textFormat: Text.PlainText
                color: theme.inverted ? "white" : "black"
                font.pointSize: 17
                font.weight: Font.Light
                wrapMode: Text.WordWrap
                onLinkActivated: { Qt.openUrlExternally(link) }
                text: qsTr("If you like ocNews and have not purchased it alread via Ovi Store and you want to support the development, consider a small donation.")
                visible: root.paypalEmail
            }

            PayPalChooser {
                id: donation
                organization: root.paypalOrganization
                item: root.paypalItem
                email: root.paypalEmail
                message: root.paypalMessage
                visible: root.paypalEmail
            }

            Button {
                text: qsTr("Help")
                visible: root.helpPage !== ""
                onClicked: root.accept()
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Button {
                text: qsTr("Close")
                onClicked: root.reject()
                anchors.horizontalCenter: parent.horizontalCenter
            }

            ScrollDecorator {}
        }
    }



    function openFile(file, properties) {
             var component = Qt.createComponent(file)
             if (component.status == Component.Ready)
                 pageStack.push(component, properties);
             else
                 console.log("Error loading component:", component.errorString());
         }

    onAccepted: { config.displayedVersion = versionInt; openFile(helpPage) }
    onRejected: config.displayedVersion = versionInt
}

