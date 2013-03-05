import QtQuick 1.1
import QtDesktop 0.1
import Components 1.0

Window {
    id: settingsWindow
    anchors.fill: parent
    focus: true
    title: "Inställningar"
    modality: Qt.WindowModal
    width: 640
    height: settingsLayout.height+10
    visible: true

    KeyNavigation.tab: startDir

    Component.onDestruction: {
        if (startDirValid.when)
            startDir.path = getSetting("Location/start_dir", userSettings.defaults.startDir)
        userSettings.save();
    }

    Binding { target: userSettings; property: "startDir"; value: startDir.path }
    Binding { target: userSettings; property: "playerCmd"; value: playerCmd.text }
    Binding { target: userSettings; property: "filenameTemplate"; value: filenameTemplate.text }
    Binding { target: userSettings; property: "pirateplaySeBase"; value: pirateplaySeBase.text }
    Binding { target: userSettings; property: "fontPointSize"; value: fontPointSize.text }

    FormLayout {
        id: settingsLayout
        anchors.left: parent.left; anchors.right: parent.right
        anchors.margins: 15
        anchors.verticalCenter: parent.verticalCenter

        FormRow {
            label: "Standardkatalog"

            FilePathField {
                id: startDir
                width: parent.width
                KeyNavigation.tab: playerCmd
                path: userSettings.startDir
                pathType: "dir"
            }
        }
        FormRow {
            label: ""
            Message {
                id: startDirValid
                state: "error"
                text: "Katalogen existerar inte"
                when: !pathIsDir(startDir.path)
            }
        }

        FormRow {
            label: "Uppspelningskommando"
            visible: !mobile()

            TextField {
                id: playerCmd
                width: parent.width
                KeyNavigation.tab: filenameTemplate;
                text: userSettings.playerCmd
            }
        }
        FormRow {
            label: "Namngivningsschema"

            TextField {
                id: filenameTemplate
                width: parent.width
                KeyNavigation.tab: fontPointSize;
                text: userSettings.filenameTemplate
            }
        }
        FormRow {
            label: "Teckenstorlek i punkter (börjar gälla efter omstart)"

            TextField {
                id: fontPointSize
                width: parent.width
                KeyNavigation.tab: settingsWindow;
                text: userSettings.fontPointSize
                validator: IntValidator { bottom: 1 }
            }
        }
        FormRow { Label { text: "Variabler: %title%, %name%, %season%, %time%, %description%."; width: parent.width } label: "" }
        FormRow {
            label: "Pirateplay.se-bas"
            visible: !mobile() && showDebugOptions.checked
            TextField {
                id: pirateplaySeBase
                width: parent.width
                text: userSettings.pirateplaySeBase
            }
        }
        FormRow {
            id: actionButtons
            label: ""
            layoutDirection: Qt.RightToLeft

            Component.onCompleted: if (mobile()) actionButtons.destroy()

            Button { height: 25; text: "Stäng"; onClicked: settingsWindow.close() }
            Button { height: 25; text: "Standardinställningar"; onClicked: userSettings.reset() }
            CheckBox { id: showDebugOptions; text: "Visa utvecklaralternativ"; visible: !mobile() }
        }
    }
}
