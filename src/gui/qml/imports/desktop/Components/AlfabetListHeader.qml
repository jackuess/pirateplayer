import QtQuick 1.1

Rectangle {
    id: header

    property variant target
    property string alfabet: ""
    property variant alfabetHash: {}

    Keys.onPressed: if (typeof alfabetHash[event.text.toUpperCase()] !== "undefined") target.currentIndex = alfabetHash[event.text.toUpperCase()]

    anchors.horizontalCenter: parent.horizontalCenter
    color: "#555"
    //radius: 5
    height: row.height + row.spacing*2; width: parent.width
    Row {
        id: row
        anchors.horizontalCenter: parent.horizontalCenter; anchors.verticalCenter: parent.verticalCenter
        spacing: 10
        Repeater {
            model: header.alfabet.length
            Rectangle {
                height: label.height; width: label.width
                color: "transparent"
                Text {
                    id: label
                    color: target.currentSection == text ? "#0099cc" : "#ddd"
                    text: header.alfabet[index]
                    font.bold: true
                }
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: target.currentIndex = alfabetHash[label.text]
                    onEntered: label.font.underline = true
                    onExited: label.font.underline = false
                }
            }
        }
    }

    Connections {
        target: if (header.target.model) header.target.model
        onStatusChanged: {
            var model = header.target.model;
            if (model.status == XmlListModel.Ready) {
                var letters = {}
                var alfabetString = ""
                for (var i=0; i<model.count; i++) {
                    var c = model.get(i)[header.target.section.property][0];
                    if (typeof letters[c] === "undefined") {
                        letters[c.toUpperCase()] = i;
                        alfabetString += c;
                    }
                }
                header.alfabet = alfabetString;
                header.alfabetHash = letters;
            }
        }
    }
}
