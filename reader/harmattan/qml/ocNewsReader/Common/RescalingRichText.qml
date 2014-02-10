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

import QtQuick 1.1

/* Pretty fancy element for displaying rich text fitting the width.
 *
 * Images are scaled down to fit the width, or, technically speaking, the
 * rich text content is actually scaled down so the images fit, while the
 * font size is scaled up to keep the original font size.
 */

Item {
    id: root

    property string text
    property alias color: contentText.color
    property real fontSize: 17
    property string _RICHTEXT_STYLESHEET_PREAMBLE: "<html><style>a { text-decoration: none; color: '" + theme.selectionColor + "' }</style><body>";
    property string _RICHTEXT_STYLESHEET_APPENDIX: "</body></html>";

    property real scaling: 1
    property bool isScaled

    signal linkActivated(string link)

    height: contentText.height * scaling
    clip: true

    onWidthChanged: {
        isScaled = false;
        if (useRichText) rescaleTimer.restart();
    }

    Text {
        id: contentText

        width: parent.width / scaling
        scale: scaling

        transformOrigin: Item.TopLeft
        font.pointSize: parent.fontSize / scaling
        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
        textFormat: useRichText ? Text.RichText : Text.StyledText
        smooth: true
        font.weight: Font.Light

        text: useRichText ? _RICHTEXT_STYLESHEET_PREAMBLE + parent.text + _RICHTEXT_STYLESHEET_APPENDIX : parent.text

        onPaintedWidthChanged: {
            console.log("contentWidth: " + paintedWidth);
            if (useRichText) rescaleTimer.restart();
        }

        onLinkActivated: {
            root.linkActivated(link);
        }
    }

    Timer {
        id: rescaleTimer
        interval: 100

        onTriggered: {
            var paintedWidth = Math.floor(contentText.paintedWidth);
            if (! isScaled)
            {
                isScaled = true;
                scaling = parent.width / (contentText.paintedWidth + 0.0);
                console.log("scaling: " + scaling);
            }
        }
    }
}
