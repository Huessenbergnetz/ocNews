import QtQuick 2.0
import Sailfish.Silica 1.0


Page {
    id: page

    property bool testConnResult: false

    function createAccount() {
        dbus.setSetting("account/enabled", true);
        dbus.setSetting("account/user", username.text);
        dbus.setSetting("account/password", password.text);
        dbus.setSetting("account/server", server.text);
        dbus.setSetting("account/usessl", usessl.checked);
        dbus.setSetting("account/ignoresslerrors", ignoresslerrors.checked);
    }

    function saveAccount() {
        dbus.setSetting("account/enabled", accountStatus.checked)
        dbus.setSetting("account/user", username.text);
        dbus.setSetting("account/password", password.text);
        dbus.setSetting("account/server", server.text);
        dbus.setSetting("account/usessl", usessl.checked);
        dbus.setSetting("account/ignoresslerrors", ignoresslerrors.checked);
    }

    function getAccount() {
        accountStatus.checked = dbus.getSetting("account/enabled", "true") === "true" ? true : false;
        username.text = dbus.getSetting("account/user", "");
        password.text = dbus.getSetting("account/password", "");
        server.text = dbus.getSetting("account/server", "");
        usessl.checked = dbus.getSetting("account/usessl", "true") === "true" ? true : false;
        ignoresslerrors.checked = dbus.getSetting("account/ignoresslerrors", "false") === "true" ? true : false;
    }

    Component.onCompleted: {
        if(dbus.isConfigSet()) {
            accExists = 1;
            getAccount();
        } else {
            usessl.checked = true;
        }
    }

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height + Theme.paddingLarge

        PullDownMenu {
            MenuItem {
                id: cancel
                text: qsTr("Cancel")
                onClicked: Qt.quit()
            }
            MenuItem {
                id: createOrSave
                visible: accExists === 0 ? testConnResult : true
                text: accExists === 0 ? qsTr("Create") : qsTr("Save");
                onClicked: accExists === 0 ? createAccount() : saveAccount()
            }
            MenuItem {
                id: testConn
                text: qsTr("Test connection")
                onClicked: testConnResult = true
            }
        }

        Column {
            id: column

            anchors { left: parent.left; right: parent.right }
            spacing: Theme.paddingSmall

            PageHeader {
                id: header
                title: accExists === 0 ? qsTr("Create ocNews User Account") : qsTr("Edit ocNews User Accout")
            }

            TextSwitch {
                id: accountStatus
                anchors { left: parent.left; right: parent.right }
                visible: accExists !== 0
                text: checked ? qsTr("Account is enabled") : qsTr("Account is disabled")
            }
            TextField {
                id: username
                anchors { left: parent.left; right: parent.right }
                focus: accExists === 0; label: qsTr("User Name"); placeholderText: label
                EnterKey.enabled: text || inputMethodComposing
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                EnterKey.onClicked: password.focus = true
            }
            TextField {
                id: password
                anchors { left: parent.left; right: parent.right }
                label: qsTr("Password"); placeholderText: label
                echoMode: TextInput.Password
                EnterKey.enabled: text || inputMethodComposing
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                EnterKey.onClicked: server.focus = true
            }
            TextField {
                id: server
                anchors { left: parent.left; right: parent.right }
                label: qsTr("Server"); placeholderText: qsTr("Server: e.g. cloud.example.com")
                inputMethodHints: Qt.ImhUrlCharactersOnly
                EnterKey.enabled: text || inputMethodComposing
                EnterKey.iconSource: "image://theme/icon-m-enter-next"
                EnterKey.onClicked: server.focus = false
            }
            TextSwitch {
                id: usessl
                text: qsTr("Use HTTPS (SSL/TLS) connection")
                anchors { left: parent.left; right: parent.right }
                description: qsTr("Because ownCloud News requires to send your username and password with every request, you should keep this enabled to use an encrypted connection, when your server supports encryption.")
            }
            TextSwitch {
                id: ignoresslerrors
                text: qsTr("Ignore SSL errors")
                anchors { left: parent.left; right: parent.right }
                description: qsTr("Only ignore SSL errors when you really know what you are doing.")
                enabled: usessl.checked
            }
        }
    }
}
