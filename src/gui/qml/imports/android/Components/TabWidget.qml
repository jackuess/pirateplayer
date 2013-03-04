import QtQuick 1.1
import Components 1.0

AbstractTabWidget {
    id: tabWidget

    position: "north"

    header: Item {
        id: header

        width: tabWidget.width; height: 60

        Row {
            width: parent.width; height: parent.height

            Repeater {
                model: tabWidget.content.length
                delegate: Rectangle {
                    color: "transparent"
                    width: parent.width/tabWidget.content.length; height: parent.height

                    Label {
                        id: label
                        width: parent.width; height: parent.height
                        horizontalAlignment: Qt.AlignHCenter; verticalAlignment: Qt.AlignVCenter
                        text: tabWidget.content[index].title
                        elide: Text.ElideRight
                        font.bold: tabWidget.current == index
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: tabWidget.current = index
                    }
                    Rectangle {
                        anchors.bottom: parent.bottom
                        color: "#0099cc"
                        width: parent.width; height: tabWidget.current == index ? 5 : 2
                    }
                }
            }
        }
    }
}
