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
    id: root
    anchors.fill: parent
    z: 1000

    signal clicked()

    property string version
    property string name
    property string description
    property string helpPage
    property string donationHeader
    property string donationText
    property string paypalOrganization
    property string paypalItem
    property string paypalEmail
    property string paypalMessage
    property string paypalLabel

    property ListModel changeLogModel: null
    property string changeLogTracker

    Loader {
        anchors.fill: parent
        sourceComponent: parent.visible ? info : null
    }

    Component {
        id: info

        Rectangle {
            color: Theme.rgba("black", 0.8)
            anchors.fill: parent

            property string _RICHTEXT_STYLESHEET_PREAMBLE: "<html><style>a { text-decoration: none; color: '" + Theme.secondaryHighlightColor + "' }</style><body>";
            property string _RICHTEXT_STYLESHEET_APPENDIX: "</body></html>";

            SilicaFlickable {
                id: infoFlick
                anchors.fill: parent
                contentHeight: infoCol.height + Theme.paddingLarge

                VerticalScrollDecorator {}

                Column {
                    id: infoCol
                    anchors { left: parent.left; right: parent.right; top: parent.top; topMargin: Theme.paddingLarge }

                    Label {
                        id: title
                        anchors { left: parent.left; right: parent.right; leftMargin: Theme.paddingLarge; rightMargin: Theme.paddingLarge }
                        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                        text: qsTr("Welcome to %1 %2", "First is app name, second is version number").arg(root.name).arg(root.version)
                        color: Theme.highlightColor
                    }

                    Text {
                        id: description
                        anchors { left: parent.left; right: parent.right; leftMargin: Theme.paddingLarge; rightMargin: Theme.paddingLarge }
                        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                        textFormat: Text.RichText
                        color: Theme.primaryColor
                        font.pixelSize: Theme.fontSizeSmall
                        visible: root.description !== ""
                        text: _RICHTEXT_STYLESHEET_PREAMBLE + root.description + _RICHTEXT_STYLESHEET_APPENDIX
                    }

                    SectionHeader { text: qsTr("Last changes"); visible: clRepeater.count > 0 }

                    Repeater {
                        id: clRepeater
                        model: changeLogModel.get(0).entries
                        anchors { left: parent.left; right: parent.right; leftMargin: Theme.paddingLarge; rightMargin: Theme.paddingLarge }

                        Item {
                            anchors { left: parent.left; right: parent.right; leftMargin: Theme.paddingLarge; rightMargin: Theme.paddingLarge }
                            height: desc.height

                            property string issueUrl: (model.issue && root.changeLogTracker) ? " (<a href='" + root.changeLogTracker + model.issue +"'>#" + model.issue + "</a>)" : ""

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

                    SectionHeader { text: root.donationHeader; visible: root.donationHeader }

                    Text {
                        anchors { left: parent.left; leftMargin: Theme.paddingLarge; right: parent.right; rightMargin: Theme.paddingLarge }
                        textFormat: Text.PlainText
                        color: Theme.primaryColor
                        wrapMode: Text.WordWrap
                        font.pixelSize: Theme.fontSizeSmall
                        onLinkActivated: { Qt.openUrlExternally(link) }
                        text: root.donationText
                        visible: root.donationText
                    }

                    PaypalChooser {
                        id: donation
                        organization: root.paypalOrganization
                        item: root.paypalItem
                        email: root.paypalEmail
                        message: root.paypalMessage
                        visible: root.paypalEmail
                        label: root.paypalLabel
                    }


                    Row {
                        anchors { left: parent.left; right: parent.right; leftMargin: Theme.paddingLarge; rightMargin: Theme.paddingLarge }


                        Button {
                            text: qsTr("Help")
                            visible: root.helpPage !== ""
                            onClicked: { root.clicked(); root.visible = false; pageStack.push(Qt.resolvedUrl(root.helpPage)) }
                        }

                        Button {
                            text: qsTr("Close")
                            onClicked: { root.clicked(); root.visible = false }
                        }
                    }
                }
            }
        }
    }
}
