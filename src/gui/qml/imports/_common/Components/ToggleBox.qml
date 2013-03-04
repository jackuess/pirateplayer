import QtQuick 1.1

Column {
    default property alias children: items.children
    property string label

    id: box
    spacing: 10

    Item {
        property bool show: false

        id: toggle
        width: parent.width; height: showLabel.height

        Rectangle {
            color: Qt.rgba(0,0,0, .05)
            anchors { left: parent.left; right: showLabel.left; verticalCenter: parent.verticalCenter; margins: 5 }
            height: 1
        }
        Rectangle {
            color: Qt.rgba(0,0,0, .05)
            anchors { left: showLabel.right; right: parent.right; verticalCenter: parent.verticalCenter; margins: 5 }
            height: 1
        }

        Text {
            id: showLabel
            anchors.horizontalCenter: parent.horizontalCenter
            text: (toggle.show ? "Dölj " : "Visa ") + box.label
        }

        MouseArea {
            anchors.fill: parent
            onClicked: toggle.show = !toggle.show
        }
    }

    Item {
        id: items
        width: parent.width; height: childrenRect.height
        visible: toggle.show
    }
}
