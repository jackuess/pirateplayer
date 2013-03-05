import QtQuick 1.1

Rectangle {
    id: window

    //anchors.fill: parent
    color: "#333"
    property string title: ""
    property int modality

    Component.onDestruction: {
        forceActiveFocus();
        focus = false;
    }

    Keys.onPressed: {
        if (event.key == Qt.Key_Backspace || event.key == Qt.Key_Back) {
            event.accepted = true;
            window.destroy();
        }
    }
    Keys.onMenuPressed: ;

    MouseArea {
        anchors.fill: parent
        onClicked: mouse.accepted = true
    }
}
