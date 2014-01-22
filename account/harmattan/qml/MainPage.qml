import QtQuick 1.1
import com.nokia.meego 1.0

Page {
    id: mainPage
    anchors.fill: parent

    Component.onCompleted: {
        if (initType==1)
        {
            editTools.visible=true
            tools=editTools
        }
        else
        {
            commonTools.visible=true
            tools=commonTools
        }
    }

    function checkFilled()
    {
        return (server.text.length>0 && username.text.length>0 && password.text.length>0);
    }

    function createAccount()
    {
//        createfade.visible = true;
        actor.createAccount(username.text,
                            password.text,
                            server.text,
                            usessl.checked,
                            ignoresslerrors.checked);
    }

    function saveAccount()
    {
        actor.saveAccount(username.text,
                          password.text,
                          server.text,
                          usessl.checked,
                          ignoresslerrors.checked,
                          title.checked);
    }

    Flickable {
        anchors.fill: parent
        anchors.margins: 5
        contentWidth: width
        contentHeight: contentcolumn.height
        boundsBehavior: Flickable.StopAtBounds
        visible: initType != 2

        Column {
            id: contentcolumn
            width: parent.width
            spacing: 10

            Title {
                id: title
                image: "image://theme/icon-m-ocnews"
                title: "ocNews"
                subtitle: qsTrId("qtn_ocnews_setup")
                width: parent.width
                showcheckbox: initType==1
                checked: enabledvalue
            }

            LabeledTextField {
                id: username
                width: parent.width
                label: qsTrId("qtn_ocnews_username")
                placeholderText: qsTrId("qtn_ocnews_set_username")
                text: usernametext
            }

            LabeledTextField {
                id: password
                width: parent.width
                echoMode: TextInput.Password
                label: qsTrId("qtn_password")
                placeholderText: qsTrId("qtn_set_password")
                text: passwordtext
            }

            LabeledTextField {
                id: server
                width: parent.width
                inputMethodHints: Qt.ImhUrlCharactersOnly
                label: qsTrId("qtn_server")
                placeholderText: qsTrId("qtn_set_server")
                clearbutton: true
                text: servertext
            }

            LabeledSwitch {
                id: usessl
                width: parent.width
                text: qsTrId("qtn_ssl_enabled")
                checked: usesslvalue
            }

            LabeledSwitch {
                id: ignoresslerrors
                width: parent.width
                text: qsTrId("qtn_ignore_ssl")
                checked: ignoresslerrorsvalue
            }

            ButtonElement {
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTrId("qtn_create_account")
                visible: initType!==1
                width: parent.width/2
                enabled: checkFilled()
                onClicked: createAccount()
            }
        }
    }

    Rectangle {
        id: createfade
        anchors.fill: parent
        color: "black"
        visible: onloadbusy

        Column {
            anchors.centerIn: parent
            width: parent.width
            spacing: 30

            BusyIndicator {
                anchors.horizontalCenter: parent.horizontalCenter
                platformStyle: BusyIndicatorStyle { size: "large" }
                running: parent.visible
            }

            Label {
                anchors.horizontalCenter: parent.horizontalCenter
                wrapMode: Text.WordWrap
                text: qsTrId("qtn_creating")
                visible: !onloadbusy
            }
        }
    }

    Rectangle {
        id: onlyOne
        anchors.fill: parent
        color: "black"
        visible: initType == 2

        Column {
            anchors.centerIn: parent
            width: parent.width
            spacing: 30

            Label {
                anchors.horizontalCenter: parent.horizontalCenter
                wrapMode: Text.WordWrap
                text: qsTrId("qtn_only_one_account")
                platformStyle: LabelStyle { fontPixelSize: 35 }
                width: parent.width
            }
        }
    }
}
