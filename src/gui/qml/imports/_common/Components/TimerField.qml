import QtQuick 1.1
import Components 1.0
import QtDesktop 0.1

Row {
    id: timeField
    property int ms
    property alias enabled: hours.enabled

    Binding { target: timeField; property: "ms"; value: (parseInt(hours.text)*60+parseInt(minutes.text))*60000 }

    TextField {
        id: hours
        width: 40
        validator: IntValidator { bottom: 0 }
        text: "0"
    }
    Label { anchors.verticalCenter: hours.verticalCenter; text: "timmar" }
    TextField {
        id: minutes
        enabled: hours.enabled
        width: 40
        validator: IntValidator { bottom: 0; top: 59 }
        text: "0"
    }
    Label { anchors.verticalCenter: minutes.verticalCenter; text: "minuter" }
}
