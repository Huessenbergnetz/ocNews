import QtQuick 2.2
import Sailfish.Silica 1.0

CoverBackground {
    id: defCover

    property bool active: status === Cover.Active

    Connections {
        target: updater
        onUpdateFinished: lastUpdText.text = defCover.calcLastUpd(dbus.getStat(1));
    }

    Timer {
        interval: 60000
        repeat: true
        triggeredOnStart: true
        running: defCover.active
        onTriggered: lastUpdText.text = defCover.calcLastUpd(dbus.getStat(1));
    }

    function calcLastUpd(lastDate)
    {

        var last = new Date(lastDate);
        var now = new Date();
        var diff = now - last;

        var nDays = Math.round(diff/(1000*60*60*24));
        var nHours = Math.round(diff/(1000*60*60));
        var nMins = Math.round(diff/(1000*60));

        var ret;

        if (nDays > 14) {
            ret = qsTr("Never")
        } else if (nHours > 23) {
            ret = qsTr("%n day(s) ago", "", nDays)
        } else if (nMins > 59) {
            ret = qsTr("%n hour(s) ago", "", nHours)
        } else if (nMins > 0) {
            ret = qsTr("%n minute(s) ago", "", nMins)
        } else {
            ret = qsTr("Just now")
        }

        return ret;
    }

    Image {
        source: "/usr/share/harbour-ocnews-reader/icons/graphic-cover-ocnews-background.png"
        x: 0; y: 0; z: -1
        opacity: 0.1
        width: parent.width
    }

    Column {
        id: countColumn
        visible: !updateRunning.visible && coverConnector.mode === "overview"
        anchors { top: parent.top; left: parent.left; right: parent.right; topMargin: Theme.paddingLarge; leftMargin: Theme.paddingLarge; rightMargin: Theme.paddingLarge }

        Text {
            id: count
            color: Theme.primaryColor
            font.pixelSize: Theme.fontSizeHuge
            font.weight: Font.Light
            textFormat: Text.PlainText
            text: config.viewMode === 0 ? foldersModelSql.totalUnread : combinedModelSql.totalUnread
        }

        Text {
            text: qsTr("Unread news")
            id: countText
            width: parent.width
            color: Theme.primaryColor
            font.pixelSize: Theme.fontSizeSmall
            textFormat: Text.StyledText
            lineHeight: 0.7
            wrapMode: Text.WordWrap
        }
    }

    Label {
        id: lastUpdText
        color: Theme.highlightColor
        anchors { top: countColumn.bottom; left: parent.left; right: parent.right; topMargin: Theme.paddingLarge * 1.5; leftMargin: Theme.paddingLarge; rightMargin: Theme.paddingLarge }
        font.pixelSize: Theme.fontSizeLarge
        text: defCover.calcLastUpd(dbus.getStat(1));
        width: parent.width
        textFormat: Text.PlainText
        visible: !updateRunning.visible && coverConnector.mode === "overview"
        wrapMode: Text.WordWrap
        lineHeight: 0.7
    }

    Label {
        id: updateRunning
        color: Theme.highlightColor
        text: qsTr("Updating")
        anchors { verticalCenter: parent.verticalCenter; left: parent.left; right: parent.right; leftMargin: Theme.paddingLarge; rightMargin: Theme.paddingLarge }
        width: parent.width
        visible: false
        font.pixelSize: Theme.fontSizeLarge
        maximumLineCount: 1
        truncationMode: TruncationMode.Fade
        textFormat: Text.PlainText
        Connections {
            target: updater
            onUpdateFinished: { updateRunning.visible = false }
            onUpdateError: { updateRunning.visible = false }
            onUpdateStarted: { updateRunning.visible = true }
        }
        Timer {
            property int angle: 0

            running: defCover.active && parent.visible
            interval: 50
            repeat: true

            onTriggered: {
                var a = angle;
                var opacity = 0.5 + 0.5 * Math.sin(angle * (Math.PI / 180.0));
                // Workaround for Silica's bug on opacity != 0 on cover page when label is truncated with fading
                parent.color = Theme.rgba(Theme.highlightColor, opacity)
                angle = (angle + 10) % 360;
            }
        }
    }




    Column {
        visible: !updateRunning.visible && coverConnector.mode === "item" && !coverConnector.loading

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: Theme.paddingLarge
        anchors.rightMargin: Theme.paddingLarge

        Item {
            width: parent.width
            height: Theme.paddingLarge
        }

        Label {
            width: parent.width
            truncationMode: TruncationMode.Fade
            font.pixelSize: Theme.fontSizeExtraSmall
            color: Theme.secondaryColor
            maximumLineCount: 1
            text: item.feedName
        }

        Separator {
            width: parent.width
            color: Theme.secondaryColor
        }

        Item {
            width: parent.width
            height: Theme.paddingSmall
        }

        Label {
            width: parent.width
            color: Theme.primaryColor
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            elide: Text.ElideRight
            font.pixelSize: Theme.fontSizeSmall
            maximumLineCount: 6
            text: item.title
            }
        }


    CoverActionList {
        id: coverAction
        enabled: !updateRunning.visible && coverConnector.mode === "overview"

        CoverAction {
            iconSource: "image://theme/icon-cover-new"
            onTriggered: {
                var dialog = pageStack.push(Qt.resolvedUrl("../Dialogs/CreateFeed.qml"))
                dialog.accepted.connect(function() {operationRunning = true })
                ocNewsReader.activate()
            }
        }

        CoverAction {
            iconSource: "image://theme/icon-cover-sync"
            onTriggered: { updateRunning.visible = true; updater.startUpdate(); }
        }
    }


    CoverActionList {
        enabled: !updateRunning.visible && coverConnector.mode === "item" && item.previous !== 0 && item.next !== 0

        CoverAction {
            iconSource: "image://theme/icon-cover-previous"
            onTriggered: pageStack.currentPage.loadPrevNext(item.previous)
        }

        CoverAction {
            iconSource: "image://theme/icon-cover-next"
            onTriggered: pageStack.currentPage.loadPrevNext(item.next)
        }
    }


    CoverActionList {
        enabled: !updateRunning.visible && coverConnector.mode === "item" && item.previous === 0 && item.next !== 0

        CoverAction {
            iconSource: "image://theme/icon-cover-next"
            onTriggered: pageStack.currentPage.loadPrevNext(item.next)
        }
    }


    CoverActionList {
        enabled: !updateRunning.visible && coverConnector.mode === "item" && item.previous !== 0 && item.next === 0

        CoverAction {
            iconSource: "image://theme/icon-cover-previous"
            onTriggered: pageStack.currentPage.loadPrevNext(item.previous)
        }
    }
}
