import QtQuick 1.1
import QtDesktop 0.1
import Pirateplayer 1.0

Column {
    property variant currentStatus

    anchors.margins: 10
    width: 130

    Button {
        width: parent.width
        text: "Avbryt"
        enabled: table.anythingSelected && downloadStack.get(table.currentIndex)["statusCol"] == DownloadList.Downloading
        onClicked: downloadStack.abortDownload(table.currentIndex)
    }
    Button {
        id: removeButton
        width: parent.width
        text: "Ta bort"
        enabled: table.anythingSelected
        onClicked: downloadStack.removeRow(table.currentIndex);
    }
    Button {
        width: parent.width
        text: "Ta bort från disk"
        enabled: table.anythingSelected
        onClicked: {
            removePath(downloadStack.get(table.currentIndex)["fileName"]);
            removeButton.clicked();
        }
    }
    Button {
        width: parent.width
        text: "Öppna"
        enabled: table.anythingSelected
        onClicked: Qt.openUrlExternally(downloadStack.get(table.currentIndex)["fileName"])
    }
}
