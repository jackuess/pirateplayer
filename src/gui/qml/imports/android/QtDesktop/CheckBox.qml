import QtQuick 1.1
import Components 1.0

Item {
    id: checkBox

    property bool checked: false
    property alias text: label.text

    width: childrenRect.width; height: childrenRect.height

    Row {
        spacing: 10
        Rectangle {
            anchors.verticalCenter: label.verticalCenter
            width: 15; height: 15
            border { width: 1; color: "#555" }
            color: "transparent"

            Rectangle {
                //anchors.fill: parent
                width: parent.width-7; height: parent.height-7
                anchors.centerIn: parent
                //anchors.margins: 2
                color: checkBox.checked ? root.highlightColor : "transparent"
            }
        }
        Label {
            id: label
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: checkBox.checked = !checkBox.checked
    }
}
