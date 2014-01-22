/* Copyright (C) 2013 Martin Grimme <martin.grimme _AT_ gmail.com>
*
* This file was apapted from Tidings
* Copyright (C) 2013 Martin Grimme <martin.grimme _AT_ gmail.com>
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/

import QtQuick 2.0
import Sailfish.Silica 1.0

Item {
    property SilicaListView flickable: parent
    property int threshold: 500
    property bool _activeUp
    property bool _activeDown

    BackgroundItem {
        visible: opacity > 0
        y: 0
        width: flickable.width
        height: Theme.itemSizeLarge
        highlighted: pressed
        opacity: _activeUp ? 1 : 0

        Behavior on opacity {
            NumberAnimation { duration: 300; easing.type: Easing.InOutQuad }
        }

        Image {
            anchors.right: parent.right
            anchors.rightMargin: Theme.paddingLarge
            anchors.verticalCenter: parent.verticalCenter
            source: "image://theme/icon-l-up"

        }

        onPressed: {
            flickable.cancelFlick();
            flickable.scrollToTop();
        }
    }

    BackgroundItem {
        visible: opacity > 0
        y: flickable.height - height
        width: flickable.width
        height: Theme.itemSizeLarge
        highlighted: pressed
        opacity: _activeDown ? 1 : 0

        Behavior on opacity {
            NumberAnimation { duration: 300; easing.type: Easing.InOutQuad }
        }

        Image {
            anchors.right: parent.right
            anchors.rightMargin: Theme.paddingLarge
            anchors.verticalCenter: parent.verticalCenter
            source: "image://theme/icon-l-down"

        }

        onPressed: {
            flickable.cancelFlick();
            flickable.scrollToBottom();
        }
    }

    Connections {
        target: flickable

        onVerticalVelocityChanged: {
            //console.log("velocity: " + target.verticalVelocity);

            if (target.verticalVelocity < 0)
            {
                _activeDown = false;
            }
            else
            {
                _activeUp = false;
            }

            if (target.verticalVelocity < -threshold &&
                target.contentHeight > 3 * target.height)
            {
                _activeUp = true;
                _activeDown = false;
            }
            else if (target.verticalVelocity > threshold &&
                     target.contentHeight > 3 * target.height)
            {
                _activeUp = false;
                _activeDown = true;
            }
            else if (Math.abs(target.verticalVelocity) < 10)
            {
                _activeUp = false;
                _activeDown = false;
            }
        }
    }
}
