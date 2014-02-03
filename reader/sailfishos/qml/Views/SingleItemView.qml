import QtQuick 2.0
import Sailfish.Silica 1.0
import "../Common"

Page {
    id: singleItemView

    property string itemId
    property bool directOpening: false

    property string guidHash
    property string url
    property string title
    property string author
    property string pubDate
    property string body
    property string enclosureMime
    property string enclosureLink
    property bool unread
    property bool starred
    property string feedName
    property string feedId
    property bool containsImg

    property string textFormatType: dbus.getSetting("display/textformat", "rich")
    property bool showImgsDefault: dbus.getSetting("display/handleimgs", 0) > 0

    property string _RICHTEXT_STYLESHEET_PREAMBLE: "<html><style>a { text-decoration: none; color: '" + Theme.secondaryHighlightColor + "' }</style><body>";
    property string _RICHTEXT_STYLESHEET_APPENDIX: "</body></html>";

    function getItemData(showImgs)
    {
        var itemData = singleItemModelSql.getItemData(itemId, showImgs);
        guidHash = itemData["guidHash"];
        url = itemData["url"];
        title = itemData["title"];
        author = itemData["author"];
        pubDate = itemData["pubDate"];
        body = itemData["body"];
        enclosureMime = itemData["enclosureMime"];
        enclosureLink = itemData["enclosureLink"];
        unread = itemData["unread"];
        starred = itemData["starred"];
        feedName = itemData["feedName"];
        feedId= itemData["feedId"];
        containsImg = itemData["containsImg"]
    }

    function starParams() {
        var params={};
        params[feedId]=guidHash;
        return params;
    }

    function markParams() {
        var params = new Array();
        params[0]=itemId;
        return params;
    }

    allowedOrientations: Orientation.Landscape | Orientation.Portrait

    Component.onCompleted: {
        getItemData(showImgsDefault);
    }

    onStatusChanged: {
        if (status == PageStatus.Active) {
            if (unread) items.markItems("read", markParams())
            pageStack.pushAttached(Qt.resolvedUrl("SingleItemWebView.qml"), {itemUrl: url})
        }
    }

    Connections {
        target: items
        onStarredItemsSuccess: starred = !starred
    }

    SilicaFlickable {
        id: singleItem
        PageHeader { title: feedName }
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottom: itemViewFetchIndicator.visible ? itemViewFetchIndicator.top : parent.bottom
        VerticalScrollDecorator {}

        contentHeight: headerRow.height + pubInfos.height + sep.height + bodyText.height + 5 * Theme.paddingLarge

        PullDownMenu {
            id: singleItemPully
            busy: operationRunning
            MenuItem {
                text: qsTr("Open in Browser")
                onClicked: Qt.openUrlExternally(url)
            }
            MenuItem {
                text: starred ? qsTr("Remove from favourites") : qsTr("Mark as favourite")
                enabled: !operationRunning
                onClicked: {
                    operationRunning = true
                    starred ? items.starItems("unstar", starParams() ) : items.starItems("star", starParams() )
                }
            }
            MenuItem {
                text: qsTr("Show images")
                visible: containsImg
                onClicked: getItemData(true);
            }
        }

        Row {
            id: headerRow
            anchors { top: parent.top; left: parent.left; right: parent.right; topMargin: 4 * Theme.paddingLarge; leftMargin: Theme.paddingLarge; rightMargin: Theme.paddingLarge }
            spacing: 5

            Label {
                id: itemTitle
                width: parent.width - starImage.width
                text: title
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                textFormat: Text.PlainText
                color: Theme.highlightColor
            }


            Image {
                id: starImage
                visible: starred
                width: 32
                height: 32
                sourceSize.width: 32
                sourceSize.height: 32
                source: "image://theme/icon-s-favorite"
            }

        }

        Row {
            id: pubInfos
            anchors { top: headerRow.bottom; topMargin: 2; left: parent.left; right: parent.right; leftMargin: Theme.paddingLarge; rightMargin: Theme.paddingLarge }
            width: parent.width

            Label {
                id: pubDateText
                text: pubDate
                font.pixelSize: Theme.fontSizeExtraSmall
//                font.weight: Font.Light
                textFormat: Text.PlainText
                color: Theme.highlightColor
            }

            Label {
                id: authorText
                text: author != "" ? " | " + author : ""
                font.pixelSize: Theme.fontSizeExtraSmall
//                font.weight: Font.Light
                width: parent.width - pubDateText.width
                truncationMode: TruncationMode.Fade
                textFormat: Text.PlainText
                color: Theme.highlightColor
            }
        }

        Separator {
            id: sep
            anchors { top: pubInfos.bottom; topMargin: 5; left: parent.left; right: parent.right; leftMargin: Theme.paddingLarge; rightMargin: Theme.paddingLarge }
            width: parent.width
            color: Theme.primaryColor
        }

        Text {
            id: bodyText
            text: textFormatType === "rich" ? _RICHTEXT_STYLESHEET_PREAMBLE + body + _RICHTEXT_STYLESHEET_APPENDIX : body
            width: singleItem.width
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            font.pixelSize: Theme.fontSizeSmall
            font.weight: Font.Light
            anchors { top: sep.bottom; topMargin: 12; left: parent.left; right: parent.right; leftMargin: Theme.paddingLarge; rightMargin: Theme.paddingLarge }
            onLinkActivated: pageStack.push(Qt.resolvedUrl("../Dialogs/OpenLink.qml"), {link: link})
            smooth: true
            textFormat: textFormatType == "rich" ? Text.RichText : Text.StyledText
            color: Theme.primaryColor
        }

        PushUpMenu {
            id: itemViewPushy
            enabled: singleItem.contentHeight >= singleItem.height
            visible: singleItem.contentHeight >= singleItem.height
            MenuItem {
                id: goToTop
                text: qsTr("Scroll to top")
                onClicked: singleItem.scrollToTop()
            }
            MenuItem {
                text: qsTr("Open in Browser")
                onClicked: Qt.openUrlExternally(url)
            }
        }

    }

    FetchImagesIndicator {
        id: itemViewFetchIndicator
    }
}
