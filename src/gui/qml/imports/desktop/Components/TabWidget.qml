import QtQuick 1.1
import Components 1.0

AbstractTabWidget {
    id: tabWidget

    position: "west";

    header: Item {
        id: header

        width: column.width; height: tabWidget.height

        Rectangle {
            anchors.centerIn: parent
            height: parent.width
            width: parent.height
            rotation: 270
            gradient: Gradient {
                GradientStop { position: 0; color: Qt.rgba(0,0,0, 0) }
                GradientStop { position: 1; color: Qt.rgba(0,0,0, .1) }
            }
        }

        Column {
            id: column
            width: childrenRect.width

            Repeater {
                model: tabWidget.content.length
                delegate: Rectangle {
                    color: tabWidget.current == index ? Qt.rgba(255,255,255, .2) : "transparent"
                    width: label.implicitHeight + 20; height: label.implicitWidth + 20

                    Text {
                        id: label
                        anchors.centerIn: parent
                        rotation: 270
                        text: tabWidget.content[index].title
                        elide: Text.ElideRight
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
