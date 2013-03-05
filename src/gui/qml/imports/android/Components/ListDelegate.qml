import QtQuick 1.1
import Components 1.0

AbstractListDelegate {
    id: listDelegate

    Rectangle {
        anchors.bottom: parent.bottom
        width: parent.width; height: 1
        color: Qt.rgba(255,255,255, .15)
    }

    states: State {
        name: "pressed"
        PropertyChanges { target: listDelegate; color: root.highlightColor }
    }

    Timer {
        id: resetTimer
        interval: 100; running: false; repeat: false
        onTriggered: { listDelegate.state = ""; clicked(); }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            mouse.accepted = true;
            listDelegate.state = "pressed";
            resetTimer.start();
        }
    }
}
