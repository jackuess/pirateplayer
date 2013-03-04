import QtQuick 1.1

Rectangle {
    id: message

    property alias text: label.text

    states: [
        State {
            name: "error"
            PropertyChanges { target: message; color: "#e99"; border.color: "#a77" }
            PropertyChanges { target: label; color: "#900" }
        },
        State {
            name: "info"
            PropertyChanges { target: message; color: "#ee9"; border.color: "#aa7" }
            PropertyChanges { target: label; color: "#990" }
        }
    ]

    border.width: 1
    height: label.height + 10; width: parent.width
    radius: 5

    Text {
        id: label
        anchors.verticalCenter: parent.verticalCenter
        font.bold: true
        x: 5
    }
}
