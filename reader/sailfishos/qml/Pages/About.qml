import QtQuick 2.0
import Sailfish.Silica 1.0
import "../BTComponents"
import "../Models"

AboutPage {

    appTitle: "ocNews"
    appVersion: versionString
    appDescription: qsTr("The ownCloud News App client for Sailfish OS")
    appHomepage: "http://ocnews.buschmann23.de"
    appCover: "/usr/share/harbour-ocnews-reader/icons/ocNews-Sailfish-Cover.jpg"
    appCopyright: "© 2013-2014, Buschtrommel"
    appLicense: qsTr("Licensed under the <a href='http://www.gnu.org/licenses/gpl-2.0.en.html'>GNU GPL v2</a>")

    changelogModel: ChangelogModel {}
    bugTrackerBaseUrl: "https://github.com/Buschtrommel/ocNews/issues/"

    contributorsAvatarBasePath: "/usr/share/harbour-ocnews-reader/icons/contributors"
    contributorsModel: ContributorsModel {}

    privacyPolicyQmlFile: "../Dialogs/PrivacyPolicy.qml"

    contactCompany: "Buschtrommel"
    contactName: "Matthias Fehring"
    contactStreet: "Engelhardstraße"
    contactHouseNo: "32"
    contactZIP: "81369"
    contactCity: qsTr("Munich")
    contactCountry: qsTr("Germany")
    contactEmail: "kontakt@buschmann23.de"
    contactWebsite: "www.buschmann23.de"

    paypalLabel: qsTr("Donate via PayPal")
    paypalEmail: "kontakt@buschmann23.de"
    paypalOrganization: "Buschtrommel"
    paypalItem: "ocNews"
    paypalMessage: qsTr("Leave a message (English or German):")

    bugUrl: "https://github.com/Buschtrommel/ocNews/issues"
    translateUrl: "https://www.transifex.com/projects/p/ocnews/"

    licensesModel: ListModel {
        ListElement { text: QT_TR_NOOP("<a href='https://github.com/pycage/tidings'>RescalingRichtext</a> by Martin Grimme is licensed under the terms of the <a href='http://www.gnu.org/licenses/gpl-2.0.en.html'>GNU GPL v2</a>.") }
        ListElement { text: QT_TR_NOOP("<a href='https://bitbucket.org/razvanpetru/qslog/'>QsLog</a> by by razvanpetru is licensed under the terms of the <a href='https://bitbucket.org/razvanpetru/qslog/src'>New BSD license</a>.") }
    }
}

//import QtQuick 2.0
//import Sailfish.Silica 1.0
//import "../BTComponents"

//Page {
//    id: about

//    property string _RICHTEXT_STYLESHEET_PREAMBLE: "<html><style>a { text-decoration: none; color: '" + Theme.secondaryHighlightColor + "' }</style><body>";
//    property string _RICHTEXT_STYLESHEET_APPENDIX: "</body></html>";

//    SilicaFlickable {
//        id: singleItem
//        anchors.fill: parent
//        contentHeight: imgCol.height + aboutCol.height + Theme.paddingLarge
//        VerticalScrollDecorator {}

//        PullDownMenu {
//            MenuItem {
//                text: qsTr("Homepage")
//                onClicked: Qt.openUrlExternally("http://ocnews.buschmann23.de")
//            }
//            MenuItem {
//                text: qsTr("Privacy Policy")
//                onClicked: { var dialog = pageStack.push(Qt.resolvedUrl("../Dialogs/PrivacyPolicy.qml")) }
//            }
//            MenuItem {
//                text: qsTr("Changelog")
//                onClicked: pageStack.push(Qt.resolvedUrl("../BTComponents/Changelog.qml"))
//            }
//            MenuItem {
//                text: qsTr("Contributors")
//                onClicked: pageStack.push(Qt.resolvedUrl("../BTComponents/Contributors.qml"), {avatarBasePath: "/usr/share/harbour-ocnews-reader/icons/contributors", iconBasePath: "/usr/share/harbour-ocnews-reader/icons"})
//            }
//        }

//        Column {
//            id: imgCol
//            anchors { left: parent.left; right: parent.right }
//            PageHeader { title: qsTr("About") }

//            Image {
//                visible: true
//                width: parent.width
//                smooth: true
//                sourceSize.width: 540
//                sourceSize.height: 270
//                source: "/usr/share/harbour-ocnews-reader/icons/ocNews-Sailfish-Cover.jpg"
//            }

//        }


//        Column {
//            id: aboutCol
//            anchors { left: parent.left; right: parent.right; leftMargin: Theme.paddingLarge; top: imgCol.bottom; topMargin: 10 }
//            spacing: 5

//            Label {
//                id: labelName
//                textFormat: Text.PlainText
//                text: "ocNews " + versionString
//                font.pixelSize: Theme.fontSizeLarge
//                color: Theme.highlightColor
//            }

//            Text {
//                id: description
//                text: qsTr("The ownCloud News App client for Sailfish OS")
//                width: parent.width
//                wrapMode: Text.WordWrap
//                textFormat: Text.PlainText
//                color: Theme.secondaryHighlightColor
//            }

//            Text {
//                id: copyright
//                text: "© 2013-2014, Buschtrommel"
//                width: parent.width
//                wrapMode: Text.WordWrap
//                textFormat: Text.PlainText
//                color: Theme.primaryColor
//            }

//            Text {
//                id: license
//                text: _RICHTEXT_STYLESHEET_PREAMBLE + qsTr("Licensed under the <a href='http://www.gnu.org/licenses/gpl-2.0.en.html'>GNU GPL v2</a>") + _RICHTEXT_STYLESHEET_APPENDIX
//                width: parent.width
//                textFormat: Text.RichText
//                color: Theme.primaryColor
//                onLinkActivated: { Qt.openUrlExternally(link) }
//            }

//            SectionHeader { text: qsTr("Contribute") }

//            Row {
//                width: parent.width - Theme.paddingLarge

//                Button {
//                    width: parent.width/2
//                    text: qsTr("Translate")
//                    onClicked: Qt.openUrlExternally("https://www.transifex.com/projects/p/ocnews/")
//                }

//                Button {
//                    width: parent.width/2
//                    text: qsTr("Report bugs")
//                    onClicked: Qt.openUrlExternally("https://github.com/Buschtrommel/ocNews/issues")
//                }
//            }

//            PaypalChooser {
//                id: donation
//                anchors { left: parent.left; leftMargin: -Theme.paddingLarge; right: parent.right }
//                organization: "Buschtrommel"
//                item: "ocNews"
//                email: "kontakt@buschmann23.de"
//                message: qsTr("Leave a message (English or German):")
//            }
//        }
//    }
//}
