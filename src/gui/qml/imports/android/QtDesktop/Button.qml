import QtQuick 1.1

Rectangle {
    id: button

    property alias text: label.text
    property bool enabled: true

    signal clicked

    border { color: "#444"; width: 1 }
    clip: true
    color: "#333"
    height: 35; width: label.width + 20

    states: [
        State {
            name: "pressed"
            when: mouseArea.pressed && button.enabled
            PropertyChanges { target: button; color: root.highlightColor }
        },
        State {
            name: "dissabled"
            when: !button.enabled
            PropertyChanges { target: label; color: "gray" }
            PropertyChanges { target: mouseArea; onClicked: undefined }
        }
    ]

    Text {
        id: label

        anchors.horizontalCenter: parent.horizontalCenter; anchors.verticalCenter: parent.verticalCenter
        color: "white"
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: button.clicked()
    }
}
