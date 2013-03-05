import QtQuick 1.1
import Components 1.0
import QtDesktop 0.1

Item {
    property variant model
    property bool expanded: false
    property int selectedIndex: 0

    id: comboBox
    implicitWidth: label.implicitWidth+btn.width; height: 20

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
        onClicked: drop.createObject(root)
    }

    Component {
        id: drop

        Window {
            id: _drop
            anchors.fill: parent

            color: "transparent"

            Rectangle {
                property int delegateHeight: comboBox.height

                id: dropRect
                anchors.centerIn: parent
                border { width: 2; color: "#333" }
                width: root.width-50
                height: model.count*(delegateHeight+list.spacing-1)+list.anchors.margins
                color: "#444"

                focus: true

                ListView {
                    id: list
                    anchors.fill: parent
                    anchors.margins: 20
                    spacing: 20
                    model: comboBox.model

                    onCurrentIndexChanged: comboBox.selectedIndex = currentIndex
                    currentIndex: comboBox.selectedIndex

                    delegate: CheckBox {
                        checked: model.index == list.currentIndex
                        width: dropRect.width; height: dropRect.delegateHeight
                        text: model.text

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                mouse.accepted = true;
                                list.currentIndex = model.index;
                                destructionTimer.start();
                            }
                            Timer {
                                id: destructionTimer
                                interval: 100; running: false; repeat: false
                                onTriggered: { _drop.destroy(); }
                            }
                        }
                    }
                }
            }
        }
    }
}
