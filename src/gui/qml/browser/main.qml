import QtQuick 1.1
import QtDesktop 0.1
import Components 1.0

Item {
    property alias history: loader.history
    anchors.fill: parent
    clip: true

    BrowserBar {
        id: bar
    }

    Loader {
        id: loader

        property variant currentArgs: { }
        //property variant currentArgs: { url: "http://www.svtplay.se/fraga-doktorn" }
        property variant history: []

        //Pushing/popping directly to/from loader.history would only modify a copy of loader.history, due to limitations of QML properties
        //We'll make a copy of loader.history, push to it and reasign loader.history
        function historyPush(item) {
            var tmpHistory = history;
            tmpHistory.push(item);
            history = tmpHistory;
        }
        function historyPop() {
            var tmpHistory = history;
            var r = tmpHistory.pop()
            history = tmpHistory;
            return r;
        }

        //Same goes for setting currentArgs
        function setArg(key, value) {
            var tmpArgs = currentArgs;
            tmpArgs[key] = value;
            currentArgs = tmpArgs;
        }

        function goBack() {
            var oldView = historyPop();

            currentArgs = oldView.args
            source = oldView.url;
        }
        function go(url, args, selectedIndex, silent) {
            if (!silent) {
                setArg("selectedIndex", selectedIndex);
                historyPush({ url: source, args: currentArgs });
            }

            currentArgs = args;
            source = url;
        }

        clip: true
        focus: true
        anchors { margins: 5; left: parent.left; right: parent.right; bottom: parent.bottom; top: bar.bottom }

        source: "menu.qml"
        //source: "pirateplay.qml"

        Keys.onPressed: {
            if ((event.key == Qt.Key_Backspace || event.key == Qt.Key_Back) && history.length > 0) {
                event.accepted = true;
                goBack();
            }
        }
    }
}
