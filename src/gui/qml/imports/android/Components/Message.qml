import QtQuick 1.1

AbstractMessage {
    id: message

    textColor: Qt.rgba(0,0,0, .7)

    states: [
        State {
            name: "error"
            PropertyChanges { target: message; color: "#B13535" }
        },
        State {
            name: "info"
            PropertyChanges { target: message; color: root.highlightColor }
        }
    ]
}
