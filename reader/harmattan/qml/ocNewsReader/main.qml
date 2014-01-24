import QtQuick 1.1
import com.nokia.meego 1.0
import com.nokia.extras 1.0
import QtMobility.feedback 1.1

import "Common"
import "Delegates"
import "Pages"
import "Sheets"
import "Views"

PageStackWindow {
    id: appWindow

    initialPage: mainView

    property bool operationRunning: updater.isUpdateRunning()

    Component.onCompleted: {
        theme.inverted = dbus.getSetting("display/themecolor", "white") == "black";
        theme.colorScheme = 15;
        if (openItemId != "0")
            openFile("Views/SingleItemView.qml", {itemId: openItemId, directOpening: true});
    }

    Connections {
        target: dbusproxy
        onDbusShowContent: { openFile("Views/SingleItemView.qml", {itemId: itemId, directOpening: true}) }
    }
    Connections {
        target: folders
        onCreatedFolderSuccess: { infoMessages.show(); infoMessages.text = qsTr("Created folder %1").arg(foldername); infoMessages.iconSource = "image://theme/icon-s-common-done"; successfulEffect.play(); operationRunning = false }
        onCreatedFolderError: { infoMessages.show(); infoMessages.text = createresulterror; infoMessages.iconSource = "image://theme/icon-s-error"; errorEffect.play(); operationRunning = false }
        onRenamedFolderSuccess: { infoMessages.show(); infoMessages.text = qsTr("Renamed folder to %1").arg(newfoldername); infoMessages.iconSource = "image://theme/icon-s-common-done"; successfulEffect.play(); operationRunning = false }
        onRenamedFolderError: { infoMessages.show(); infoMessages.text = renameresulterror; infoMessages.iconSource = "image://theme/icon-s-error"; errorEffect.play(); operationRunning = false }
        onDeletedFolderSuccess: { infoMessages.show(); infoMessages.text = qsTr("Deleted Folder."); infoMessages.iconSource = "image://theme/icon-s-common-done"; successfulEffect.play(); operationRunning = false }
        onDeletedFolderError: { infoMessages.show(); infoMessages.text = deleteresulterror; infoMessages.iconSource = "image://theme/icon-s-error"; errorEffect.play(); operationRunning = false }
        onRequestedFoldersError: { infoMessages.show(); infoMessages.text = requerstresulterror; infoMessages.iconSource = "image://theme/icon-s-error"; errorEffect.play(); operationRunning = false }
        onMarkedReadFolderError: operationRunning = false
        onMarkedReadFolderSuccess: operationRunning = false
    }
    Connections {
        target: feeds
        onCreatedFeedSuccess: { infoMessages.show(); infoMessages.text = qsTr("Created feed %1").arg(feedname); infoMessages.iconSource = "image://theme/icon-s-common-done"; successfulEffect.play(); operationRunning = false }
        onCreatedFeedError: { infoMessages.show(); infoMessages.text = createFeedResultError; infoMessages.iconSource = "image://theme/icon-s-error"; errorEffect.play(); operationRunning = false }
        onMovedFeedSuccess: {  infoMessages.show(); infoMessages.text = qsTr("Moved feed."); infoMessages.iconSource = "image://theme/icon-s-common-done"; successfulEffect.play(); operationRunning = false }
        onMovedFeedError: { infoMessages.show(); infoMessages.text = moveFeedResultError; infoMessages.iconSource = "image://theme/icon-s-error"; errorEffect.play(); operationRunning = false }
        onDeletedFeedSuccess: { infoMessages.show(); infoMessages.text = qsTr("Deleted feed."); infoMessages.iconSource = "image://theme/icon-s-common-done"; successfulEffect.play(); operationRunning = false }
        onDeletedFeedError: { infoMessages.show(); infoMessages.text = deleteFeedResultError; infoMessages.iconSource = "image://theme/icon-s-error"; errorEffect.play(); operationRunning = false }
        onMarkedReadFeedError: operationRunning = false
        onMarkedReadFeedSuccess: operationRunning = false
    }
    Connections {
        target: items
        onStarredItemsError: { infoMessages.show(); infoMessages.text = starredItemsError; infoMessages.iconSource = "image://theme/icon-s-error"; errorEffect.play(); operationRunning = false }
        onStarredItemsSuccess: operationRunning =false
        onMarkedAllItemsReadError: { infoMessages.show(); infoMessages.text = markedAllItemsReadErrorString; infoMessages.iconSource = "image://theme/icon-s-error"; errorEffect.play(); operationRunning = false }
        onMarkedAllItemsReadSuccess: operationRunning = false
        onMarkedItemsError: operationRunning = false
        onMarkedItemsSuccess: operationRunning = false
        onUpdatedItemsError: operationRunning = false
        onUpdatedItemsSuccess: operationRunning = false
    }
    Connections {
        target: updater
        onUpdateError: { infoMessages.show(); infoMessages.text = updateErrorMessage; infoMessages.iconSource = "image://theme/icon-s-error"; errorEffect.play(); operationRunning = false }
        onUpdateFinished: operationRunning = false
        onUpdateStarted: operationRunning = true
    }
    Connections {
        target: dbus
        onGotVersion: { infoMessages.show(); infoMessages.text = qsTr("Found ownCloud News Version %1").arg(version); infoMessages.iconSource = "image://theme/icon-s-common-done"; successfulEffect.play(); operationRunning = false }
        onGotVersionError: { infoMessages.show(); infoMessages.text = errmsg; infoMessages.iconSource = "image://theme/icon-s-error"; errorEffect.play(); operationRunning = false }
        onInitError: { infoMessages.show(); infoMessages.text = errorMessage; infoMessages.iconSource = "image://theme/icon-s-error"; errorEffect.play(); operationRunning = false }
        onCleanedDatabase: { infoMessages.show(); infoMessages.text = qsTr("Deleted complete database"); infoMessages.iconSource = "image://theme/icon-s-common-done"; successfulEffect.play(); operationRunning = false }
        onCleanedCertificates: { infoMessages.show(); infoMessages.text = qsTr("Removed trusted certificates. The changes take effect after an application restart."); infoMessages.iconSource = "image://theme/icon-s-common-done"; successfulEffect.play(); operationRunning = false }
    }


    function openFile(file, properties) {
             var component = Qt.createComponent(file)
             if (component.status == Component.Ready)
                 pageStack.push(component, properties);
             else
                 console.log("Error loading component:", component.errorString());
         }

    MainView {
        id: mainView
    }

    ToolBarLayout {
        id: commonTools
        visible: false
        ToolIcon {
            platformIconId: "toolbar-back"
            anchors.left: (parent === undefined) ? undefined : parent.left
            onClicked: { pageStack.pop(); }
        }
    }

    // ----------------- Start Info Messages --------------------
        InfoBanner {
            id: infoMessages
            timerShowTime: 5000
            topMargin: 50
        }

        ThemeEffect {
            id: successfulEffect
            effect: "PositiveTacticon"
        }

        ThemeEffect {
            id: errorEffect
            effect: "NegativeTacticon"
        }

    // ----------------- End Info Messages --------------------
}
