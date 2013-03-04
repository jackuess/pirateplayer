import QtQuick 1.1
import Components 1.0

Label {
    id: statusMessage

    property variant target

    anchors.verticalCenter: parent.verticalCenter; anchors.horizontalCenter: parent.horizontalCenter
    font.pointSize: 14

    states: [
        State {
            name: "NotingFound"
            when: statusMessage.target.status == XmlListModel.Ready && statusMessage.target.count < 1 && statusMessage.target.source != ""
            PropertyChanges { target: statusMessage; text: "Ingenting funnet" }
        },
        State {
            name: "Loading"
            when: statusMessage.target.status == XmlListModel.Loading || typeof statusMessage.target.source == ""
            PropertyChanges { target: statusMessage; text: "Laddar..." }
        },
        State {
            name: "Ready"
            when: statusMessage.target.status == XmlListModel.Ready
            PropertyChanges { target: statusMessage; visible: false }
        },
        State {
            name: "Error"
            when: statusMessage.target.status == XmlListModel.Error
            PropertyChanges { target: statusMessage; text: "Ett fel uppstod!" }
        }
    ]
}
