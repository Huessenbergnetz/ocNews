import QtQuick 2.2
import Sailfish.Silica 1.0
import "../BTComponents"
import "../Models"

AboutPage {

    appTitle: "ocNews"
    appVersion: versionString
    appDescription: qsTr("The ownCloud News App client for Sailfish OS")
    appHomepage: "https://www.buschmann23.de/entwicklung/anwendungen/ocnews/"
    appCover: "/usr/share/harbour-ocnews-reader/icons/ocNews-Sailfish-Cover.jpg"
    appCopyright: "© 2013-2014, Buschtrommel"
    appLicense: qsTr("Licensed under the <a href='http://www.gnu.org/licenses/gpl-2.0.en.html'>GNU GPL v2</a>")

    changelogModel: ChangelogModel {}
    bugTrackerBaseUrl: "https://github.com/Buschtrommel/ocNews/issues/"

    contributorsAvatarBasePath: "/usr/share/harbour-ocnews-reader/icons/contributors"
    contributorsModel: ContributorsModel {}

    privacyPolicyQmlFile: "../Dialogs/PrivacyPolicy.qml"

    contactCompany: "Buschtrommel"
    contactName: "Matthias Fehring"
    contactStreet: "Engelhardstraße"
    contactHouseNo: "32"
    contactZIP: "81369"
    contactCity: qsTr("Munich")
    contactCountry: qsTr("Germany")
    contactEmail: "kontakt@buschmann23.de"
    contactWebsite: "www.buschmann23.de"

    paypalLabel: qsTr("Donate via PayPal")
    paypalEmail: "kontakt@buschmann23.de"
    paypalOrganization: "Buschtrommel"
    paypalItem: "ocNews"
    paypalMessage: qsTr("Leave a message (English or German):")

    bugUrl: "https://github.com/Buschtrommel/ocNews/issues"
    translateUrl: "https://www.transifex.com/projects/p/ocnews/"

    licensesModel: ListModel {
        ListElement { text: QT_TR_NOOP("<a href='https://github.com/pycage/tidings'>RescalingRichtext</a> by Martin Grimme is licensed under the terms of the <a href='http://www.gnu.org/licenses/gpl-2.0.en.html'>GNU GPL v2</a>.") }
        ListElement { text: QT_TR_NOOP("<a href='https://bitbucket.org/razvanpetru/qslog/'>QsLog</a> by razvanpetru is licensed under the terms of the <a href='https://bitbucket.org/razvanpetru/qslog/src'>New BSD license</a>.") }
    }
}
