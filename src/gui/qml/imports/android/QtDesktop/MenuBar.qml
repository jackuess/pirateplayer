import QtQuick 1.1

Item {
    Component.onCompleted: {
        parent.Keys.menuPressed.connect(children[0].activate)
    }
}
