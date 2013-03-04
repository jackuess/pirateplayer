import QtQuick 1.1

Item {
    default property alias content: stack.children
    property Component header

    property int current: 0
    property int count: stack.children.length
    property string position: "North"

    onCurrentChanged: setOpacities()
    Component.onCompleted: setOpacities()

    Keys.onPressed: {
        if (event.key == Qt.Key_PageUp && event.modifiers == Qt.ControlModifier) {
            current = (current-1).mod(count);
            event.accepted = true;
        }
        else if (event.key == Qt.Key_PageDown && event.modifiers == Qt.ControlModifier) {
            current = (current+1)%count;
            event.accepted = true;
        }
    }

    function setOpacities() {
        for (var i = 0; i < stack.children.length; ++i) {
            stack.children[i].opacity = (i == current ? 1 : 0)
            stack.children[i].focus = i == current;
        }
    }

    Loader {
        id: headerLoader
        sourceComponent: header
    }

    Item {
        id: stack
        anchors.left: position === "west" ? headerLoader.right : parent.left
        anchors.right: parent.right

        anchors.top: position === "north" ? headerLoader.bottom : parent.top
        anchors.bottom: parent.bottom
    }
}
