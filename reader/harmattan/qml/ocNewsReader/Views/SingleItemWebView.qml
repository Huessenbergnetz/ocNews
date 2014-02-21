import QtQuick 1.1
import QtWebKit 1.0
import com.nokia.meego 1.0

Page {
    id: singleItemWebView
    tools: singleItemWebViewTools

    property alias url: webView.url

    Flickable {
        id: flickable
        width: parent.width
//        height: parent.height
//        contentWidth: Math.max(parent.width,webView.width)
        contentWidth: width
        contentHeight: Math.max(parent.height,webView.height)

        WebView {
            id: webView
            anchors.fill: parent
            transformOrigin: Item.TopLeft
            preferredWidth: flickable.width
            preferredHeight: flickable.height

            contentsScale: 1
                     onContentsSizeChanged: {
                         // zoom out
                         contentsScale = Math.min(1,flickable.width / contentsSize.width)
                     }
        }
    }

    ScrollDecorator {
        flickableItem: flickable
    }

    ToolBarLayout {
        id: singleItemWebViewTools
        visible: true
        ToolIcon {
            platformIconId: "toolbar-back"
            anchors.left: (parent === undefined) ? undefined : parent.left
            onClicked: pageStack.pop()
        }
    }
}
