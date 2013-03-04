import QtQuick 1.1
import Components 1.0

Item {
    property variant model
    property bool expanded: false
    property int selectedIndex: 0

    id: comboBox
    implicitWidth: label.implicitWidth+btn.width; height: 20

    Component.onCompleted: {
        drop.createObject(root, { });
    }

    Label {
        id: label
        width: parent.width-btn.width
        text: model.get(selectedIndex).text
    }

    Rectangle {
        anchors { bottom: label.bottom; left: label.left; right: btn.right }
        height: 1
        color: root.highlightColor
    }

    Rectangle {
        id: btn
        anchors.left: label.right
        anchors.bottom: label.bottom
        anchors.leftMargin: 5
        clip: true
        color: "transparent"
        width: Math.sqrt(Math.pow(label.height, 2)*2)/2
        height: width
        Rectangle {
            color: root.highlightColor
            x: 2; y: 2
            width: label.height; height: label.height
            rotation: 45
        }
    }

    MouseArea {
        width: parent.childrenRect.width; height: parent.childrenRect.height
        onClicked: comboBox.expanded = !comboBox.expanded;
    }

    Component {
        id: drop

        Rectangle {
            id: dropRect

            property int delegateHeight: comboBox.height

            anchors.centerIn: root
            border { width: 2; color: "#333" }
            width: comboBox.expanded ? root.width-50 : 0
            height: comboBox.expanded ? model.count*(delegateHeight+spacing-1)+list.anchors.margins : 0
            color: "#444"
            opacity: comboBox.expanded ? 1 : 0

            ListView {
                id: list
                anchors.fill: parent
                anchors.margins: 20
                spacing: 20
                model: comboBox.model

                onCurrentIndexChanged: comboBox.selectedIndex = currentIndex

                delegate: CheckBox {
                    checked: model.index == list.currentIndex
                    width: dropRect.width; height: delegateHeight
                    text: model.text

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            list.currentIndex = model.index;
                            comboBox.expanded = false;
                        }
                    }
                }
            }
        }
    }
}
