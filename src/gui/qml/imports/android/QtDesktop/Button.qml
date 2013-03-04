import QtQuick 1.1

Rectangle {
    id: button

    property alias text: label.text
    property bool enabled: true

    signal clicked

    clip: true

    color: "#444"

    height: 30; width: label.width + 20

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

    Rectangle {
        anchors.bottom: parent.bottom
        width: parent.width; height: 2
        color: "#333"
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: button.clicked()
    }
}
