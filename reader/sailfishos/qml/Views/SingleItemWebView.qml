import QtQuick 2.0
//import QtWebKit 3.0
//import QtWebKit.experimental 1.0
import Sailfish.Silica 1.0

Page {
    id: singleWebView

    allowedOrientations: Orientation.Landscape | Orientation.Portrait

    property string itemUrl: ""
    property int itemId: -1
    property bool itemUnread: true

    onStatusChanged: {
        if (status === PageStatus.Active) {
            if (config.articleOpening === 1) {
                webView.url = itemUrl
                if (itemUnread) {
                    var params = [];
                    params[0] = itemId;
                    items.markItems("read", params)
                }
            } else {
                webView.url = item.url
            }
        }
    }


    BusyIndicator {
        id: loadStatus
        z: 2
        anchors { top: parent.top; topMargin: 16; horizontalCenter: parent.horizontalCenter }
        visible: webView.loading
        running: visible
        opacity: visible ? 1 : 0
        Behavior on opacity { FadeAnimation {} }
    }

    SilicaWebView {
        id: webView
        anchors.fill: parent

        PullDownMenu {
            MenuItem {
                text: qsTr("Clear cookies")
                onClicked: nami.clearCookies()
            }
            MenuItem {
                text: webView.loading ? qsTr("Stop loading") : qsTr("Reload")
                onClicked: webView.loading ? webView.stop() : webView.reload()
            }
            MenuItem {
                text: qsTr("Open in browser")
                onClicked: Qt.openUrlExternally(webView.url)
            }
            MenuItem {
                text: qsTr("Back")
                onClicked: (config.articleOpening !== 1) ? pageStack.popAttached() : pageStack.pop()
            }
        }

        _cookiesEnabled: config.enableCookies

        experimental.userAgent: "Mozilla/5.0 (Maemo; Linux; Jolla; Sailfish; Mobile) AppleWebKit/534.13 (KHTML, like Gecko) NokiaBrowser/8.5.0 Mobile Safari/534.13"
        experimental.preferences.pluginsEnabled: true
        experimental.preferences.javascriptEnabled: true
        experimental.preferences.defaultFontSize: Theme.fontSizeSmall
        experimental.preferences.privateBrowsingEnabled: config.privateBrowsing

    }
}
