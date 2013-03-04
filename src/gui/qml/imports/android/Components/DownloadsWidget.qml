import QtQuick 1.1
//import Components 1.0

BarredListView {
    id: tree

    property variant status: ["Ej startad", "Laddar ner", "Färdig", "Fel uppstod", "Abruten"]

    anchors.fill: parent

    delegate: Item {
        height: childrenRect.height+20; width: parent.width
        Column {
            width: parent.width
            y: 10
            spacing: 5

            Text {
                color: "white"
                width: parent.width
                text: model.fileName
            }
            Text {
                color: "white"
                width: parent.width
                text: status[model["statusCol"]]
            }
            Item {
                width: parent.width; height: childrenRect.height
                Text {
                    id: sizeLabel
                    color: "white"
                    text: model.downloadedSize + " Mb"
                }
                Rectangle {
                    anchors { bottom: sizeLabel.bottom; left: sizeLabel.right; right: progressLabel.left; margins: 2; leftMargin: 10; rightMargin: 10 }
                    color: "#444"
                    height: 3
                    Rectangle {
                        color: "#0099cc"
                        width: parent.width*model.progressCol/100; height: parent.height
                    }
                }
                Text {
                    id: progressLabel
                    anchors.right: parent.right
                    color: "white"
                    text: model.progressCol + "%"
                }
            }
            Rectangle {
                color: "#272727"
                height: 1; width: parent.width
            }
        }
    }
}
