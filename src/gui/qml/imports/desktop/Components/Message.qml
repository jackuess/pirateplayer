import QtQuick 1.1

AbstractMessage {
    id: message

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
}
