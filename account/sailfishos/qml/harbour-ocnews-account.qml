import QtQuick 2.0
import Sailfish.Silica 1.0

ApplicationWindow
{
    initialPage: Component { AccountPage { } }

    property int accExists: 0

    cover: CoverBackground {
        CoverPlaceholder {
            text: accExists === 0 ? qsTr("Create new ocNews account") : qsTr("Edit ocNews account");
            icon.source: "/usr/share/icons/hicolor/86x86/apps/harbour-ocnews-reader.png"
        }
    }
}
