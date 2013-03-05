import QtQuick 1.1
import Components 1.0

Item {
    id: menuItem
    property string text
    property variant currentMenu

    function activate() {
        if (typeof currentMenu === "undefined")
            currentMenu = menu.createObject(root)
        else
            currentMenu = currentMenu.destroy();
    }

    Component {
        id: menu

        Item {
            anchors.fill: parent

            MouseArea {
                anchors.fill: parent
                onClicked: activate()
            }

            Column {
                focus: true
                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                width: 200; height: menuItem.children.length*50

                Component.onDestruction: focus=false

                Keys.onPressed: {
                    if (event.key == Qt.Key_Backspace || event.key == Qt.Key_Back) {
                        event.accepted = true;
                        menuItem.activate();
                    }
                }

                Repeater {
                    model: menuItem.children

                    Rectangle {
                        border { color: "#444"; width: 1 }
                        color: "#333"
                        width: 200; height: 50

                        Label {
                            anchors.fill: parent
                            anchors.margins: 10
                            verticalAlignment: Qt.AlignVCenter
                            text: menuItem.children[model.index].text
                        }

                        MouseArea {
                            anchors.fill: parent
                            onPressed: {
                                parent.color = root.highlightColor;
                                activateTimer.start();
                            }
                            Timer {
                                id: activateTimer
                                interval: 100; running: false; repeat: false
                                onTriggered: { menuItem.activate(); }
                                Component.onCompleted: triggered.connect(menuItem.children[model.index].triggered)
                            }
                        }
                    }
                }
            }
        }
    }
}
