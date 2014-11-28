import QtQuick 2.0
import Sailfish.Silica 1.0
import "../Common"

Page {
    id: singleItemView
    objectName: "ItemPage"

    property bool directOpening: false

    property string _RICHTEXT_STYLESHEET_PREAMBLE: "<html><style>a { text-decoration: none; color: '" + Theme.secondaryHighlightColor + "' }</style><body>";
    property string _RICHTEXT_STYLESHEET_APPENDIX: "</body></html>";

    function starParams() {
        var params={};
        params[item.feedId]=item.guidHash;
        return params;
    }

    function markParams() {
        var params =[];
        params[0]=item.itemId;
        return params;
    }

    function loadPrevNext(id) {
        contentCol.opacity = 0
        item.itemId = id
        singleItem.contentY = 0
        markReadTimer.restart()
        backToNonOpaque.restart()
    }

    Timer {
        id: markReadTimer
        interval: 200
        onTriggered: if (item.unread) { items.markItems("read", markParams()); item.unread = false }
    }

    Timer {
        id: backToNonOpaque
        interval: 200
        onTriggered: contentCol.opacity = 1
    }

    allowedOrientations: Orientation.Landscape | Orientation.Portrait

    onStatusChanged: {
        if (status === PageStatus.Active) {
            if (item.unread) { items.markItems("read", markParams()); item.unread = false }
            pageStack.pushAttached(Qt.resolvedUrl("SingleItemWebView.qml"))
        }
    }

    Connections {
        target: items
        onStarredItemsSuccess: item.starred = !item.starred
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
                onClicked: Qt.openUrlExternally(item.url)
            }
            MenuItem {
                text: item.starred ? qsTr("Remove from favourites") : qsTr("Mark as favourite")
                enabled: !operationRunning
                onClicked: {
                    operationRunning = true
                    item.starred ? items.starItems("unstar", starParams() ) : items.starItems("star", starParams() )
                }
            }
            MenuItem {
                text: qsTr("Show images")
                visible: item.containsImg
                onClicked: item.showImages()
            }
            MenuItem {
                text: item.previous !== 0 ? qsTr("Previous in list") : qsTr("First in list")
                enabled: item.previous !== 0
                onClicked: loadPrevNext(item.previous)
            }
        }


        PushUpMenu {
            id: itemViewPushy
            MenuItem {
                text: item.next !== 0 ? qsTr("Next in list") : qsTr("Last in list")
                enabled: item.next !== 0
                onClicked: loadPrevNext(item.next)
            }
            MenuItem {
                id: goToTop
                text: qsTr("Scroll to top")
                onClicked: singleItem.scrollToTop()
                visible: singleItem.contentHeight >= singleItem.height
            }
            MenuItem {
                text: qsTr("Open in Browser")
                onClicked: Qt.openUrlExternally(item.url)
            }
        }


        Column {
            id: contentCol
            width: parent.width
            height: childrenRect.height
            opacity: 1

            Behavior on opacity {
                NumberAnimation { target: contentCol; property: "opacity"; duration: contentCol.opacity === 1 ? 0 : 300; easing.type: Easing.InOutQuad }
            }

            PageHeader { title: item.feedName }

            Row {
                id: headerRow
                anchors { left: parent.left; right: parent.right; leftMargin: Theme.paddingLarge; rightMargin: Theme.paddingLarge }
                spacing: 5

                Label {
                    id: itemTitle
                    width: parent.width - starImage.width
                    text: item.title
                    wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                    textFormat: Text.PlainText
                    color: Theme.highlightColor
                }


                Image {
                    id: starImage
                    opacity: item.starred ? 1 : 0
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
                    text: item.pubDate
                    font.pixelSize: Theme.fontSizeExtraSmall
                    textFormat: Text.PlainText
                    color: Theme.highlightColor
                }

                Label {
                    id: authorText
                    text: item.author !== "" ? " | " + item.author : ""
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
                text: item.body
                fontSize: config.fontSize
                color: Theme.primaryColor
                anchors { left: parent.left; right: parent.right; leftMargin: Theme.paddingLarge; rightMargin: Theme.paddingLarge }
                onLinkActivated: config.directLinkOpening ? Qt.openUrlExternally(link) : pageStack.push(Qt.resolvedUrl("../Dialogs/OpenLink.qml"), {link: link})
            }

            EnclosureItem {
                id: enclosure
                width: parent.width
                visible: item.enclosureLink != ""
                enclosureItemId: item.itemId
                name: item.enclosureName
                host: item.enclosureHost
                encType: item.enclosureType
                encSrc: item.enclosureLink
                encMime: item.enclosureMime
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
