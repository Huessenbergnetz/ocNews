import QtQuick 1.1
import com.nokia.meego 1.0

import "../Common"
import "../Sheets"

Page {
    id: about

    anchors.fill: parent

    tools: aboutTabs

    orientationLock: PageOrientation.LockPortrait

    property string _RICHTEXT_STYLESHEET_PREAMBLE: "<html><style>a { text-decoration: none; color: '" + theme.selectionColor + "' }</style><body>";
    property string _RICHTEXT_STYLESHEET_APPENDIX: "</body></html>";


    // ------------- Header Start ----------------

    Header {
        id: aboutPageHeader
        text: qsTr("About")
    }

    // ------------- Header End ----------------

    TabGroup {
        id: tabgroup
        currentTab: aboutTab
        anchors { top: aboutPageHeader.bottom; left: parent.left; right: parent.right }

        Page {
            id: aboutTab
            orientationLock: PageOrientation.LockPortrait

            Flickable {
                id: aboutFlick
                anchors.fill: parent
                contentHeight: coverImage.height + firstAboutCol.height + 100


                Image {
                    id: coverImage
                    anchors { top: parent.top; left: parent.left; right: parent.right }
                    asynchronous: true
                    sourceSize.width: 540
                    sourceSize.height: 270
                    width: parent.width
                    height: width/2
                    source: "/opt/ocNewsReader/images/ocNews-Harmattan-Cover.jpg"
                }

                Column {
                    id: firstAboutCol
                    anchors { top: coverImage.bottom; topMargin: 5; left: parent.left; leftMargin: 20; right: parent.right; rightMargin: 20 }
                    spacing: 10

                    Label {
                        id: labelName
                        width: parent.width
                        textFormat: Text.PlainText
                        text: "ocNews " + versionString
                        font.pointSize: 32
                    }

                    Text {
                        id: description
                        text: qsTr("The ownCloud News App client for Harmattan")
                        width: parent.width
                        font.pointSize: 17
                        font.weight: Font.Light
                        wrapMode: Text.WordWrap
                        textFormat: Text.PlainText
                        color: theme.inverted ? "white" : "black"
                    }

                    Text {
                        id: copyright
                        width: parent.width
                        font.pointSize: 17
                        font.weight: Font.Light
                        text: "© 2013-2014, Buschtrommel"
                        textFormat: Text.PlainText
                        color: theme.inverted ? "white" : "black"
                    }

                    Text {
                        id: license
                        width: parent.width
                        font.pointSize: 17
                        font.weight: Font.Light
                        text: _RICHTEXT_STYLESHEET_PREAMBLE + qsTr("Licensed under the <a href='http://www.gnu.org/licenses/gpl-2.0.en.html'>GNU GPL v2</a>") + _RICHTEXT_STYLESHEET_APPENDIX
                        textFormat: Text.RichText
                        color: theme.inverted ? "white" : "black"
                        onLinkActivated: { Qt.openUrlExternally(link) }
                    }

                    GroupHeader { text: qsTr("Contribute") }

                    PayPalChooser {
                        id: donation
                        width: parent.width + 40
                        organization: "Buschtrommel"
                        item: "ocNews"
                        email: "kontakt@buschmann23.de"
                        message: qsTr("Leave a message (English or German):")
                    }

                    Button {
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: qsTr("Translate")
                        onClicked: Qt.openUrlExternally("https://www.transifex.com/projects/p/ocnews/")
                    }

                    Button {
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: qsTr("Report bugs")
                        onClicked: Qt.openUrlExternally("https://github.com/Buschtrommel/ocNews/issues")
                    }

                    GroupHeader { text: qsTr("Further information") }

                    Button {
                        id: privacyPolicyButton
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: qsTr("Privacy policy")
                        onClicked: {
                            aboutPrivacySheet.policy = config.privacyShown
                            aboutPrivacySheet.open();
                        }
                    }

                    Button {
                        id: websiteLink
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: qsTr("Visit website")
                        onClicked: Qt.openUrlExternally("http://ocnews.buschmann23.de")
                    }

                    GroupHeader { text: qsTr("Third party licenses") }

                    Text {
                        id: rescalingRichtextLicense
                        width: parent.width
                        font.pointSize: 17
                        font.weight: Font.Light
                        text: _RICHTEXT_STYLESHEET_PREAMBLE + qsTr("<a href='https://github.com/pycage/tidings'>RescalingRichtext</a> by Martin Grimme is licensed under the terms of the <a href='http://www.gnu.org/licenses/gpl-2.0.en.html'>GNU GPL v2</a>.") + _RICHTEXT_STYLESHEET_APPENDIX
                        textFormat: Text.RichText
                        color: theme.inverted ? "white" : "black"
                        wrapMode: Text.WordWrap
                        onLinkActivated: { Qt.openUrlExternally(link) }
                    }

                    Text {
                        id: qsLogLicense
                        width: parent.width
                        font.pointSize: 17
                        font.weight: Font.Light
                        text: _RICHTEXT_STYLESHEET_PREAMBLE + qsTr("<a href='https://bitbucket.org/razvanpetru/qslog/'>QsLog</a> by razvanpetru is licensed under the terms of the <a href='https://bitbucket.org/razvanpetru/qslog/src'>New BSD license</a>.") + _RICHTEXT_STYLESHEET_APPENDIX
                        textFormat: Text.RichText
                        color: theme.inverted ? "white" : "black"
                        wrapMode: Text.WordWrap
                        onLinkActivated: { Qt.openUrlExternally(link) }
                    }
                }
            }

            ScrollDecorator { flickableItem: aboutFlick }
        }

        Page {
            id: contributorsTab
            orientationLock: PageOrientation.LockPortrait

            Flickable {
                id: contributorsContent
                anchors.fill: parent
                contentHeight: contentCol.height + aboutTabs.height + 50
                flickableDirection:  Flickable.VerticalFlick


                Column {
                    id: contentCol
                    spacing: 10
                    anchors { right: parent.right; rightMargin: 15; left: parent.left; leftMargin: 15 }

                    GroupHeader {
                        text: qsTr("Upstream author")
                    }

                    ContributorItem {
                        name: "Buschmann (Matthias Fehring)"
                        role: qsTr("Main developer, ocNews creator")
                        image: "buschmann.png"
                        website: "http://www.buschmann23.de"
                        twitter: "buschmann23"
                        github: "buschmann23"
                    }

                    GroupHeader {
                        text: qsTr("Major contributors")
                    }

                    ContributorItem {
                        name: "Tigre-Bleu (Antoine Vacher)"
                        role: qsTr("UI, French translation")
                        image: "tigre-bleu.png"
                        twitter: "tigre_bleu"
                    }

                    GroupHeader {
                        text: qsTr("Thanks to")
                    }

                    ContributorItem {
                        name: "Finalmix (太空飞瓜)"
                        role: qsTr("Chinese (China) translation")
                        image: "finalmix.png"
                        weibo: "finalmix"
                    }

                    ContributorItem {
                        name: "Flywheeldk (Peter Jespersen)"
                        role: qsTr("Danish translation")
                        image: "flywheeldk.png"
                        website: "http://illogical.dk/"
                        twitter: "myflywheel"
                        linkedin: "pjespersen"
                        bitbucket: "flywheel"
                    }

                    ContributorItem {
                        name: "gwmgdemj"
                        role: qsTr("Chinese (China) translation")
                    }

                    ContributorItem {
                        name: "Heimen Stoffels"
                        role: qsTr("Dutch (Netherlands) translation")
                    }

                    ContributorItem {
                        name: "Jean Clovis"
                        role: qsTr("French translation")
                        github: "jean-clovis"
                        image: "jean-clovis.png"
                    }

                    ContributorItem {
                        name: "lewa (Alexey)"
                        role: qsTr("Russian translation")
                    }

                    ContributorItem {
                        name: "Lorenzo Facca"
                        role: qsTr("Italian translation")
                        twitter: "LorenzoFacca"
                        website: "http://jollacommunity.altervista.org/"
                    }

                    ContributorItem {
                        name: "Mariano Flotron"
                        role: qsTr("Spanish translation")
                    }

                    ContributorItem {
                        name: "Palacex (Lutsenko Denis)"
                        role: qsTr("Russian translation")
                    }

                    GroupHeader {
                        text: qsTr("Used third party elements")
                    }

                    ContributorItem {
                        name: qsTr("Martin Grimme's Tidings Elements")
                        role: "RescalingRichText"
                        website: "https://github.com/pycage/tidings"
                        github: "pycage"
                    }
                }
            }

            ScrollDecorator {
                flickableItem: contributorsContent
            }
        }

        Page {
            id: changelogTab
            orientationLock: PageOrientation.LockPortrait


            ListView {
                id: changelogContent
                anchors.fill: parent
                anchors.bottomMargin: aboutTabs.height
                model: ChangelogModel {}

                section {
                    property: 'version'
                    delegate: GroupHeader { text: qsTr("Version") + " " + section }
                }

                delegate: Item {
                    width: parent.width
                    height: col.height

                    Column {
                        id: col
                        anchors { left: parent.left; right: parent.right; rightMargin: 20 }

                        Text {
                            id: date
                            textFormat: Text.PlainText
//                            width: parent.width
                            anchors { left: parent.left; leftMargin: 20; right: parent.right }
                            wrapMode: Text.WordWrap
                            color: theme.inverted ? "white" : "black"
                            font.pointSize: 17
                            text: Qt.formatDateTime(new Date(model.time), Qt.DefaultLocaleShortDate)
                        }

                        Text {
                            id: text
                            width: parent.width
                            textFormat: Text.RichText
                            wrapMode: Text.WordWrap
                            font.pointSize: 17
                            font.weight: Font.Light
                            color: theme.inverted ? "white" : "black"
                            onLinkActivated: { Qt.openUrlExternally(link) }
                            text: _RICHTEXT_STYLESHEET_PREAMBLE + model.text + _RICHTEXT_STYLESHEET_APPENDIX
                        }
                    }
                }
            }

            ScrollDecorator {
                flickableItem: changelogContent
            }

        }
    }


    // ----------------- Toolbar Start --------------------

    ToolBarLayout {
        id: aboutTabs

        ToolIcon {
            platformIconId: "toolbar-back"
            anchors.left: (parent === undefined) ? undefined : parent.left
            onClicked: { pageStack.pop(); }
        }
        ButtonRow {
            style: TabButtonStyle { }
            TabButton {
                iconSource: "image://theme/icon-m-toolbar-application"
                tab: aboutTab
                onClicked: aboutPageHeader.text = qsTr("About")
            }
            TabButton {
                iconSource: "image://theme/icon-m-toolbar-contact"
                tab: contributorsTab
                onClicked: aboutPageHeader.text = qsTr("Contributors")
            }
            TabButton {
                iconSource: "image://theme/icon-m-toolbar-list"
                tab: changelogTab
                onClicked: aboutPageHeader.text = qsTr("Changelog")
            }
        }
    }
    // ----------------- Toolbar End --------------------


    PrivacySheet {
        id: aboutPrivacySheet
    }

}
