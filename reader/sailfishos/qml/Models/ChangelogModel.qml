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

/*
  Types:
  0 - New
  1 - Improved/Enhanced
  2 - Fixed
  3 - Note
*/

import QtQuick 2.2

ListModel {
    ListElement {
        version: "1.9.5"
        date: 1432645932000
        entries: [
            ListElement { type: 0; issue: "51"; description: "optional direct opening in WebView or Browser" },
            ListElement { type: 2; issue: "65"; description: "crash on opening news item" }
        ]
    }

    ListElement {
        version: "1.9.4"
        date: 1416161271000
        entries: [
            ListElement { type: 0; issue: ""; description: "implement QsLog to create log files" },
            ListElement { type: 0; issue: ""; description: "add application language chooser to settings" },
            ListElement { type: 1; issue: ""; description: "updating BTComponents (About page, Changelog, Contributors, etc.)" },
            ListElement { type: 1; issue: ""; description: "translation updates" }
        ]
    }

    ListElement {
        version : "1.9.3"
        date: 1415812273000
        entries: [
            ListElement { type: 0; issue: ""; description: "partly Dutch (Netherlands) translation by Heimen Stoffels" },
            ListElement { type: 1; issue: "58"; description: "SailfishOS Update 9 compatibility" },
            ListElement { type: 1; issue: ""; description: "translation updates" }
        ]
    }

    ListElement {
        version : "1.9.2"
        date: 1404318720000
        entries: [
            ListElement { type: 2; issue: "52"; description: "also react on text changes when rescaling rich text" },
            ListElement { type: 2; issue: "54"; description: "fixing bugs in configuration system, should also fix issue" }
        ]
    }

    ListElement {
        version: "1.9.1"
        date: 1404255600000
        entries: [
            ListElement { type: 1; issue: ""; description: "show All Articles entry even when there are no unread items and hide feeds and folders is set to true" },
            ListElement { type: 1; issue: ""; description: "use threaded model population for normal itemsmodel, too" },
            ListElement { type: 2; issue: "49"; description: "fix return of NULL value in database query for total unread count" },
            ListElement { type: 2; issue: "47"; description: "when scaling rich text, first calculate the scaling, then set the text" },
            ListElement { type: 2; issue: ""; description: "add forgotten connections between items signal markedAllItemsReadSuccess to models, to fix not updated models" },
            ListElement { type: 2; issue: "5"; description: "use nemo-keepalive for reliable update interval" }
        ]
    }
    ListElement {
        version: "1.9.0"
        date: 1404077599000
        entries: [
            ListElement { type: 0; issue: "41"; description: "optional show article excerpts in list views" },
            ListElement { type: 0; issue: "38"; description: "optional notify about new items, also notify about errors on SailfishOS, notifications are now part of the background daemon" },
            ListElement { type: 1; issue: ""; description: "changelog layout" },
            ListElement { type: 1; issue: ""; description: "removed Martine Grimme's FancyScroller, using native scroller instead" },
            ListElement { type: 1; issue: "43"; description: "simplified wording for some context menus in list views" },
            ListElement { type: 1; issue: "40"; description: "code cleanups" },
            ListElement { type: 1; issue: "44"; description: "data models" },
            ListElement { type: 2; issue: "46"; description: "local unread count was not updated after moving a feed" }
        ]
    }

    ListElement {
        version: "1.8.2"
        date: 1399932292000
        entries: [
            ListElement { type: 1; issue: ""; description: "about page design" },
            ListElement { type: 1; issue: ""; description: "donation currency chooser" },
            ListElement { type: 2; issue: "39"; description: "send user authentication directly in the request to adopt to latest News App changes" }
        ]
    }

    ListElement {
        version: "1.8.1"
        date: 1399132800000
        entries: [
            ListElement { type: 0; issue: "37"; description: "option for hiding read feeds and folders" },
            ListElement { type: 0; issue: ""; description: "rename feeds" },
            ListElement { type: 0; issue: ""; description: "partly danish translation by flywheeldk" },
            ListElement { type: 1; issue: ""; description: "dim count bubble of feeds without unread entries when viewing a folder" }
        ]
    }

    ListElement {
        version: "1.8.0"
        date: 1394195811000
        entries: [
            ListElement { type: 0; issue: ""; description: "user selectable font size" },
            ListElement { type: 0; issue: ""; description: "navigate between previous and next item through pulley menus" },
            ListElement { type: 0; issue: ""; description: "navigate between items on cover" },
            ListElement { type: 0; issue: ""; description: "changelog page" },
            ListElement { type: 1; issue: ""; description: "change count bubble color according to item count" },
            ListElement { type: 1; issue: ""; description: "improve web view" }
        ]
    }

    ListElement {
        version: "1.7.2"
        date: 1392815460000
        entries: [
            ListElement { type: 1; issue: ""; description: "improved web preview of items (should load mobile site if available)" },
            ListElement { type: 2; issue: "31"; description: "reader crasehs when loading certain news" }
        ]
    }

    ListElement {
        version: "1.7.1"
        date: 1392676560000
        entries: [
            ListElement { type: 0; issue: ""; description: "add option to shut down engine together with reader app" },
            ListElement { type: 1; issue: ""; description: "ranslation updates" },
            ListElement { type: 1; issue: ""; description: "get image file type by magic bytes" },
            ListElement { type: 2; issue: "31"; description: "remove css classes from item body HTML text" },
            ListElement { type: 2; issue: "25"; description: "listen and react to network access changes" }
        ]
    }

    ListElement {
        version: "1.7.0"
        date: 1392234780000
        entries: [
            ListElement { type: 0; issue: "28"; description: "display and handle enclosures (attached files)" },
            ListElement { type: 0; issue: "16"; description: "option to download and cache images at updating" },
            ListElement { type: 0; issue: "12"; description: "dialog for opening links" },
            ListElement { type: 2; issue: "11"; description: "resize pictures on item view (taken from Martin Grimme’s Tidings RSS reader)" },
            ListElement { type: 2; issue: "4"; description: "search field was losing focus" },
            ListElement { type: 2; issue: "30"; description: "updateFinished signal was sent before stats got updated" },
            ListElement { type: 2; issue: "29"; description: "adding new feed from cover action did not activate busy state" },
            ListElement { type: 2; issue: "27"; description: "update label on cover page is not pulsing during refresh" }
        ]
    }

    ListElement {
        version: "1.6.1"
        date: 1391129487000
        entries: [
            ListElement { type: 2; issue: "26"; description: "all posts view not refreshed after complete sync update" },
            ListElement { type: 2; issue: ""; description: "feed list view was not refreshed after complete sync update" },
            ListElement { type: 2; issue: ""; description: "minor stuff in translations" }
        ]
    }

    ListElement {
        version: "1.6.0"
        date: 1391018338000
        entries: [
            ListElement { type: 0; issue: "10"; description: "second mode for the main view, choose between folders or all feeds" },
            ListElement { type: 0; issue: "17"; description: "the daemon is now reacting to network (only on Harmattan) and config changes (on both)" },
            ListElement { type: 0; issue: "9"; description: "display last refresh date in pulley menu" },
            ListElement { type: 0; issue: ""; description: "translations for Spanish (by Mariano Flotron), Italian (by Lorenzo Facca) and Chinese(China) (by finalmix and gwmgdemj)" },
            ListElement { type: 1; issue: "18"; description: "nicer contributors page" },
            ListElement { type: 1; issue: "23"; description: "better indication for feeds and folders with no unread articles" },
            ListElement { type: 0; issue: "13"; description: "unread count on cover is not updated when the app is displayed" },
            ListElement { type: 0; issue: "22"; description: "if title contains an ampersand, the part after it was not shown" },
            ListElement { type: 0; issue: "24"; description: "on hiding virtual keyboard, docked sorting panel was shown shortly" }
        ]
    }

    ListElement {
        version: "1.5.2"
        date: 1390571400000
        entries: [
            ListElement { type: 0; issue: ""; description: "french translation by Antoine Vacher aka. tigre-bleu" },
            ListElement { type: 0; issue: ""; description: "contributors page" },
            ListElement { type: 1; issue: ""; description: "use a centralized property to indicate if an operaton is running" }
        ]
    }

    ListElement {
        version: "1.5.1"
        date: 1390309200000
        entries: [
            ListElement { type: 0; issue: ""; description: "add maintenance options (delete database, reset configuration)" },
            ListElement { type: 1; issue: ""; description: "check for non existing database tables on every daemon start" }
        ]
    }


    ListElement {
        version: "1.5.0"
        date: 1390172400000
        entries: [
            ListElement { type: 0; issue: ""; description: "it is now possible to set a default sort order for the feed item list, as well to decide how already read news items should be displayes (show, hide, show after unread)" },
            ListElement { type: 1; issue: ""; description: "speed up marking all items read" }
        ]
    }

    ListElement {
        version: "1.4.1"
        date: 1389912720000
        entries: [
            ListElement { type: 0; issue: ""; description: "dded Martin Grimmes FancyScroller from Tidings" },
            ListElement { type: 1; issue: ""; description: "Quit in main menu now shuts down reader and engine" },
            ListElement { type: 2; issue: ""; description: "file permissions in Sailfish OS RPM" },
            ListElement { type: 2; issue: ""; description: "open in browser button in item view push up menu is now working" },
            ListElement { type: 2; issue: ""; description: "minor UI fixes" },
            ListElement { type: 2; issue: ""; description: "busy indicator was running on already read posts in special list view" }
        ]
    }

    ListElement {
        version: "1.4.0"
        date: 1389722460000
        entries: [
            ListElement { type: 0; issue: ""; description: "initial port to Sailfish OS" }
        ]
    }
}
