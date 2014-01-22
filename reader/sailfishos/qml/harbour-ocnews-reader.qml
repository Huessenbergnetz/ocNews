import QtQuick 2.0
import Sailfish.Silica 1.0

import "Views"
import "Pages"

ApplicationWindow
{
    id: ocNewsReader

    initialPage: Component { MainView { } }

    cover: CoverPage {}
}
