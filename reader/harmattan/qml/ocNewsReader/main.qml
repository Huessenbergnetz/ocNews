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
            Connections {
                target: folders
                onCreatedFolderSuccess: { infoMessages.show(); infoMessages.text = qsTr("Created folder %1").arg(foldername); infoMessages.iconSource = "image://theme/icon-s-common-done"; successfulEffect.play() }
                onCreatedFolderError: { infoMessages.show(); infoMessages.text = createresulterror; infoMessages.iconSource = "image://theme/icon-s-error"; errorEffect.play() }
                onRenamedFolderSuccess: { infoMessages.show(); infoMessages.text = qsTr("Renamed folder to %1").arg(newfoldername); infoMessages.iconSource = "image://theme/icon-s-common-done"; successfulEffect.play() }
                onRenamedFolderError: { infoMessages.show(); infoMessages.text = renameresulterror; infoMessages.iconSource = "image://theme/icon-s-error"; errorEffect.play() }
                onDeletedFolderSuccess: { infoMessages.show(); infoMessages.text = qsTr("Deleted Folder."); infoMessages.iconSource = "image://theme/icon-s-common-done"; successfulEffect.play() }
                onDeletedFolderError: { infoMessages.show(); infoMessages.text = deleteresulterror; infoMessages.iconSource = "image://theme/icon-s-error"; errorEffect.play() }
                onRequestedFoldersError: { infoMessages.show(); infoMessages.text = requerstresulterror; infoMessages.iconSource = "image://theme/icon-s-error"; errorEffect.play() }
            }
            Connections {
                target: feeds
                onCreatedFeedSuccess: { infoMessages.show(); infoMessages.text = qsTr("Created feed %1").arg(feedname); infoMessages.iconSource = "image://theme/icon-s-common-done"; successfulEffect.play() }
                onCreatedFeedError: { infoMessages.show(); infoMessages.text = createFeedResultError; infoMessages.iconSource = "image://theme/icon-s-error"; errorEffect.play() }
                onMovedFeedSuccess: {  infoMessages.show(); infoMessages.text = qsTr("Moved feed."); infoMessages.iconSource = "image://theme/icon-s-common-done"; successfulEffect.play() }
                onMovedFeedError: { infoMessages.show(); infoMessages.text = moveFeedResultError; infoMessages.iconSource = "image://theme/icon-s-error"; errorEffect.play() }
                onDeletedFeedSuccess: { infoMessages.show(); infoMessages.text = qsTr("Deleted feed."); infoMessages.iconSource = "image://theme/icon-s-common-done"; successfulEffect.play() }
                onDeletedFeedError: { infoMessages.show(); infoMessages.text = deleteFeedResultError; infoMessages.iconSource = "image://theme/icon-s-error"; errorEffect.play() }
            }
            Connections {
                target: items
                onStarredItemsError: { infoMessages.show(); infoMessages.text = starredItemsError; infoMessages.iconSource = "image://theme/icon-s-error"; errorEffect.play() }
                onMarkedAllItemsReadError: { infoMessages.show(); infoMessages.text = markedAllItemsReadErrorString; infoMessages.iconSource = "image://theme/icon-s-error"; errorEffect.play() }
            }
            Connections {
                target: updater
                onUpdateError: { infoMessages.show(); infoMessages.text = updateErrorMessage; infoMessages.iconSource = "image://theme/icon-s-error"; errorEffect.play() }
            }
            Connections {
                target: dbus
                onGotVersion: { infoMessages.show(); infoMessages.text = qsTr("Found ownCloud News Version %1").arg(version); infoMessages.iconSource = "image://theme/icon-s-common-done"; successfulEffect.play() }
                onGotVersionError: { infoMessages.show(); infoMessages.text = errmsg; infoMessages.iconSource = "image://theme/icon-s-error"; errorEffect.play() }
                onInitError: { infoMessages.show(); infoMessages.text = errorMessage; infoMessages.iconSource = "image://theme/icon-s-error"; errorEffect.play() }
                onCleanedDatabase: { infoMessages.show(); infoMessages.text = qsTr("Deleted complete database"); infoMessages.iconSource = "image://theme/icon-s-common-done"; successfulEffect.play() }
                onCleanedCertificates: { infoMessages.show(); infoMessages.text = qsTr("Removed trusted certificates. The changes take effect after an application restart."); infoMessages.iconSource = "image://theme/icon-s-common-done"; successfulEffect.play() }
            }
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
