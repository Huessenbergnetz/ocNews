import QtQuick 1.1
import com.nokia.meego 1.0
//import com.nokia.extras 1.0

//import "../JS/globals.js" as GLOBALS

Page {
    id: singleItemView
    tools: singleItemViewTools
    orientationLock: PageOrientation.Automatic

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
    property string _RICHTEXT_STYLESHEET_PREAMBLE: "<html><style>a { text-decoration: none; color: '" + theme.selectionColor + "' }</style><body>";
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

    function openFile(file, properties) {
        var component = Qt.createComponent(file)
        if (component.status == Component.Ready)
            pageStack.push(component, properties);
        else
            console.log("Error loading component:", component.errorString());
    }

    Component.onCompleted: {
        getItemData(showImgsDefault);
    }

    onStatusChanged: {
        if (status == PageStatus.Active) {
            if (unread) { operationRunning = true; items.markItems("read", markParams()) }
        }
    }

    Connections {
        target: items
        onStarredItemsSuccess: starred = !starred
    }



    Flickable {
        id: itemContent
        anchors { right: parent.right; rightMargin: 15; left: parent.left; leftMargin: 15; top: parent.top; topMargin: 10; bottom: singleItemViewTools.top }
        width: parent.width
        height: parent.height - singleItemViewTools.height
        contentWidth: parent.width - 30
        contentHeight: header.height + feedNameText.height + pubDateText.height + headerSeperator.height + bodyText.height + openUrlButtonStyle.buttonHeight + showImgButtonStyle.buttonHeight + 20
        flickableDirection:  Flickable.VerticalFlick

        Behavior on height {
            NumberAnimation { duration: 300; easing.type: Easing.InOutQuad }
        }

        Row {
            id: header

            Text {
                width: itemContent.width - 35
                text: title
                font.pointSize: 23
                font.weight: Font.Light
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                textFormat: Text.PlainText
                color: theme.inverted ? "white" : "black"
            }

            BusyIndicator {
                id: headerBusyIndicator
                platformStyle: BusyIndicatorStyle { id: headerBusyIndicatorStyle; size: "small"; inverted: theme.inverted }
                visible: operationRunning
                running: operationRunning
            }
        }

        Text {
            id: feedNameText
            text: feedName
            width: parent.width
            elide: Text.ElideRight
            font.pointSize: 16
            font.weight: Font.Light
            anchors { top: header.bottom; topMargin: 10 }
            textFormat: Text.PlainText
            color: theme.inverted ? "white" : "black"
        }

        Row {
            id: pubInfos
            anchors { top: feedNameText.bottom; topMargin: 2 }
            width: parent.width

            Text {
                id: pubDateText
                text: pubDate
                font.pointSize: 16
                font.weight: Font.Light
                textFormat: Text.PlainText
                color: theme.inverted ? "white" : "black"
            }

            Text {
                id: authorText
                text: author != "" ? " | " + author : ""
                font.pointSize: 16
                font.weight: Font.Light
                width: parent.width - pubDateText.width
                elide: Text.ElideRight
                textFormat: Text.PlainText
                color: theme.inverted ? "white" : "black"
            }
        }

        Image {
            id: headerSeperator
            height: 2
            width: parent.width
            anchors { left: parent.left; right: parent.right; top: pubInfos.bottom; topMargin: 15 }
            source: "image://theme/meegotouch-separator" + (theme.inverted? "-inverted" : "") + "-background-horizontal"
            fillMode: Image.TileHorizontally
        }

        Text {
            id: bodyText
            text: textFormatType === "rich" ? _RICHTEXT_STYLESHEET_PREAMBLE + body + _RICHTEXT_STYLESHEET_APPENDIX : body
            width: itemContent.width
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            font.pointSize: 17
            font.weight: Font.Light
            anchors { top: headerSeperator.bottom; topMargin: 12 }
            onLinkActivated: { Qt.openUrlExternally(link) }
            smooth: false
            textFormat: textFormatType == "rich" ? Text.RichText : Text.StyledText
            color: theme.inverted ? "white" : "black"
        }

        Button {
            id: openUrlButton
            text: qsTr("Open website")
            anchors { top: bodyText.bottom; topMargin: 20; horizontalCenter: parent.horizontalCenter }
            onClicked: Qt.openUrlExternally(url)
            platformStyle: ButtonStyle { id: openUrlButtonStyle }
        }

        Button {
            id: showImgButton
            text: qsTr("Load images")
            anchors { top: openUrlButton.bottom; topMargin: 10; horizontalCenter: parent.horizontalCenter }
            platformStyle: ButtonStyle { id: showImgButtonStyle }
            visible: containsImg
            onClicked: getItemData(true);
        }
    }

    ScrollDecorator {
        flickableItem: itemContent
        anchors { right: parent.right; rightMargin: -itemContent.anchors.rightMargin }
    }

// ------------- ToolBar Start -------------
    ToolBarLayout {
        id: singleItemViewTools
        visible: true
        ToolIcon {
            platformIconId: directOpening ? "toolbar-ocnews" : "toolbar-back"
            anchors.left: (parent === undefined) ? undefined : parent.left
            onClicked: directOpening ? openFile("MainView.qml") : pageStack.pop()
        }
        ToolIcon {
            platformIconId: starred ? operationRunning ? "toolbar-favorite-mark-dimmed" : "toolbar-favorite-mark" : operationRunning ? "toolbar-favorite-unmark-dimmed" : "toolbar-favorite-unmark"
            enabled: !operationRunning
            onClicked: {
                operationRunning = true
                starred ? items.starItems("unstar", starParams() ) :
                          items.starItems("star", starParams() )
            }
        }
        ToolIcon {
            platformIconId: "toolbar-share"
            onClicked: shareUi.share(url, title, feedName)
        }
    }


// ----------------- ToolBar End -------------

}
