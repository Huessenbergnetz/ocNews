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

Page {
    id: changelogPage

    property string _RICHTEXT_STYLESHEET_PREAMBLE: "<html><style>a { text-decoration: none; color: '" + Theme.secondaryHighlightColor + "' }</style><body>";
    property string _RICHTEXT_STYLESHEET_APPENDIX: "</body></html>";

    property alias model: changelogList.model
    property string bugTrackerBase: ""

    SilicaListView {
        id: changelogList
        anchors.fill: parent
        anchors.bottomMargin: Theme.paddingLarge

        header: Column {
            id: headerContainer
            width: changelogPage.width

            PageHeader { title: qsTr("Changelog") }

            Row {
                width: parent.width

                Column {
                    width: parent.width/4

                    Image {
                        width: 32; height: 32;
                        anchors.horizontalCenter: parent.horizontalCenter
                        source: "image://theme/icon-m-add"
                    }

                    Label {
                        width: parent.width
                        truncationMode: TruncationMode.Fade
                        color: Theme.secondaryColor
                        font.pixelSize: Theme.fontSizeExtraSmall
                        horizontalAlignment: Text.AlignHCenter
                        text: qsTr("New")
                    }
                }

                Column {
                    width: parent.width/4

                    Image {
                        width: 32; height: 32;
                        anchors.horizontalCenter: parent.horizontalCenter
                        source: "image://theme/icon-m-favorite"
                    }

                    Label {
                        width: parent.width
                        truncationMode: TruncationMode.Fade
                        color: Theme.secondaryColor
                        font.pixelSize: Theme.fontSizeExtraSmall
                        horizontalAlignment: Text.AlignHCenter
                        text: qsTr("Improved")
                    }
                }

                Column {
                    width: parent.width/4

                    Image {
                        width: 32; height: 32;
                        anchors.horizontalCenter: parent.horizontalCenter
                        source: "image://theme/icon-m-crash-reporter"
                    }

                    Label {
                        width: parent.width
                        truncationMode: TruncationMode.Fade
                        color: Theme.secondaryColor
                        font.pixelSize: Theme.fontSizeExtraSmall
                        horizontalAlignment: Text.AlignHCenter
                        text: qsTr("Fixed")
                    }
                }

                Column {
                    width: parent.width/4

                    Image {
                        width: 32; height: 32;
                        anchors.horizontalCenter: parent.horizontalCenter
                        source: "image://theme/icon-s-high-importance"
                    }

                    Label {
                        width: parent.width
                        truncationMode: TruncationMode.Fade
                        color: Theme.secondaryColor
                        font.pixelSize: Theme.fontSizeExtraSmall
                        horizontalAlignment: Text.AlignHCenter
                        text: qsTr("Note")
                    }
                }
            }
        }

        VerticalScrollDecorator {}

        delegate: ChangelogDelegate { trackerUrl: changelogPage.bugTrackerBase }
    }
}


