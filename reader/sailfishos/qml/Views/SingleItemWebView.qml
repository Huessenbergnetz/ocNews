import QtQuick 2.0
import QtWebKit 3.0
import Sailfish.Silica 1.0

Page {
    id: singleWebView

    property string itemUrl

    onStatusChanged: {
        if (status == PageStatus.Active) {
            webView.url = itemUrl
        }
    }

//    Component.onCompleted: {
//            webView.url = itemUrl
//        }

    SilicaWebView {
        id: webView
        anchors.fill: parent

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

//        ViewPlaceholder {
//            id: viewPlaceHolder
//            property string errorString

//            enabled: webView.opacity === 0 && !webView.loading
//            text: "Web content load error: " + errorString
//            hintText: "Check network connectivity and pull down to reload"
//        }
    }
}
