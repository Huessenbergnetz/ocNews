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

Item {
    id: changelogDelegate

    width: parent.width
    height: col.height

    property string trackerUrl: ""

    property string _RICHTEXT_STYLESHEET_PREAMBLE: "<html><style>a { text-decoration: none; color: '" + Theme.secondaryHighlightColor + "' }</style><body>";
    property string _RICHTEXT_STYLESHEET_APPENDIX: "</body></html>";

    Column {
        id: col
        anchors { left: parent.left; leftMargin: Theme.horizontalPageMargin; right: parent.right; rightMargin: Theme.horizontalPageMargin }

        Item {
            width: parent.width
            height: Theme.itemSizeSmall

            Text {
                width: parent.width/2
                height: parent.height
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
                font.pixelSize: Theme.fontSizeMedium
                color: Theme.highlightColor
                anchors.left: parent.left
                text: model.version
            }

            Text {
                width: parent.width/2
                height: parent.height
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignRight
                font.pixelSize: Theme.fontSizeExtraSmall
                color: Theme.secondaryColor
                anchors.right: parent.right
                text: Qt.formatDateTime(new Date(model.date), Qt.DefaultLocaleShortDate)
            }
        }

        Repeater {
            width: parent.width
            model: entries

            Item {
                width: parent.width
                height: desc.height

                property string issueUrl: (model.issue && trackerUrl) ? " (<a href='" + trackerUrl + model.issue +"'>#" + model.issue + "</a>)" : ""

                Image {
                    id: typeIcon
                    width: 32; height: 32
                    source: model.type === 0 ? "image://theme/icon-m-add" : model.type === 1 ? "image://theme/icon-m-favorite" : model.type === 2 ? "image://theme/icon-m-crash-reporter" : "image://theme/icon-s-high-importance"
                    anchors { left: parent.left; top: parent.top; }
                }

                Text {
                    id: desc
                    width: parent.width - typeIcon.width - Theme.paddingSmall
                    anchors { left: typeIcon.right; leftMargin: Theme.paddingSmall; right: parent.right }
                    textFormat: Text.RichText
                    color: Theme.primaryColor
                    wrapMode: Text.WordWrap
                    font.pixelSize: Theme.fontSizeSmall
                    onLinkActivated: { Qt.openUrlExternally(link) }
                    text: _RICHTEXT_STYLESHEET_PREAMBLE + model.description + issueUrl + _RICHTEXT_STYLESHEET_APPENDIX
                }
            }
        }
    }
}
