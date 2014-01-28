import QtQuick 2.0
import Sailfish.Silica 1.0

import "Views"
import "Pages"

ApplicationWindow
{
    id: ocNewsReader

    property bool operationRunning: updater.isUpdateRunning()
    property int viewMode: dbus.getSetting("display/viewmode", 0)


    Connections {
        target: updater
        onUpdateFinished: { operationRunning = false; if (viewMode !== 0) combinedModelSql.refresh() }
        onUpdateError: operationRunning = false
        onUpdateStarted: operationRunning = true
    }
    Connections {
        target: folders
        onCreatedFolderError: operationRunning = false
        onCreatedFolderSuccess: { operationRunning = false; if (viewMode !== 0) combinedModelSql.refresh() }
        onDeletedFolderError: operationRunning = false
        onDeletedFolderSuccess: { operationRunning = false; if (viewMode !== 0) combinedModelSql.refresh() }
        onMarkedReadFolderError: operationRunning = false
        onMarkedReadFolderSuccess: { operationRunning = false; if (viewMode !== 0) combinedModelSql.refresh() }
        onRenamedFolderError: operationRunning = false
        onRenamedFolderSuccess: { operationRunning = false; if (viewMode !== 0) combinedModelSql.refresh() }
    }
    Connections {
        target: feeds
        onCreatedFeedError: operationRunning = false
        onCreatedFeedSuccess: { operationRunning = false; if (viewMode !== 0) combinedModelSql.refresh() }
        onDeletedFeedError: operationRunning = false
        onDeletedFeedSuccess: { operationRunning = false; if (viewMode !== 0) combinedModelSql.refresh() }
        onMarkedReadFeedError: operationRunning = false
        onMarkedReadFeedSuccess: { operationRunning = false; if (viewMode !== 0) combinedModelSql.refresh() }
        onMovedFeedError: operationRunning = false
        onMovedFeedSuccess: { operationRunning = false; if (viewMode !== 0) combinedModelSql.refresh() }
    }
    Connections {
        target: items
        onMarkedAllItemsReadError: operationRunning = false
        onMarkedAllItemsReadSuccess: { operationRunning = false; if (viewMode !== 0) combinedModelSql.refresh() }
        onMarkedItemsError: operationRunning = false
        onMarkedItemsSuccess: { operationRunning = false; if (viewMode !== 0) combinedModelSql.refresh() }
        onStarredItemsError: operationRunning = false
        onStarredItemsSuccess: { operationRunning = false; if (viewMode !== 0) combinedModelSql.refresh() }
        onUpdatedItemsError: operationRunning = false
        onUpdatedItemsSuccess: { operationRunning = false; if (viewMode !== 0) combinedModelSql.refresh() }
        onRequestedItemsError: operationRunning = false
        onRequestedItemsSuccess: { operationRunning = false; if (viewMode !== 0) combinedModelSql.refresh() }
    }
    Connections {
        target: dbus
        onSavedConfig: viewMode = dbus.getSetting("display/viewmode", 0)
    }

    initialPage: Component { MainView { } }

    cover: CoverPage {}
}
