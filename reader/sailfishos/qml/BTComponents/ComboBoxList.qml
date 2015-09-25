/*
    BTComponents - QML components shared between various Buschtrommel projects
    Copyright (C) 2014  Buschtrommel/Matthias Fehring
    Contact: Matthias Fehring (Buschmann) <kontakt@buschmann23.de>

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


ComboBox {
    property string initialValue
    property string choosenValue
    property alias model: repeater.model

    function getInitialValue() {
        var found = false;
        var i = 0;
        while ((!found) && (i < model.count)) {
            if (repeater.model.get(i).value == initialValue) {
                box.currentIndex = i;
                found = true;
            }
            i++;
        }
    }

    function setChoosenValue()
    {
        var choosen = repeater.model.get(box.currentIndex).value;

        choosenValue = choosen;

        if (choosen !== initialValue) {
//            initialValue = choosen;
//            choosenValue = choosen;
        }
    }

    Component.onCompleted: { choosenValue = initialValue; timer.start() }

//    onCurrentIndexChanged: if (currentIndex > -1) setChoosenValue()
    onCurrentIndexChanged: {
        _currentIndexSet = true
        if (_completed && !_updating) {
            _updating = true
            _updateCurrent(currentIndex, null)
            _updating = false
        }
        if (currentIndex > -1) setChoosenValue()
    }

    id: box
    menu: ContextMenu {
          Repeater {
               id: repeater
               MenuItem { text: model.name; visible: (model.enabled) ? model.enabled : true }
          }
    }

    Timer {
        id: timer
        interval: 500
        repeat: false
        triggeredOnStart: false
        onTriggered: getInitialValue()
    }
}
