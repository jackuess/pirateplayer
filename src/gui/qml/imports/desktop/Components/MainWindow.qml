import QtQuick 1.1
import QtDesktop 0.1

Window {
    property variant windowSize: getSetting("MainWindow/size", { width: 720, height: 480 })
    property variant windowPosition: getSetting("MainWindow/pos", { x: 200, y: 200 })
    property int menuBarHeight

    width: windowSize.width; height: windowSize.height
    Component.onCompleted: {
        menuBarHeight = height-windowSize.height
    }

    x: windowPosition.x; y: windowPosition.y
    visible: true

    Component.onDestruction: {
        setSetting("MainWindow/size", { width: width, height: height-menuBarHeight });
        setSetting("MainWindow/pos", { x: x, y: y });
    }
}
