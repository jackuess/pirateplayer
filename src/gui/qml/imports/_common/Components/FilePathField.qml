import QtQuick 1.1
import QtDesktop 0.1
import Components 1.0

Row {
    property alias path: fileName.text
    property string pathType: "file"
    property string filter: "Alla filer (*.*)"

    height: childrenRect.height
    spacing: 5

    TextField {
        id: fileName
        width: parent.width-browserButton.width-parent.spacing
    }

    Button {
        id: browserButton
        height: fileName.height
        text: "Bläddra"
        onClicked: {
            var newPath = pathType == "dir" ? getExistingDirectory(path) : getSaveFileName(path, filter);
            if (newPath !== "")
                fileName.text = newPath;
        }
    }
}
