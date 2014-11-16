/*
    Framrekkari - Transifex Client for SailfishOS
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

BackgroundItem {
    id: root

    property string label
    property string textChoosen
    property string choosenValue
    property bool addNotSpecified: false
    property bool highlightError: false

    property int _duration: 200

    width: parent ? parent.width : 0
    height: contentItem.height
    contentHeight: visible ? (titleText.height == flow.implicitHeight ? Theme.itemSizeSmall : Theme.itemSizeExtraLarge) : 0
    opacity: enabled ? 1.0 : 0.4

    Flow {
        id: flow

        anchors {
            left: parent.left; right: parent.right; verticalCenter: parent.verticalCenter
            leftMargin: Theme.paddingLarge; rightMargin: Theme.paddingLarge
        }

        move: Transition { NumberAnimation { properties: "x,y"; easing.type: Easing.InOutQuad; duration: root._duration } }

        Label {
            id: titleText
            text: root.label
            width: Math.min(implicitWidth + Theme.paddingSmall, parent.width)
            color: root.down ? Theme.highlightColor : highlightError ? "red" : Theme.primaryColor
            truncationMode: TruncationMode.Fade
        }

        Label {
            id: choosenText
            text: root.textChoosen
            color: highlightError ? "red" : Theme.highlightColor
            width: Math.min(implicitWidth, parent.width)
            truncationMode: TruncationMode.Fade
        }

    }

    onClicked: {
        var dialog = pageStack.push(dialogComp, {chosenValue: root.choosenValue, textChosen: root.textChoosen, label: root.label})
        dialog.accepted.connect(function() {root.choosenValue = dialog.chosenValue; root.textChoosen = dialog.textChosen})
    }


    Component {
        id: dialogComp
        Page {
            id: dialogRoot

            signal accepted()

            property string chosenValue
            property string textChosen
            property alias label: pHeader.title
            property alias listModel: list.model

            PageHeader { id: pHeader }

            SearchField {
                id: search
                width: parent.width
                anchors { top: pHeader.bottom; left: parent.left; right: parent.right }
                placeholderText: qsTr("Search")
                EnterKey.iconSource: "image://theme/icon-m-enter-close"
                EnterKey.onClicked: search.focus = false
                onTextChanged: list.model.search = text
            }

            ListView {
                id: list
                anchors { top: search.bottom; bottom: parent.bottom; left: parent.left; right: parent.right }
                contentWidth: parent.width
                clip: true
                model: languageModel

                VerticalScrollDecorator {}

                delegate: ListItem {
                    id: listItem
                    width: parent.width
                    height: Theme.itemSizeSmall

                    ListView.onAdd: AddAnimation { target: listItem }
                    ListView.onRemove: animateRemoval(listItem)

                    highlighted: model.value === dialogRoot.chosenValue

                    Label {
                        id: signLabel
                        anchors { left: parent.left; leftMargin: Theme.paddingLarge; right: parent.right; rightMargin: Theme.paddingLarge; verticalCenter: parent.verticalCenter }
                        text: Theme.highlightText(model.name, search.text, Theme.highlightColor)
                    }

                    onClicked: {
                        dialogRoot.chosenValue = model.value
                        dialogRoot.textChosen = model.name
                        dialogRoot.accepted()
                        search.text = ""
                        search.focus = false
                        pageStack.pop()
                    }
                }
            }
        }
    }
}
