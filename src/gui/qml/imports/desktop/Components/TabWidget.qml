import QtQuick 1.1
import Components 1.0

AbstractTabWidget {
    id: tabWidget

    position: "west"

    header: Item {
        id: header

        width: 160; height: parent.height

        Rectangle {
            anchors.centerIn: parent
            height: parent.width
            width: parent.height
            rotation: 270
            gradient: Gradient {
                GradientStop { position: 0; color: Qt.rgba(0,0,0, .2) }
                GradientStop { position: 1; color: Qt.rgba(0,0,0, 0) }
            }
        }

        Column {
            width: parent.width

            Repeater {
                model: tabWidget.content.length
                delegate: Rectangle {
                    color: tabWidget.current == index ? Qt.rgba(0,0,0, .1) : "transparent"

                    width: parent.width; height: 36

                    Text {
                        id: label
                        horizontalAlignment: Qt.AlignHCenter; verticalAlignment: Qt.AlignVCenter
                        anchors.fill: parent
                        text: tabWidget.content[index].title
                        elide: Text.ElideRight
                        font.bold: tabWidget.current == index
                    }
                    Rectangle {
                        anchors.bottom: parent.bottom
                        width: parent.width; height: model.index+1==tabWidget.content.length ? 0 : 1
                        color: Qt.rgba(0,0,0, .05)
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: tabWidget.current = index
                    }
                }
            }
        }
    }
}


//import QtQuick 1.0

//Item {
//    id: tabWidget

//    default property alias content: stack.children

//    property int current: 0
//    property int count: stack.children.length

//    onCurrentChanged: setOpacities()
//    Component.onCompleted: setOpacities()

//    Keys.onPressed: {
//        if (event.key == Qt.Key_PageUp && event.modifiers == Qt.ControlModifier) {
//            current = (current-1).mod(count);
//            event.accepted = true;
//        }
//        else if (event.key == Qt.Key_PageDown && event.modifiers == Qt.ControlModifier) {
//            current = (current+1)%count;
//            event.accepted = true;
//        }
//    }

//    function setOpacities() {
//        for (var i = 0; i < stack.children.length; ++i) {
//            stack.children[i].opacity = (i == current ? 1 : 0)
//            stack.children[i].focus = i == current;
//        }
//    }

//    Item {
//        id: header

//        width: 160; height: parent.height

//        Rectangle {
//            anchors.centerIn: parent
//            height: parent.width
//            width: parent.height
//            rotation: 270
//            gradient: Gradient {
//                GradientStop { position: 0; color: Qt.rgba(0,0,0, .2) }
//                GradientStop { position: 1; color: Qt.rgba(0,0,0, 0) }
//            }
//        }

//        Column {
//            width: parent.width

//            Repeater {
//                model: stack.children.length
//                delegate: Rectangle {
//                    color: tabWidget.current == index ? Qt.rgba(0,0,0, .1) : "transparent"

//                    width: parent.width; height: 36

//                    Text {
//                        id: label
//                        horizontalAlignment: Qt.AlignHCenter; verticalAlignment: Qt.AlignVCenter
//                        anchors.fill: parent
//                        text: stack.children[index].title
//                        elide: Text.ElideRight
//                        font.bold: tabWidget.current == index
//                    }
//                    Rectangle {
//                        anchors.bottom: parent.bottom
//                        width: parent.width; height: model.index+1==stack.children.length ? 0 : 1
//                        color: Qt.rgba(0,0,0, .05)
//                    }

//                    MouseArea {
//                        anchors.fill: parent
//                        onClicked: tabWidget.current = index
//                    }
//                }
//            }
//        }
//    }

//    Item {
//        id: stack
//        height: tabWidget.height
//        anchors.left: header.right; anchors.right: tabWidget.right
//    }
//}
