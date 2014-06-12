/*
    BTComponents - QML components shared between various Buschtrommel projects
    Copyright (C) 2014  Buschtrommel/Matthias Fehring
    Contact: Matthias Fehring <kontakt@buschmann23.de>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

import QtQuick 2.0
import Sailfish.Silica 1.0

Row {
    spacing: 10
    anchors { left: parent.left; leftMargin: Theme.paddingLarge; right: parent.right; rightMargin: Theme.paddingLarge }

    property string avatarPath
    property string iconPath

    Image {
        id:contribImage
        source: model.image ? avatarPath + "/" + model.image : avatarPath + "/placeholder.png"
        sourceSize.height: 86
        sourceSize.width: 86
        width: 86
        height: 86
    }

    Column {
        spacing: 1
        width: parent.width - contribImage.width

        Label {
            text: qsTranslate("ContributorsModel", model.name)
            font.pixelSize: Theme.fontSizeSmall
            width: parent.width
            wrapMode: Text.WordWrap
        }

        Label {
            text: qsTranslate("ContributorsModel", model.role)
            font.pixelSize: Theme.fontSizeExtraSmall
            width: parent.width
            wrapMode: Text.WordWrap
        }

        Row {
            id: imageLinks
            spacing: 10

            IconButton {
                icon.source: iconPath + "/icon-m-browser.png"
                visible: model.website ? true : false
                width: icon.width; height: icon.height
                onClicked: Qt.openUrlExternally(model.website)
            }

            IconButton {
                icon.source: iconPath + "/icon-m-twitter.png"
                visible: model.twitter ? true : false
                width: icon.width; height: icon.height
                onClicked: Qt.openUrlExternally("https://twitter.com/" + model.twitter)
            }

            IconButton {
                icon.source: iconPath + "/icon-m-github.png"
                visible: model.github ? true : false
                width: icon.width; height: icon.height
                onClicked: Qt.openUrlExternally("https://github.com/" + model.github)
            }

            IconButton {
                icon.source: iconPath + "/icon-m-bitbucket.png"
                visible: model.bitbucket ? true : false
                width: icon.width; height: icon.height
                onClicked: Qt.openUrlExternally("https://bitbucket.org/" + model.bitbucket)
            }

            IconButton {
                icon.source: iconPath + "/icon-m-linkedin.png"
                visible: model.linkedin ? true : false
                width: icon.width; height: icon.height
                onClicked: Qt.openUrlExternally("http://www.linkedin.com/profile/view?id=" + model.linkedin)
            }

            IconButton {
                icon.source: iconPath + "/icon-m-weibo.png"
                visible: model.weibo ? true : false
                width: icon.width; height: icon.height
                onClicked: Qt.openUrlExternally("http://www.weibo.com/" + model.weibo)
            }
        }
    }
}

