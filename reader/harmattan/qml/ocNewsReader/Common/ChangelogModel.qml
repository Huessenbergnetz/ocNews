// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

ListModel {
    ListElement {
        version: "1.9.5"
        time: 1432645932000
        text: "<ul>
                <li>New: optional direct opening in Browser</li>
               </ul>"
    }

    ListElement {
        version: "1.9.4"
        time: 1417168815000
        text: "<ul>
                <li>New: implement QsLog to create log files</li>
                <li>New: maintenance option to delete all ocNews accounts</li>
                <li>Improved: translation updates</li>
               </ul>"
    }

    ListElement {
        version : "1.9.3"
        time: 1415812273000
        text: "<ul>
                <li>Improved: translation updates</li>
               </ul>"
    }

    ListElement {
        version : "1.9.2"
        time: 1404318720000
        text: "<ul>
                <li>Fixed: also react on text changes when rescaling rich text, fixing issue #52</li>
                <li>Fixed: fixing bugs in configuration system, should also fix issue #54</li>
               </ul>"
    }

    ListElement {
        version: "1.9.1"
        time: 1404255600000
        text: "<ul>
                <li>Changed: show All Articles entry even when there are no unread items and hide feeds and folders is set to true</li>
                <li>Changed: use threaded model population for normal itemsmodel, too</li>
                <li>Fixed: fix return of NULL value in database query for total unread count (should fix issue #49)</li>
               </ul>"
    }

    ListElement {
        version: "1.9.0"
        time: 1404077599000
        text: "<ul>
                <li>New: optional show article excerpts in list views (issue #41)<li>
                <li>New: optional notify about new items, also notify about errors on SailfishOS, notifications are now part of the background daemon (issue #38)</li>
                <li>New: startup info</li>
                <li>Changed: changelog layout<li>
                <li>Changed: simplified wording for some context menus in list views (issue #43)</li>
                <li>Improved: code cleanups (issue #40)</li>
                <li>Improved: data models (issue #44)</li>
                <li>Fixed: local unread count was not updated after moving a feed (issue #46)</li>
              </ul>"
    }

    ListElement {
        version: "1.8.2"
        time: 1399932292000
        text: "<ul>
                <li>Improved: about page design</li>
                <li>Fixed: send user authentication directly in the request to adopt to latest News App changes, fixes issue #39<li>
               </ul>"
    }

    ListElement {
        version: "1.8.1"
        time: 1399132800000
        text: "<ul>
                <li>New: option for hiding read feeds and folders (issue #37)</li>
                <li>New: rename feeds</li>
                <li>New: partly danish translation by flywheeldk</li>
                <li>Fixed: broken russian translation for SelectionItem (issue #36)</li>
               </ul>"
    }

    ListElement {
        version: "1.8.0"
        time: 1394195811000
        text: "<ul>
                <li>New: user selectable font size</li>
                <li>New: navigate between previous and next item through toolbar icons</li>
                <li>New: changelog page</li>
                <li>New: partly russian translation by palacex</li>
                <li>Changed: open in web browser is now a button in toolbar</li>
               </ul>"
    }

    ListElement {
        version: "1.7.2"
        time: 1392815460000
        text: "<ul>
                <li>Fixed: image link opener was showing the same image every time after loading one</li>
               </ul>"
    }

    ListElement {
        version: "1.7.1"
        time: 1392676560000
        text: "<ul>
                <li>New: add option to shut down engine together with reader app</li>
                <li>New: translation updates</li>
                <li>Changed: get image file type by magic bytes</li>
                <li>Fixed: remoe css classes from item body HTML text (issue #31)</li>
               </ul>"
    }

    ListElement {
        version: "1.7.0"
        time: 1392234780000
        text: "<ul>
                <li>New: display and handle enclosures (attached files) (issue #28)</li>
                <li>New: option to download and cache images at updating (issue #16)</li>
                <li>New: dialog for opening links (issue #12)</li>
                <li>Fixed: resize pictures on item view (taken from Martin Grimme’s Tidings RSS reader) (issue #11)</li>
               </ul>"
    }

    ListElement {
        version: "1.6.1"
        time: 1391129580000
        text: "<ul>
                <li>Fixed: feed list view was not refreshed after complete sync update</li>
                <li>Fixed: minor stuff in translations</li>
               </ul>"
    }

    ListElement {
        version: "1.6.0"
        time: 1391018338000
        text: "<ul>
                <li>NEW: second mode for the main view, choose between folders or all feeds (issue #10)</li>
                <li>NEW: the daemon is now reacting to network (only on Harmattan) and config changes (on both) (issue #17)</li>
                <li>NEW: translations for Spanish (by Mariano Flotron) and Chinese(China) (by finalmix and gwmgdemj)</li>
                <li>Changed: nicer contributors page (issue #18)</li>
                <li>Changed: better indication for feeds and folders with no unread articles (issue #23)</li>
               </ul>"
    }

    ListElement {
        version: "1.5.2"
        time: 1390571400000
        text: "<ul>
                <li>NEW: french translation by Antoine Vacher aka. tigre-bleu</li>
                <li>NEW: contributors page</li>
                <li>Changed: use a centralized property to indicate if an operaton is running</li>
               </ul>"
    }

    ListElement {
        version: "1.5.1"
        time: 1390309200000
        text: "<ul><li>Changed: check for non existing database tables on every daemon start</li></ul>"
    }

    ListElement {
        version: "1.5.0"
        time: 1390172400000
        text: "<ul>
                <li>NEW: it is now possible to set a default sort order for the feed item list, as well to decide how already read news items should be displayes (show, hide, show after unread)</li>
                <li>Fixed: searching in feed item list is now working again</li>
                <li>Fixed: speed up marking all items read</li>
               </ul>"
    }

    ListElement {
        version: "1.4.1"
        time: 1389912720000
        text: "<ul>
                <li>Fixed: when daemon was crashed without notice and account was deleted in account manager it could happen, that it was not possible to create a new accout</li>
                <li>Fixed: clicking on the launcher icon is not activating an already running ocNews instance</li>
               </ul>"
    }

    ListElement {
        version: "1.4.0"
        time: 1389722460000
        text: "<ul>
                <li>First version after Nokia Store release</li>
                <li>Changed: do not start daemon at system start but on D-Bus request</li>
                <li>NEW: option for shutting both completely down, reader and daemon/engine</li>
               </ul>"
    }
}
