import QtQuick 1.1
import com.nokia.meego 1.0

PageStackWindow {
    id: appWindow

    initialPage: mainPage

    MainPage {
        id: mainPage
    }

    ToolBarLayout {
        id: commonTools
        visible: false
        ToolIcon {
            platformIconId: "toolbar-back"
            onClicked: Qt.quit()
        }
        ToolIcon {
            platformIconId: "toolbar-view-menu"
            anchors.right: (parent === undefined) ? undefined : parent.right
            onClicked: (createMenu.status == DialogStatus.Closed) ? createMenu.open() : createMenu.close()
        }
    }

    ToolBarLayout {
        id: editTools
        visible: false
        ToolButton {
            text: qsTrId("qtn_button_save")
            enabled: mainPage.checkFilled()
            onClicked: mainPage.saveAccount()
        }
        ToolButton {
            text: qsTrId("qtn_button_cancel")
            onClicked: Qt.quit()
        }
        ToolIcon {
            platformIconId: "toolbar-view-menu"
            anchors.right: (parent === undefined) ? undefined : parent.right
            onClicked: (editMenu.status == DialogStatus.Closed) ? editMenu.open() : editMenu.close()
        }
    }

    Menu {
        id: editMenu
        visualParent: pageStack
        MenuLayout {
            MenuItem { text: qsTrId("qtn_account_help"); onClicked: helpSheet.open() }
            MenuItem { text: qsTrId("qtn_delete_account"); onClicked: delconfirm.open() }
        }
    }

    Menu {
        id: createMenu
        visualParent: pageStack
        MenuLayout {
            MenuItem { text: qsTrId("qtn_account_help"); onClicked: helpSheet.open() }
        }
    }

    QueryDialog {
        id: delconfirm
        titleText: qsTrId("qtn_delete_title")
        message: qsTrId("qtn_delete_message")
        acceptButtonText: qsTrId("qtn_delete_accept")
        rejectButtonText: qsTrId("qtn_delete_reject")
        onAccepted: actor.deleteAccount()
    }

    HelpSheet {
        id: helpSheet
    }
}
