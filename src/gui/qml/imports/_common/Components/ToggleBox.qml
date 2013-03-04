import QtQuick 1.1
import Components 1.0

Column {
    default property alias children: items.children
    property string label

    id: box
    spacing: 10

    Item {
        property bool show: false
        property color lineColor: mobile() ? Qt.rgba(255,255,255, .3) : Qt.rgba(0,0,0, .05)

        id: toggle
        width: parent.width; height: showLabel.height

        Rectangle {
            color: parent.lineColor
            anchors { left: parent.left; right: showLabel.left; verticalCenter: parent.verticalCenter; margins: 5 }
            height: 1
        }
        Rectangle {
            color: parent.lineColor
            anchors { left: showLabel.right; right: parent.right; verticalCenter: parent.verticalCenter; margins: 5 }
            height: 1
        }

        Label {
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
