import QtQuick 1.1

Rectangle {
    property alias text: label.text
    property bool when: true

    id: message
    clip: true
    border { width: 1; color: Qt.rgba(0,0,0, .3) }
    height: when ? label.height + 10 : 0; width: parent.width
    visible: when
    radius: 5

    Rectangle {
        anchors.fill: parent

        gradient: Gradient {
            GradientStop { position: 0.0; color: Qt.rgba(255,255,255, .1) }
            GradientStop { position: 1.0; color: Qt.rgba(0,0,0, .1) }
        }
    }

    states: [
        State {
            name: "error"
            PropertyChanges { target: message; color: "#e99" }
        },
        State {
            name: "info"
            PropertyChanges { target: message; color: "#F2E3A0" }
        }
    ]

    Label {
        id: label
        anchors.verticalCenter: parent.verticalCenter
        color: Qt.rgba(0,0,0, .5)
        font.bold: true
        x: 5
    }
}
