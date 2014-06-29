import QtQuick 1.1
import com.nokia.meego 1.0

Item {
    id: root
    height: donation.height
    anchors { left: parent.left; right: parent.right }

    property string email
    property string organization
    property string item
    property string message
    property string url
    property alias color: donation.titleColor

    function openDonation(currency)
    {
        var lang = "";
        var loc = new String(locale);
        var cc = loc.slice(3, 5);

        console.log(loc)
        console.log(cc)

        switch (loc) {
        case "da_DK":
        case "he_IL":
        case "id_ID":
        case "ja_JP":
        case "no_NO":
        case "pt_BR":
        case "ru_RU":
        case "sv_SE":
        case "th_TH":
        case "tr_TR":
        case "zh_CN":
        case "zh_HK":
        case "zh_TW":
            lang = loc;
            break;
        default:
            lang = "";
            break;
        }

        if (lang === "") {
            switch(cc) {
            case "AU":
            case "AT":
            case "BE":
            case "BR":
            case "CA":
            case "CH":
            case "CN":
            case "DE":
            case "ES":
            case "GB":
            case "FR":
            case "IT":
            case "NL":
            case "PL":
            case "PT":
            case "RU":
                lang = cc;
                break;
            default:
                lang = "US";
                break;
            }
        }

        if (currency !== "") {
            url = "https://www.paypal.com/cgi-bin/webscr?cmd=_donations&business=" + email +"&lc=" + lang +"&item_name="+ organization +"&item_number=" + item +"&no_note=0&cn="+ message + "&no_shipping=1&currency_code="+ currency +"&bn=PP%2dDonationsBF%3abtn_donate_LG%2egif%3aNonHosted"

            Qt.openUrlExternally(encodeURI(url))
        }

    }

    ListModel {
        id: donationModel
        ListElement { name: ""; value: "" }
        ListElement { name: ""; value: "EUR" }
        ListElement { name: ""; value: "USD" }
        ListElement { name: ""; value: "AUD" }
        ListElement { name: ""; value: "BRL" }
        ListElement { name: ""; value: "GBP" }
        ListElement { name: ""; value: "DKK" }
        ListElement { name: ""; value: "HKD" }
        ListElement { name: ""; value: "ILS" }
        ListElement { name: ""; value: "JPY" }
        ListElement { name: ""; value: "CAD" }
        ListElement { name: ""; value: "MXN" }
        ListElement { name: ""; value: "TWD" }
        ListElement { name: ""; value: "NZD" }
        ListElement { name: ""; value: "NOK" }
        ListElement { name: ""; value: "PHP" }
        ListElement { name: ""; value: "PLN" }
        ListElement { name: ""; value: "RUB" }
        ListElement { name: ""; value: "SEK" }
        ListElement { name: ""; value: "CHF" }
        ListElement { name: ""; value: "SGD" }
        ListElement { name: ""; value: "THB" }
        ListElement { name: ""; value: "CZK" }
        ListElement { name: ""; value: "HUF" }
        Component.onCompleted: {
            donationModel.get(0).name = qsTr("Currency")
            donationModel.get(1).name = qsTr("Euro")
            donationModel.get(2).name = qsTr("US Dollar")
            donationModel.get(3).name = qsTr("Australian Dollar")
            donationModel.get(4).name = qsTr("Brazilian Real")
            donationModel.get(5).name = qsTr("Pound Sterling")
            donationModel.get(6).name = qsTr("Danish Krone")
            donationModel.get(7).name = qsTr("Hong Kong Dollar")
            donationModel.get(8).name = qsTr("Israel New Shekel")
            donationModel.get(9).name = qsTr("Japanese Yen")
            donationModel.get(10).name = qsTr("Canadian Dollar")
            donationModel.get(11).name = qsTr("Mexican Peso")
            donationModel.get(12).name = qsTr("New Taiwan Dollar")
            donationModel.get(13).name = qsTr("New Zealand Dollar")
            donationModel.get(14).name = qsTr("Norwegian Krone")
            donationModel.get(15).name = qsTr("Philippine Peso")
            donationModel.get(16).name = qsTr("Polish Zloty")
            donationModel.get(17).name = qsTr("Russian Ruble")
            donationModel.get(18).name = qsTr("Swedish Krone")
            donationModel.get(19).name = qsTr("Swiss Franc")
            donationModel.get(20).name = qsTr("Singapore Dollar")
            donationModel.get(21).name = qsTr("Thai Baht")
            donationModel.get(22).name = qsTr("Czech Koruna")
            donationModel.get(23).name = qsTr("Hungarian Forint")
        }
    }

    SelectionItem {
        id: donation
        title: qsTr("Donate via PayPal")
        width: parent.width - 40
        anchors.horizontalCenter: parent.horizontalCenter
        model: donationModel
        initialValue: ""
        onCurrentValueChanged: openDonation(currentValue)
    }
}
