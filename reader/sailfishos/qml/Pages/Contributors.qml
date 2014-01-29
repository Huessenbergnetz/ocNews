import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: about

    property string _RICHTEXT_STYLESHEET_PREAMBLE: "<html><style>a { text-decoration: none; color: '" + Theme.secondaryHighlightColor + "' }</style><body>";
    property string _RICHTEXT_STYLESHEET_APPENDIX: "</body></html>";

    SilicaFlickable {
        id: singleItem
        anchors.fill: parent
        contentHeight: aboutCol.height
        VerticalScrollDecorator {}

        Column {
            id: aboutCol
            anchors { left: parent.left; leftMargin: Theme.paddingLarge; right: parent.right }
            spacing: 5
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
        }
    }
}
