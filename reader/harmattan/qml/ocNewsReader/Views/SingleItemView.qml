import QtQuick 1.1
import com.nokia.meego 1.0
import "../Sheets"
import "../Common"
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
    property int enclosureType
    property string enclosureHost
    property string enclosureName

    property bool enclosureDownloading: downloads.getCurrentItem() === itemId
    property bool enclosureInQueue: downloads.itemInQueue(itemId)
    property bool enclosureExists: downloads.itemExists(enclosureLink, enclosureMime) !== "" && !enclosureDownloading && !enclosureInQueue

    property bool showImgsDefault: dbus.getSetting("display/handleimgs", 0) > 0

    function isMailTo(url) {
        var string = url;
        if (string.indexOf("mailto") !== -1)
        {
            return true;
        } else {
            return false;
        }
    }

    function stripMailTo(url)
    {
        var string = url;
        return string.replace("mailto:","");
    }

    function isImageLink(url) {
        var string = url;
        if (string.indexOf(".jpeg") !== -1 || string.indexOf(".JPEG") !== -1)
        {
            return true;
        } else if (string.indexOf(".jpg") !== -1 || string.indexOf(".JPG") !== -1) {
            return true;
        } else if (string.indexOf(".png") !== -1 || string.indexOf(".PNG") !== -1) {
            return true;
        } else if (string.indexOf(".gif") !== -1 || string.indexOf(".GIF") !== -1) {
            return true;
        } else {
            return false;
        }
    }

    function downloadEnclosure()
    {
        enclosureInQueue = true;
        downloads.append(itemId);
    }

    function abortEnclosureDownload()
    {
        enclosureInQueue = false;
        downloads.abortDownload(itemId);
    }

    function openEnclosureDirect()
    {
        var ret;
        switch(enclosureType) {
        case 1:
            launcher.playAudio(enclosureLink);
            break;
        case 2:
            launcher.playVideo(enclosureLink);
            break;
        case 3:
            ret = "image://theme/icon-m-content-pdf"
            break;
        case 4:
            ret = "image://theme/icon-m-content-image"
            break;
        default:
            ret = "image://theme/icon-m-content-attachment";
            break;
        }
    }

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
        containsImg = itemData["containsImg"];
        enclosureType = itemData["enclosureType"];
        enclosureHost = itemData["enclosureHost"];
        enclosureName = itemData["enclosureName"];
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


    Connections {
        target: downloads
        onStarted: if(startedFileId === itemId) { enclosureDownloading = true; enclosureInQueue = false }
        onFinishedFile: if(finishedFileId === itemId) { enclosureDownloading = false; enclosureExists = downloads.itemExists(enclosureLink, enclosureMime) !== "" }
    }



    Flickable {
        id: itemContent
        anchors { right: parent.right; rightMargin: 15; left: parent.left; leftMargin: 15; top: parent.top; topMargin: 10; bottom: singleItemViewTools.top }
        width: parent.width
        height: parent.height
        contentWidth: parent.width - 30
        contentHeight: header.height + feedNameText.height + pubDateText.height + headerSeperator.height + bodyText.height + openUrlButtonStyle.buttonHeight + showImgButtonStyle.buttonHeight + 50
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

        RescalingRichText {
            id: bodyText
            text: body
            fontSize: 17
            anchors { top: headerSeperator.bottom; topMargin: 12; left: parent.left; right: parent.right }
            onLinkActivated: if (isImageLink(link)) {imagePreview.link = link; imagePreview.open()} else {linkContextMenu.link = link; linkContextMenu.open()}
            color: theme.inverted ? "white" : "black"
        }

        EnclosureItem {
            id: enclosure
            anchors { top: bodyText.bottom; topMargin: 20 }
            width: itemContent.width
            visible: enclosureLink != ""
            name: enclosureName
            host: enclosureHost
            showHost: !enclosureExists && !enclosureInQueue
            exists: enclosureExists
            inQueue: enclosureInQueue
            showProgress: enclosureDownloading
            encType: enclosureType
            Connections {
                target: downloads
                onProgress: if(enclosureDownloading) { enclosure.maxValue = tot; enclosure.value = rec }
            }
            onClicked: downloadContextMenu.open()
        }

        Button {
            id: openUrlButton
            text: qsTr("Open website")
            anchors { top: enclosure.visible ? enclosure.bottom : bodyText.bottom; topMargin: 20; horizontalCenter: parent.horizontalCenter }
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

    ContextMenu {
        id: linkContextMenu
        property string link
        MenuLayout {
            MenuItem {
                text: isMailTo(linkContextMenu.link) ? qsTr("Write to %1").arg(stripMailTo(linkContextMenu.link)) : qsTr("Open %1").arg(linkContextMenu.link)
                onClicked: {
                    Qt.openUrlExternally(linkContextMenu.link)
                }
            }
        }
    }

    ContextMenu {
        id: downloadContextMenu
        property int state
        MenuLayout {
            MenuItem {
                text: enclosureExists ? qsTr("Open enclosure") : qsTr("Open enclosure directly")
                onClicked: enclosureExists ? Qt.openUrlExternally(downloads.itemExists(enclosureLink, enclosureMime)) : singleItemView.openEnclosureDirect()
                enabled: !enclosureDownloading && !enclosureInQueue
            }
            MenuItem {
                text: enclosureDownloading || enclosureInQueue ? qsTr("Abort download") : enclosureExists ? qsTr("Delete enclosure") : qsTr("Download enclosure")
                onClicked: enclosureDownloading || enclosureInQueue ? singleItemView.abortEnclosureDownload() : enclosureExists ? deleteQuery.open() : singleItemView.downloadEnclosure()
            }
        }
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


// ----------------- Start Image Preview Sheet -----------------

    ImagePreviewSheet {
        id: imagePreview
    }


// ----------------- End Image Preview Sheet -----------------


    QueryDialog {
        id: deleteQuery
        icon: "image://theme/icon-m-bootloader-warning"
        acceptButtonText: qsTr("Delete")
        rejectButtonText: qsTr("Cancel")
        message: qsTr("Do you really want to delete this file?")
        titleText: qsTr("Delete %1?").arg(enclosureName)
        onAccepted: {
            enclosureExists = !downloads.deleteFile(enclosureLink, enclosureMime)
        }
    }

}
