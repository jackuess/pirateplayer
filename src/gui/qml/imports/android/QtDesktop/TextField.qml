import QtQuick 1.1

FocusScope {
    property alias text: input.text

    height: 25

    TextInput {
        id: input

        anchors.bottom: parent.bottom
        anchors.margins: 3
        focus: true
        width: parent.width
        color: "white"
    }

    Rectangle {
        anchors.bottom: parent.bottom
        color: root.highlightColor
        width: parent.width; height: 1
    }
}
