import QtQuick 1.1
import com.nokia.meego 1.0

import "../Common"
import "../Sheets"

Page {
    id: about

    tools: aboutTools

    orientationLock: PageOrientation.LockPortrait

    property string _RICHTEXT_STYLESHEET_PREAMBLE: "<html><style>a { text-decoration: none; color: '" + theme.selectionColor + "' }</style><body>";
    property string _RICHTEXT_STYLESHEET_APPENDIX: "</body></html>";


    // ------------- Header Start ----------------

        Header {
            id: aboutPageHeader
            text: qsTr("About")
        }

    // ------------- Header End ----------------

    TabGroup {
        id: tabgroup
        currentTab: aboutTab
        anchors { top: aboutPageHeader.bottom; topMargin: 5 }

        Page {
            id: aboutTab
            orientationLock: PageOrientation.LockPortrait

            Image {
                id: ocnewsLogo
                anchors { top: parent.top; topMargin: 10 }
                asynchronous: true
                sourceSize.width: 540
                sourceSize.height: 270
                source: "/opt/ocNewsReader/images/ocNews-Harmattan-Cover.jpg"
            }

            Label {
                id: labelName
                anchors { horizontalCenter: parent.horizontalCenter; top: ocnewsLogo.bottom; topMargin: 5 }
                textFormat: Text.PlainText
                text: "ocNews " + versionString
            }

            Text {
                id: description
                anchors { horizontalCenter: parent.horizontalCenter; top: labelName.bottom; topMargin: 5 }
                text: qsTr("The ownCloud News App client for Harmattan")
                width: parent.width
                font.pointSize: 17
                font.weight: Font.Light
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignHCenter
                textFormat: Text.PlainText
                color: theme.inverted ? "white" : "black"
            }

            Text {
                id: copyright
                anchors { horizontalCenter: parent.horizontalCenter; top: description.bottom; topMargin: 5 }
                font.pointSize: 17
                font.weight: Font.Light
                text: "© 2013-2014, Buschtrommel"
                textFormat: Text.PlainText
                color: theme.inverted ? "white" : "black"
            }

            Text {
                id: license
                anchors { horizontalCenter: parent.horizontalCenter; top: copyright.bottom; topMargin: 5 }
                font.pointSize: 17
                font.weight: Font.Light
                text: _RICHTEXT_STYLESHEET_PREAMBLE + qsTr("Licensed under the <a href='http://www.gnu.org/licenses/gpl-2.0.en.html'>GNU GPL v2</a>") + _RICHTEXT_STYLESHEET_APPENDIX
                textFormat: Text.RichText
                color: theme.inverted ? "white" : "black"
                onLinkActivated: { Qt.openUrlExternally(link) }
            }

            Button {
                id: privacyPolicyButton
                anchors { horizontalCenter: parent.horizontalCenter; top: license.bottom; topMargin: 30 }
                text: qsTr("Privacy policy")
                onClicked: {
                    aboutPrivacySheet.policy = dbus.getSetting("display/privacypolicy", false) == "true" ? true : false;
                    aboutPrivacySheet.open();
                }
            }

            Button {
                id: websiteLink
                anchors { horizontalCenter: parent.horizontalCenter; top: privacyPolicyButton.bottom; topMargin: 10 }
                text: qsTr("Visit website")
                onClicked: Qt.openUrlExternally("http://ocnews.buschmann23.de")
            }

            Button {
                id: licenseLink
                anchors { horizontalCenter: parent.horizontalCenter; top: websiteLink.bottom; topMargin: 10 }
                text: qsTr("Donate")
                onClicked: Qt.openUrlExternally("https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=RDZAG64WD34PL")
            }
        }

        Page {
            id: contributorsTab
            orientationLock: PageOrientation.LockPortrait

            Flickable {
                id: contributorsContent
                anchors { right: parent.right; rightMargin: 15; left: parent.left; leftMargin: 15; top: parent.top; topMargin: 10; bottomMargin: aboutTools.height }
                width: parent.width
                height: parent.height
                contentHeight: contentCol.height + aboutTools.height + 50
                flickableDirection:  Flickable.VerticalFlick


                Column {
                    id: contentCol
                    spacing: 10
                    width: parent.width

                    GroupHeader {
                        text: qsTr("Upstream author")
                    }

                    ContributorItem {
                        name: "Buschmann (Matthias Fehring)"
                        role: qsTr("Main developer, ocNews creator")
                        image: "buschmann.png"
                        website: "http://www.buschmann23.de"
                        twitter: "buschmann23"
                        github: "buschmann23"
                    }

                    GroupHeader {
                        text: qsTr("Major contributors")
                    }

                    ContributorItem {
                        name: "Tigre-Bleu (Antoine Vacher)"
                        role: qsTr("UI, French translation")
                        image: "tigre-bleu.png"
                        twitter: "tigre_bleu"
                    }

                    GroupHeader {
                        text: qsTr("Thanks to")
                    }

                    ContributorItem {
                        name: "Finalmix (太空飞瓜)"
                        role: qsTr("Chinese (China) translation")
                        image: "finalmix.png"
                        weibo: "finalmix"
                    }

                    ContributorItem {
                        name: "Flywheeldk (Peter Jespersen)"
                        role: qsTr("Danish translation")
                        image: "flywheeldk.png"
                        website: "http://illogical.dk/"
                        twitter: "myflywheel"
                        linkedin: "pjespersen"
                        bitbucket: "flywheel"
                    }

                    ContributorItem {
                        name: "Jean Clovis"
                        role: qsTr("French translation")
                        github: "jean-clovis"
                        image: "jean-clovis.png"
                    }

                    ContributorItem {
                        name: "Lorenzo Facca"
                        role: qsTr("Italian translation")
                        twitter: "LorenzoFacca"
                        website: "http://jollacommunity.altervista.org/"
                    }

                    ContributorItem {
                        name: "Mariano Flotron"
                        role: qsTr("Spanish translation")
                    }

                    ContributorItem {
                        name: "Palacex"
                        role: qsTr("Russian translation")
                    }

                    GroupHeader {
                        text: qsTr("Used third party elements")
                    }

                    ContributorItem {
                        name: qsTr("Martin Grimme's Tidings Elements")
                        role: "RescalingRichText"
                        website: "https://github.com/pycage/tidings"
                        github: "pycage"
                    }
                }
            }

            ScrollDecorator {
                flickableItem: contributorsContent
                anchors { right: parent.right; rightMargin: -contributorsContent.anchors.rightMargin }
            }
        }

        Page {
            id: changelogTab
            orientationLock: PageOrientation.LockPortrait


            Flickable {
                id: changelogContent
                anchors { right: parent.right; rightMargin: 15; left: parent.left; leftMargin: 15; top: parent.top; topMargin: 10; bottomMargin: aboutTools.height }
                width: parent.width
                height: parent.height
                contentHeight: changelogCol.height + aboutTools.height + 50
                flickableDirection:  Flickable.VerticalFlick


                Column {
                    id: changelogCol
                    spacing: 0
                    width: parent.width
                    move: Transition { NumberAnimation { properties: "y"; easing.type: Easing.InOutQuad } }

                    CLItem {
                        version: "1.8.3"
                        time: 1399932292000
                        text: "<ul>
                                <li>Improved: code cleanups</li>
                               </ul>"
                    }

                    CLItem {
                        version: "1.8.2"
                        time: 1399932292000
                        text: "<ul>
                                <li>Improved: about page design</li>
                                <li>Fixed: send user authentication directly in the request to adopt to latest News App changes, fixes issue #39<li>
                               </ul>"
                    }

                    CLItem {
                        version: "1.8.1"
                        time: 1399132800000
                        text: "<ul>
                                <li>New: option for hiding read feeds and folders (issue #37)</li>
                                <li>New: rename feeds</li>
                                <li>New: partly danish translation by flywheeldk</li>
                                <li>Fixed: broken russian translation for SelectionItem (issue #36)</li>
                               </ul>"
                    }

                    CLItem {
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

                    CLItem {
                        version: "1.7.2"
                        time: 1392815460000
                        text: "<ul>
                                <li>Fixed: image link opener was showing the same image every time after loading one</li>
                               </ul>"
                    }

                    CLItem {
                        version: "1.7.1"
                        time: 1392676560000
                        text: "<ul>
                                <li>New: add option to shut down engine together with reader app</li>
                                <li>New: translation updates</li>
                                <li>Changed: get image file type by magic bytes</li>
                                <li>Fixed: remoe css classes from item body HTML text (issue #31)</li>
                               </ul>"
                    }

                    CLItem {
                        version: "1.7.0"
                        time: 1392234780000
                        text: "<ul>
                                <li>New: display and handle enclosures (attached files) (issue #28)</li>
                                <li>New: option to download and cache images at updating (issue #16)</li>
                                <li>New: dialog for opening links (issue #12)</li>
                                <li>Fixed: resize pictures on item view (taken from Martin Grimme’s Tidings RSS reader) (issue #11)</li>
                               </ul>"
                    }

                    CLItem {
                        version: "1.6.1"
                        time: 1391129580000
                        text: "<ul>
                                <li>Fixed: feed list view was not refreshed after complete sync update</li>
                                <li>Fixed: minor stuff in translations</li>
                               </ul>"
                    }

                    CLItem {
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

                    CLItem {
                        version: "1.5.2"
                        time: 1390571400000
                        text: "<ul>
                                <li>NEW: french translation by Antoine Vacher aka. tigre-bleu</li>
                                <li>NEW: contributors page</li>
                                <li>Changed: use a centralized property to indicate if an operaton is running</li>
                               </ul>"
                    }

                    CLItem {
                        version: "1.5.1"
                        time: 1390309200000
                        text: "<ul><li>Changed: check for non existing database tables on every daemon start</li></ul>"
                    }

                    CLItem {
                        version: "1.5.0"
                        time: 1390172400000
                        text: "<ul>
                                <li>NEW: it is now possible to set a default sort order for the feed item list, as well to decide how already read news items should be displayes (show, hide, show after unread)</li>
                                <li>Fixed: searching in feed item list is now working again</li>
                                <li>Fixed: speed up marking all items read</li>
                               </ul>"
                    }

                    CLItem {
                        version: "1.4.1"
                        time: 1389912720000
                        text: "<ul>
                                <li>Fixed: when daemon was crashed without notice and account was deleted in account manager it could happen, that it was not possible to create a new accout</li>
                                <li>Fixed: clicking on the launcher icon is not activating an already running ocNews instance</li>
                               </ul>"
                    }

                    CLItem {
                        version: "1.4.0"
                        time: 1389722460000
                        text: "<ul>
                                <li>First version after Nokia Store release</li>
                                <li>Changed: do not start daemon at system start but on D-Bus request</li>
                                <li>NEW: option for shutting both completely down, reader and daemon/engine</li>
                               </ul>"
                    }

                }
            }

            ScrollDecorator {
                flickableItem: changelogContent
                anchors { right: parent.right; rightMargin: -changelogContent.anchors.rightMargin }
            }

        }
    }


    // ----------------- Toolbar Start --------------------

        ToolBarLayout {
            id: aboutTools
            visible: true
            ToolIcon {
                platformIconId: "toolbar-back"
                anchors.left: (parent === undefined) ? undefined : parent.left
                onClicked: { pageStack.pop(); }
            }
            ButtonRow {
                style: TabButtonStyle { }
                TabButton {
                    iconSource: "image://theme/icon-m-toolbar-application"
                    tab: aboutTab
                    onClicked: aboutPageHeader.text = qsTr("About")
                }
                TabButton {
                    iconSource: "image://theme/icon-m-toolbar-contact"
                    tab: contributorsTab
                    onClicked: aboutPageHeader.text = qsTr("Contributors")
                }
                TabButton {
                    iconSource: "image://theme/icon-m-toolbar-list"
                    tab: changelogTab
                    onClicked: aboutPageHeader.text = qsTr("Changelog")
                }
            }
        }
    // ----------------- Toolbar End --------------------


    PrivacySheet {
        id: aboutPrivacySheet
    }

}
