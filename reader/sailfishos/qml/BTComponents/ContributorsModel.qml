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

ListModel {
    ListElement {
        name: "Buschmann (Matthias Fehring)"
        role: QT_TR_NOOP("Main developer, ocNews creator")
        section: QT_TR_NOOP("Author")
        image: "buschmann.png"
        website: "http://www.buschmann23.de"
        twitter: "buschmann23"
        github: "buschmann23"
    }


    ListElement {
        name: "Tigre-Bleu (Antoine Vacher)"
        role: QT_TR_NOOP("UI, French translation")
        section: QT_TR_NOOP("Major contributors")
        image: "tigre-bleu.png"
        twitter: "tigre_bleu"
    }

    ListElement {
        name: "Finalmix (太空飞瓜)"
        role: QT_TR_NOOP("Chinese (China) translation")
        section: QT_TR_NOOP("Thanks to")
        image: "finalmix.png"
        weibo: "finalmix"
    }

    ListElement {
        name: "Flywheeldk (Peter Jespersen)"
        role: QT_TR_NOOP("Danish translation")
        section: QT_TR_NOOP("Thanks to")
        image: "flywheeldk.png"
        website: "http://illogical.dk/"
        twitter: "myflywheel"
        linkedin: "pjespersen"
        bitbucket: "flywheel"
    }

    ListElement {
        name: "Jean Clovis"
        role: QT_TR_NOOP("French translation")
        section: QT_TR_NOOP("Thanks to")
        image: "jean-clovis.png"
        github: "jean-clovis"
    }

    ListElement {
        name: "Lorenzo Facca"
        role: QT_TR_NOOP("Italian translation")
        section: QT_TR_NOOP("Thanks to")
        twitter: "LorenzoFacca"
        website: "http://jollacommunity.altervista.org/"
    }

    ListElement {
        name: "Mariano Flotron"
        role: QT_TR_NOOP("Spanish translation")
        section: QT_TR_NOOP("Thanks to")
    }

    ListElement {
        name: QT_TR_NOOP("Martin Grimme's Tidings Elements")
        section: QT_TR_NOOP("Thanks to")
        role: "RescalingRichText"
        website: "https://github.com/pycage/tidings"
        github: "pycage"
    }
}
