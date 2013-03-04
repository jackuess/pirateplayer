import QtQuick 1.1
import QtDesktop 0.1
import Pirateplayer 1.0
import Components 1.0

MainWindow {
    id: root

    property string highlightColor: "#0099cc"

    focus: true
    title: "Pirateplayer " + applicationVersion()

    onFocusChanged: tabWidget.content[tabWidget.current].focus = focus

    Keys.onPressed: if (event.key == Qt.Key_Back) { quit() }

    MenuBar {
        Menu {
            text: "&Meny"

            MenuItem {
                id: fullscreenItem
                text: "Helskärm"
                shortcut: "F11"
                onTriggered: root.windowState = root.windowState^Qt.WindowFullScreen;
                Component.onCompleted: if (mobile()) fullscreenItem.destroy()
            }
            MenuItem {
                text: "Inställningar"
                shortcut: "Ctrl+I"
                onTriggered: {
                    var _settingsWindow = Qt.createComponent(Qt.resolvedUrl("settings.qml"));
                    if (_settingsWindow.status == Component.Ready) {
                        var settingsWindow = _settingsWindow.createObject(root);
                        settingsWindow.Component.destruction.connect( function () { root.focus = true } );
                    }
                    else if(_settingsWindow.status == Component.Error)
                        console.log(_settingsWindow.errorString());
                }
            }
        }
    }

    DownloadList {
        id: downloadStack
    }

    TabWidget {
        id: tabWidget

        anchors { left: parent.left; right: parent.right; top: parent.top; bottom: newsBox.top }

        current: 0

        TabItem {
            title: "Bläddra"

            Loader {
                id: browser
                anchors.fill: parent
                focus: true
                source: "browser/main.qml"
            }
        }

        TabItem {
            title: "Nedladdningar (" + downloadStack.count + ")"

            Loader {
                anchors.fill: parent
                source: "downloads.qml"
            }
        }
    }

    NewsBox {
        id: newsBox
        anchors { left: parent.left; right: parent.right; bottom: parent.bottom }
    }

    UserSettings {
        id: userSettings
    }
}
