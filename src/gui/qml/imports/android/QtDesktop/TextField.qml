import QtQuick 1.1

Item {
    property alias text: input.text
    property alias validator: input.validator

    height: 25

    Component.onDestruction: root.forceActiveFocus()

    TextInput {
        id: input

        anchors.bottom: parent.bottom
        anchors.margins: 3
        width: parent.width
        color: "white"
    }

    Rectangle {
        anchors.bottom: parent.bottom
        color: root.highlightColor
        width: parent.width; height: 1
    }
}
