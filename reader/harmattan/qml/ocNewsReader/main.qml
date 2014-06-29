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
    id: ocNewsReader

    initialPage: mainView

    property bool operationRunning: updater.isUpdateRunning()

    Component.onCompleted: {
        theme.inverted = config.themeInverted;
        theme.colorScheme = 15;
        if (openItemId !== "0") {
            item.showImg = config.handleImgs > 0
            item.itemId = openItemId
            openFile("Views/SingleItemView.qml", {directOpening: true});
        }
    }

    Connections {
        target: config
        onThemeInvertedChanged: theme.inverted = nThemeInverted
    }

    Connections {
        target: dbusproxy
        onDbusShowContent: {
            item.showImg = config.handleImgs > 0
            item.itemId = itemId
            openFile("Views/SingleItemView.qml", {directOpening: true})
        }
    }
    Connections {
        target: folders
        onCreatedFolderSuccess: notifySuccess( qsTr("Created folder %1").arg(foldername))
        onCreatedFolderError: notifyError(createresulterror)
        onRenamedFolderSuccess: notifySuccess(qsTr("Renamed folder to %1").arg(newfoldername))
        onRenamedFolderError: notifyError(renameresulterror)
        onDeletedFolderSuccess: notifySuccess(qsTr("Deleted Folder."))
        onDeletedFolderError: notifyError(deleteresulterror)
        onRequestedFoldersError: notifyError(requerstresulterror)
        onMarkedReadFolderError: operationRunning = false
        onMarkedReadFolderSuccess: operationRunning = false
    }
    Connections {
        target: feeds
        onCreatedFeedSuccess: notifySuccess(qsTr("Created feed %1").arg(feedname))
        onCreatedFeedError: notifyError(createFeedResultError)
        onMovedFeedSuccess: notifySuccess(qsTr("Moved feed."))
        onMovedFeedError: notifyError(moveFeedResultError)
        onDeletedFeedSuccess: notifySuccess(qsTr("Deleted feed."))
        onDeletedFeedError: notifyError(deleteFeedResultError)
        onMarkedReadFeedError: operationRunning = false
        onMarkedReadFeedSuccess: operationRunning = false
        onRenamedFeedSuccess: notifySuccess(qsTr("Renamed feed to %1").arg(newName))
        onRenamedFeedError: notifyError(renamedFeedErrorString)

    }
    Connections {
        target: items
        onStarredItemsError: notifyError(starredItemsError)
        onStarredItemsSuccess: operationRunning =false
        onMarkedAllItemsReadError: notifyError(markedAllItemsReadErrorString)
        onMarkedAllItemsReadSuccess: operationRunning = false
        onMarkedItemsError: operationRunning = false
        onMarkedItemsSuccess: operationRunning = false
        onUpdatedItemsError: operationRunning = false
        onUpdatedItemsSuccess: operationRunning = false
    }
    Connections {
        target: updater
        onUpdateError: notifyError(updateErrorMessage)
        onUpdateFinished: operationRunning = false
        onUpdateStarted: operationRunning = true
    }
    Connections {
        target: dbus
        onGotVersion: notifySuccess(qsTr("Found ownCloud News Version %1").arg(version))
        onGotVersionError: notifyError(errmsg)
        onInitError: notifyError(errorMessage)
        onCleanedDatabase: notifySuccess(qsTr("Deleted complete database"))
        onCleanedCertificates: notifySuccess(qsTr("Removed trusted certificates. The changes take effect after an application restart."))
    }

    function notifySuccess(message) {
        infoMessages.show()
        infoMessages.text = message
        infoMessages.iconSource = "image://theme/icon-s-common-done"
        successfulEffect.play()
        operationRunning = false
    }

    function notifyError(message) {
        infoMessages.show()
        infoMessages.text = message
        infoMessages.iconSource = "image://theme/icon-s-error"
        errorEffect.play()
        operationRunning = false
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
