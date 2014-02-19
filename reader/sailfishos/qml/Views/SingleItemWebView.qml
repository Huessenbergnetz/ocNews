import QtQuick 2.0
//import QtWebKit 3.0
//import QtWebKit.experimental 1.0
import Sailfish.Silica 1.0

Page {
    id: singleWebView

    property string itemUrl

    allowedOrientations: Orientation.Landscape | Orientation.Portrait

    onStatusChanged: {
        if (status == PageStatus.Active) {
            webView.url = itemUrl
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
//        width: singleWebView.width
//        height: singleWebView.height

//        header: PageHeader { title: webView.title }

//        opacity: 0
//        onLoadingChanged: {
//            switch (loadRequest.status)
//            {
//                case WebView.LoadSucceededStatus:
//                    opacity = 1
//                    break
//                case WebView.LoadFailedStatus:
//                    opacity = 0
////                    viewPlaceHolder.errorString = loadRequest.errorString
//                    break
//                default:
//                    opacity = 0
//                    break
//            }
//        }

//        FadeAnimation on opacity {}

        PullDownMenu {
            MenuItem {
                text: qsTr("Reload")
                onClicked: webView.reload()
            }
            MenuItem {
                text: qsTr("Open in browser")
                onClicked: Qt.openUrlExternally(webView.url)
            }
            MenuItem {
                text: qsTr("Back")
                onClicked: pageStack.popAttached()
            }
        }

        experimental.userAgent: "Mozilla/5.0 (Maemo; Linux; Jolla; Sailfish; Mobile) AppleWebKit/534.13 (KHTML, like Gecko) NokiaBrowser/8.5.0 Mobile Safari/534.13"

//        ViewPlaceholder {
//            id: viewPlaceHolder
//            property string errorString

//            enabled: webView.opacity === 0 && !webView.loading
//            text: "Web content load error: " + errorString
//            hintText: "Check network connectivity and pull down to reload"
//        }
    }
}
