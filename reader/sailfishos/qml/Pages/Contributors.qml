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

        SectionHeader { text: qsTr("Main developer") }

        Text {
            text: _RICHTEXT_STYLESHEET_PREAMBLE + "<a href='http://www.buschmann32.de'>Matthias Fehring (Buschmann)</a>" + _RICHTEXT_STYLESHEET_APPENDIX
            width: parent.width
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            font.weight: Font.Light
            onLinkActivated: { Qt.openUrlExternally(link) }
            textFormat: Text.RichText
            color: Theme.primaryColor
        }

        SectionHeader { text: qsTr("Translators") }

        Text {
            text:  _RICHTEXT_STYLESHEET_PREAMBLE + "Antoine Vacher (tigre-bleu)<br><em>French translation</em>" + _RICHTEXT_STYLESHEET_APPENDIX
            width: parent.width
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            font.weight: Font.Light
            onLinkActivated: { Qt.openUrlExternally(link) }
            textFormat: Text.RichText
            color: Theme.primaryColor
        }

    }

    }
}
