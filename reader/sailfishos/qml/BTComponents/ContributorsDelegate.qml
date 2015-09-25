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

import QtQuick 2.2
import Sailfish.Silica 1.0

ListItem {
    id: root
    width: parent.width
    contentHeight: Math.max(contentRow.height, Theme.itemSizeLarge)
    contentWidth: parent.width

    property string avatarPath
    property string website: model.website ? model.website : ""
    property string twitter: model.twitter ? model.twitter : ""
    property string github: model.github ? model.github : ""
    property string bitbucket: model.bitbucket ? model.bitbucket : ""
    property string linkedin: model.linkedin ? model.linkedin : ""
    property string weibo: model.weibo ? model.weibo : ""

    enabled: (website || twitter || github || bitbucket || linkedin || weibo)

    showMenuOnPressAndHold: true

    menu: root.enabled ? contextMenu : null

    onClicked: root.menuOpen ? hideMenu() : showMenu()

    Row {
        id: contentRow
        spacing: 10
        anchors { left: parent.left; leftMargin: Theme.horizontalPageMargin; right: parent.right; rightMargin: Theme.horizontalPageMargin }

        Image {
            id:contribImage
            source: model.image ? avatarPath + "/" + model.image : "images/placeholder.png"
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
                color: root.highlighted ? Theme.highlightColor : Theme.primaryColor
            }

            Label {
                text: qsTranslate("ContributorsModel", model.role)
                font.pixelSize: Theme.fontSizeExtraSmall
                width: parent.width
                wrapMode: Text.WordWrap
                color: root.highlighted ? Theme.highlightColor : Theme.primaryColor
            }

            Row {
                id: serviceImages
                spacing: Theme.paddingSmall

                Image {
                    sourceSize.width: 32; sourceSize.height: 32
                    source: "images/icon-s-browser.png"
                    visible: root.website
                }

                Image {
                    sourceSize.width: 32; sourceSize.height: 32
                    source: "images/icon-s-twitter.png"
                    visible: root.twitter
                }

                Image {
                    sourceSize.width: 32; sourceSize.height: 32
                    source: "images/icon-s-github.png"
                    visible: root.github
                }

                Image {
                    sourceSize.width: 32; sourceSize.height: 32
                    source: "images/icon-s-bitbucket.png"
                    visible: root.bitbucket
                }

                Image {
                    sourceSize.width: 32; sourceSize.height: 32
                    source: "images/icon-s-linkedin.png"
                    visible: root.linkedin
                }

                Image {
                    sourceSize.width: 32; sourceSize.height: 32
                    source: "images/icon-s-weibo.png"
                    visible: root.weibo
                }
            }
        }
    }

    Component {
        id: contextMenu
        ContextMenu {
            MenuItem {
                text: qsTr("Website")
                visible: root.website
                onClicked: Qt.openUrlExternally(root.website)
            }
            MenuItem {
                text: "Twitter"
                visible: root.twitter
                onClicked: Qt.openUrlExternally("https://twitter.com/" + root.twitter)
            }
            MenuItem {
                text: "GitHub"
                visible: root.github
                onClicked: Qt.openUrlExternally("https://github.com/" + root.github)
            }
            MenuItem {
                text: "Bitbucket"
                visible: root.bitbucket
                onClicked: Qt.openUrlExternally("https://bitbucket.org/" + root.bitbucket)
            }
            MenuItem {
                text: "LinkedIn"
                visible: root.linkedin
                onClicked: Qt.openUrlExternally("http://www.linkedin.com/profile/view?id=" + root.linkedin)
            }
            MenuItem {
                text: qsTr("Sina Weibo")
                visible: root.weibo
                onClicked: Qt.openUrlExternally("http://www.weibo.com/" + root.weibo)
            }
        }
    }
}
