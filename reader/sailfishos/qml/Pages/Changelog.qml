import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: about

    property string _RICHTEXT_STYLESHEET_PREAMBLE: "<html><style>a { text-decoration: none; color: '" + Theme.secondaryHighlightColor + "' }</style><body>";
    property string _RICHTEXT_STYLESHEET_APPENDIX: "</body></html>";

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: aboutCol.height + Theme.paddingLarge
        VerticalScrollDecorator {}

        Column {
            id: aboutCol
            anchors { left: parent.left; right: parent.right }
            PageHeader { title: qsTr("Changelog") }
            move: Transition { NumberAnimation { properties: "y"; easing.type: Easing.InOutQuad } }
            add: Transition { AddAnimation {} }

            ClHeader {
                version: "1.8.2"
                time: 1399932292000
                onExpand: v182.visible = !v182.visible
            }

            ClEntry {
                id: v182
                text: "<ul>
                        <li>Improved: about page design</li>
                        <li>Improved: donation currency chooser</li>
                        <li>Fixed: send user authentication directly in the request to adopt to latest News App changes, fixes issue #39<li>
                      </ul>"
            }

            ClHeader {
                version: "1.8.1"
                time: 1399132800000
                onExpand: v181.visible = !v181.visible
            }

            ClEntry {
                id: v181
                text: "<ul>
                        <li>New: option for hiding read feeds and folders (issue #37)</li>
                        <li>New: rename feeds</li>
                        <li>New: partly danish translation by flywheeldk</li>
                        <li>Improved: dim count bubble of feeds without unread entries when viewing a folder</li>
                      </ul>"
            }

            ClHeader {
                version: "1.8.0"
                time: 1394195811000
                onExpand: v180.visible = !v180.visible
            }

            ClEntry {
                id: v180
                text: "<ul>
                        <li>New: user selectable font size</li>
                        <li>New: navigate between previous and next item through pulley menus</li>
                        <li>New: navigate between items on cover</li>
                        <li>New: changelog page</li>
                        <li>Changed: change count bubble color according to item count</li>
                        <li>Changed: improve web view</li>
                      </ul>"
            }

            ClHeader {
                version: "1.7.2"
                time: 1392815460000
                onExpand: v172.visible = !v172.visible
            }

            ClEntry {
                id: v172
                text: "<ul>
                        <li>Fixed: reader crasehs when loading certain news (issue #31)</li>
                        <li>Changed: improved web preview of items (should load mobile site if available)</li>
                      </ul>"
            }

            ClHeader {
                version: "1.7.1"
                time: 1392676560000
                onExpand: v171.visible = !v171.visible
            }

            ClEntry {
                id: v171
                text: "<ul>
                        <li>New: add option to shut down engine together with reader app</li>
                        <li>New: translation updates</li>
                        <li>Changed: get image file type by magic bytes</li>
                        <li>Fixed: remoe css classes from item body HTML text (issue #31)</li>
                        <li>Fixed: listen and react to network access changes (issue #25)</li>
                    </ul>"
            }

            ClHeader {
                version: "1.7.0"
                time: 1392234780000
                onExpand: v170.visible = !v170.visible
            }

            ClEntry {
                id: v170
                text: "<ul>
                        <li>New: display and handle enclosures (attached files) (issue #28)</li>
                        <li>New: option to download and cache images at updating (issue #16)</li>
                        <li>New: dialog for opening links (issue #12)</li>
                        <li>Fixed: resize pictures on item view (taken from Martin Grimme’s Tidings RSS reader) (issue #11)</li>
                        <li>Fixed: search field was losing focus (issue #4)</li>
                        <li>Fixed: updateFinished signal was sent before stats got updated (issue #30)</li>
                        <li>Fixed: adding new feed from cover action did not activate busy state (issue #29)</li>
                        <li>Fixed: update label on cover page is not pulsing during refresh (issue #27)</li>
                    </ul>"
            }

            ClHeader {
                version: "1.6.1"
                time: 1391129487000
                onExpand: v161.visible = !v161.visible
            }

            ClEntry {
                id: v161
                text: "<ul>
                        <li>Fixed: all posts view not refreshed after complete sync update (issue #26)</li>
                        <li>Fixed: feed list view was not refreshed after complete sync update</li>
                        <li>Fixed: minor stuff in translations</li>
                      </ul>"
            }

            ClHeader {
                version: "1.6.0"
                time: 1391018338000
                onExpand: v160.visible = !v160.visible
            }

            ClEntry {
                id: v160
                text: "<ul>
                        <li>NEW: second mode for the main view, choose between folders or all feeds (issue #10)</li>
                        <li>NEW: the daemon is now reacting to network (only on Harmattan) and config changes (on both) (issue #17)</li>
                        <li>NEW: display last refresh date in pulley menu (issu #9)</li>
                        <li>NEW: translations for Spanish (by Mariano Flotron), Italian (by Lorenzo Facca) and Chinese(China) (by finalmix and gwmgdemj)</li>
                        <li>Changed: nicer contributors page (issue #18)</li>
                        <li>Changed: better indication for feeds and folders with no unread articles (issue #23)</li>
                        <li>Fixed: unread count on cover is not updated when the app is displayed (issue #13)</li>
                        <li>Fixed: if title contains an ampersand, the part after it was not shown (issue #22)</li>
                        <li>Fixed: on hiding virtual keyboard, docked sorting panel was shown shortly (issue #24)</li>
                    </ul>"
            }

            ClHeader {
                version: "1.5.2"
                time: 1390571400000
                onExpand: v152.visible = !v152.visible
            }

            ClEntry {
                id: v152
                text: "<ul>
                        <li>NEW: french translation by Antoine Vacher aka. tigre-bleu</li>
                        <li>NEW: contributors page</li>
                        <li>Changed: use a centralized property to indicate if an operaton is running</li>
                    </ul>"
            }

            ClHeader {
                version: "1.5.1"
                time: 1390309200000
                onExpand: v151.visible = !v151.visible
            }

            ClEntry {
                id: v151
                text: "<ul>
                        <li>NEW: add maintenance options (delete database, reset configuration)</li>
                        <li>Changed: check for non existing database tables on every daemon start</li>
                    </ul>"
            }


            ClHeader {
                version: "1.5.0"
                time: 1390172400000
                onExpand: v150.visible = !v150.visible
            }

            ClEntry {
                id: v150
                text: "<ul>
                        <li>NEW: it is now possible to set a default sort order for the feed item list, as well to decide how already read news items should be displayes (show, hide, show after unread)</li>
                        <li>Fixed: speed up marking all items read</li>
                    </ul>"
            }

            ClHeader {
                version: "1.4.1"
                time: 1389912720000
                onExpand: v141.visible = !v141.visible
            }

            ClEntry {
                id: v141
                text: "<ul>
                        <li>NEW: added Martin Grimmes FancyScroller from Tidings</li>
                        <li>Changed: Quit in main menu now shuts down reader and engine</li>
                        <li>Fixed: file permissions in Sailfish OS RPM</li>
                        <li>Fixed: open in browser button in item view push up menu is now working</li>
                        <li>Fixed: minor UI fixes</li>
                        <li>Fixed: busy indicator was running on already read posts in special list view</li>
                    </ul>"
            }

            ClHeader {
                version: "1.4.0"
                time: 1389722460000
                onExpand: v140.visible = !v140.visible
            }

            ClEntry {
                id: v140
                text: "<ul><li>initial port to Sailfish OS</li></ul>"
            }
        }
    }
}
