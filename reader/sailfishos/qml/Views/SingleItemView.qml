import QtQuick 2.0
import Sailfish.Silica 1.0
import "../Common"

Page {
    id: singleItemView
    objectName: "ItemPage"

    property string itemId
    property bool directOpening: false

    property int handleRead
    property bool sortAsc
    property string searchString
    property string feedType
    property string parentFeedId

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
    property int enclosureType
    property string enclosureHost
    property string enclosureName
    property string prevId
    property string nextId

    property bool showImgsDefault: dbus.getSetting("display/handleimgs", 0) > 0

    property string _RICHTEXT_STYLESHEET_PREAMBLE: "<html><style>a { text-decoration: none; color: '" + Theme.secondaryHighlightColor + "' }</style><body>";
    property string _RICHTEXT_STYLESHEET_APPENDIX: "</body></html>";

    function getItemData(showImgs)
    {
        var itemData = singleItemModelSql.getItemData(itemId, handleRead, sortAsc, searchString, feedType, parentFeedId, showImgs);
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
        containsImg = itemData["containsImg"];
        enclosureType = itemData["enclosureType"];
        enclosureHost = itemData["enclosureHost"];
        enclosureName = itemData["enclosureName"];
        prevId = itemData["previous"];
        nextId = itemData["next"];
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
            if (unread) { items.markItems("read", markParams()); unread = false }
            pageStack.pushAttached(Qt.resolvedUrl("SingleItemWebView.qml"), {itemUrl: url})

            coverConnector.feedName = singleItemView.feedName
            coverConnector.title = singleItemView.title
            coverConnector.itemId = singleItemView.itemId
            coverConnector.searchString = singleItemView.searchString
            coverConnector.handleRead = singleItemView.handleRead
            coverConnector.sortAsc = singleItemView.sortAsc
            coverConnector.feedType = singleItemView.feedType
            coverConnector.parentFeedId = singleItemView.parentFeedId
            coverConnector.nextId = singleItemView.nextId
            coverConnector.prevId = singleItemView.prevId
            coverConnector.loading = false
        }
    }

    Connections {
        target: items
        onStarredItemsSuccess: starred = !starred
    }

    SilicaFlickable {
        id: singleItem
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottom: itemViewFetchIndicator.visible ? itemViewFetchIndicator.top : parent.bottom
        clip: true
        VerticalScrollDecorator {}
        contentHeight: contentCol.height

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
            MenuItem {
                text: prevId !== "0" ? qsTr("Previous in list") : qsTr("First in list")
                enabled: prevId !== "0"
                onClicked: pageStack.replace(Qt.resolvedUrl("SingleItemView.qml"), { itemId: prevId, searchString: searchString, handleRead: handleRead, sortAsc: sortAsc, feedType: feedType, parentFeedId: parentFeedId })
            }
        }


        PushUpMenu {
            id: itemViewPushy
            MenuItem {
                text: nextId !== "0" ? qsTr("Next in list") : qsTr("Last in list")
                onClicked: pageStack.replace(Qt.resolvedUrl("SingleItemView.qml"), { itemId: nextId, searchString: searchString, handleRead: handleRead, sortAsc: sortAsc, feedType: feedType, parentFeedId: parentFeedId })
                enabled: nextId !== "0"
            }
            MenuItem {
                id: goToTop
                text: qsTr("Scroll to top")
                onClicked: singleItem.scrollToTop()
                visible: singleItem.contentHeight >= singleItem.height
            }
            MenuItem {
                text: qsTr("Open in Browser")
                onClicked: Qt.openUrlExternally(url)
            }
        }


        Column {
            id: contentCol
            width: parent.width
            height: childrenRect.height

            PageHeader { title: feedName }

            Row {
                id: headerRow
                anchors { left: parent.left; right: parent.right; leftMargin: Theme.paddingLarge; rightMargin: Theme.paddingLarge }
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
                    opacity: starred ? 1 : 0
                    width: 32
                    height: 32
                    sourceSize.width: 32
                    sourceSize.height: 32
                    source: "image://theme/icon-s-favorite"
                    Behavior on opacity { FadeAnimation{} }
                }

            }

            Row {
                id: pubInfos
                anchors { left: parent.left; right: parent.right; leftMargin: Theme.paddingLarge; rightMargin: Theme.paddingLarge }

                Label {
                    id: pubDateText
                    text: pubDate
                    font.pixelSize: Theme.fontSizeExtraSmall
                    textFormat: Text.PlainText
                    color: Theme.highlightColor
                }

                Label {
                    id: authorText
                    text: author != "" ? " | " + author : ""
                    font.pixelSize: Theme.fontSizeExtraSmall
                    width: parent.width - pubDateText.width
                    truncationMode: TruncationMode.Fade
                    textFormat: Text.PlainText
                    color: Theme.highlightColor
                }
            }

            Item {
                width: 1
                height: Theme.paddingSmall
            }

            Separator {
                id: sep
                anchors { left: parent.left; right: parent.right; leftMargin: Theme.paddingLarge; rightMargin: Theme.paddingLarge }
                width: parent.width
                color: Theme.primaryColor
            }

            Item {
                width: 1
                height: Theme.paddingSmall
            }

            RescalingRichText {
                id: bodyText
                text: body
                fontSize: ocNewsReader.fontSize
                color: Theme.primaryColor
                anchors { left: parent.left; right: parent.right; leftMargin: Theme.paddingLarge; rightMargin: Theme.paddingLarge }
                onLinkActivated: pageStack.push(Qt.resolvedUrl("../Dialogs/OpenLink.qml"), {link: link})
            }

            EnclosureItem {
                id: enclosure
                width: parent.width
                visible: enclosureLink != ""
                enclosureItemId: itemId
                name: enclosureName
                host: enclosureHost
                encType: enclosureType
                encSrc: enclosureLink
                encMime: enclosureMime
            }

            Item {
                width: parent.width
                height: Theme.paddingLarge
            }
        }
    }

    FetchImagesIndicator {
        id: itemViewFetchIndicator
    }
}
