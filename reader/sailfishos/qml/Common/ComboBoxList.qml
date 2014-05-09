import QtQuick 2.0
import Sailfish.Silica 1.0


//ComboBox {
//    property string initialValue
//    property alias model: repeater.model

//    function getInitialValue() {
//        var found = false;
//        var i = 0;
//        while ((!found) && (i < model.count)) {
//            if (repeater.model.get(i).value == initialValue) {
//                box.currentIndex = i;
//                found = true;
//            }
//            i++;
//        }
//    }

//    Component.onCompleted: timer.start()

//    id: box
//    menu: ContextMenu {
//          Repeater {
//               id: repeater
//               MenuItem { text: model.name }
//          }
//    }

//    Timer {
//        id: timer
//        interval: 500
//        repeat: false
//        triggeredOnStart: false
//        onTriggered: getInitialValue()
//    }
//}


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
