import QtQuick 1.1
import QtDesktop 0.1

Item {
    id: bar
    anchors { margins: 5; left: parent.left; right: parent.right; top: parent.top }
    height: 30;

    Button {
        id: backButton
        height: parent.height
        text: "Bakåt"
        enabled: loader.history.length > 0
        onClicked: loader.goBack()
    }
    Button {
        id: favButton
        anchors.left: backButton.right
        height: parent.height
        text: "Lägg till favoriter"
        enabled: backButton.enabled
        width: 0
    }
    TextField {
        id: addressField
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: favButton.right
        anchors.right: fetchButton.left
        height: parent.height
        //text: "http://www.svtplay.se/video/1032053/del-9-av-12"

        Text {
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            color: "darkgray"
            font.italic: true
            text: "Ange adress manuellt"
            visible: addressField.text == ""// || !addressField.activeFocus
        }
    }
    Button {
        id: fetchButton
        anchors.right: parent.right
        enabled: addressField.text != ""
        height: parent.height
        text: "Hämta"

        onClicked: loader.go(Qt.resolvedUrl("pirateplay.qml?rnd=" + Math.random()), { url: addressField.text }, 0)
    }
}
