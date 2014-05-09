import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: about

    property string _RICHTEXT_STYLESHEET_PREAMBLE: "<html><style>a { text-decoration: none; color: '" + Theme.secondaryHighlightColor + "' }</style><body>";
    property string _RICHTEXT_STYLESHEET_APPENDIX: "</body></html>";

    SilicaFlickable {
        id: singleItem
        anchors.fill: parent
        contentHeight: aboutCol.height + Theme.paddingLarge
        VerticalScrollDecorator {}

        Column {
            id: aboutCol
            anchors { left: parent.left; leftMargin: Theme.paddingLarge; right: parent.right }
            spacing: 10
            PageHeader { title: qsTr("Contributors") }

            SectionHeader { text: qsTr("Upstream author") }

            ContributorItem {
                name: "Buschmann (Matthias Fehring)"
                role: qsTr("Main developer, ocNews creator")
                image: "buschmann.png"
                website: "http://www.buschmann23.de"
                twitter: "buschmann23"
                github: "buschmann23"
            }

            SectionHeader { text: qsTr("Major contributors") }

            ContributorItem {
                name: "Tigre-Bleu (Antoine Vacher)"
                role: qsTr("UI, French translation")
                image: "tigre-bleu.png"
                twitter: "tigre_bleu"
            }

            SectionHeader { text: qsTr("Thanks to") }

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
                name: "Jean Clovis"
                role: qsTr("French translation")
                image: "jean-clovis.png"
                github: "jean-clovis"
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

            SectionHeader {
                text: qsTr("Used third party elements")
            }

            ContributorItem {
                name: qsTr("Martin Grimme's Tidings Elements")
                role: "FancyScroller, RescalingRichText"
                website: "https://github.com/pycage/tidings"
                github: "pycage"
            }
        }
    }
}
