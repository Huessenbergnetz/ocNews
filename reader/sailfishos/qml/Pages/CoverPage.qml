import QtQuick 2.0
import Sailfish.Silica 1.0

CoverBackground {
    id: defCover

    property bool active: status == Cover.Active

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
            ret = (nDays > 1) ? qsTr("%1 days ago").arg(nDays) : qsTr("1 day ago");
        } else if (nMins > 59) {
            ret = (nHours > 1) ? qsTr("%1 hours ago").arg(nHours) : qsTr("1 hour ago");
        } else if (nMins > 0) {
            ret = (nMins > 1) ? qsTr("%1 minutes ago").arg(nMins) : qsTr("1 minute ago");
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
        visible: !updateRunning.visible
        anchors { top: parent.top; left: parent.left; right: parent.right; topMargin: Theme.paddingLarge; leftMargin: Theme.paddingLarge; rightMargin: Theme.paddingLarge }

        Text {
            id: count
            color: Theme.primaryColor
            font.pixelSize: Theme.fontSizeHuge
            font.weight: Font.Light
            textFormat: Text.PlainText
            text: unreadItems
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
        visible: !updateRunning.visible
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
                parent.opacity = 0.5 + 0.5 * Math.sin(angle * (Math.PI / 180.0));
                angle = (angle + 10) % 360;
            }
        }
    }

    CoverActionList {
        id: coverAction
        enabled: !updateRunning.visible

        CoverAction {
            iconSource: "image://theme/icon-cover-new"
            onTriggered: {
                pageStack.push(Qt.resolvedUrl("../Dialogs/CreateFeed.qml"))
                ocNewsReader.activate()
            }
        }

        CoverAction {
            iconSource: "image://theme/icon-cover-sync"
            onTriggered: { updateRunning.visible = true; updater.startUpdate(); }
        }

    }
}
