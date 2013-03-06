import QtQuick 1.1
import QtDesktop 0.1
import Pirateplayer 1.0

Column {
    property variant currentStatus
    //property bool isDownloading: (table.currentIndex > -1) && (downloadStack.get(table.currentIndex)["statusCol"] == DownloadList.Downloading)
    //onIsDownloadingChanged: console.log(isDownloading)

    anchors.margins: 10
    width: 130

    Button {id: cancelBtn
        width: parent.width
        text: "Avbryt"
        //enabled: downloadStack.get(table.currentIndex)["statusCol"] == DownloadList.Downloading // table.anythingSelected &&
        //enabled: isDownloading
        //enabled: false
        enabled: table.currentItemStatus == DownloadList.Downloading
        onClicked: downloadStack.abortDownload(table.currentIndex)
//        Component.onCompleted: {
//            table.currentIndexChanged.connect(function () {
//                console.log(table.currentIndex);
//                if (table.currentIndex > -1)
//                    cancelBtn.enabled = downloadStack.get(table.currentIndex)["statusCol"] == DownloadList.Downloading;
//                else
//                    cancelBtn.enabled = false;
//            });
//        }
    }
//    Binding {
//        function currentStatus() {
//            if (table.currentIndex > -1)
//                return downloadStack.get(table.currentIndex)["statusCol"];
//            else
//                return -1
//        }

//        target: cancelBtn
//        property: "enabled"
//        when: table.currentIndex > -1
//        value: (table.model.get(table.currentIndex)["statusCol"] == DownloadList.Downloading) || (table.model.get(table.currentIndex)["statusCol"] == DownloadList.NotStarted)
//    }
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
        //onClicked: Qt.openUrlExternally(downloadStack.get(table.currentIndex)["fileName"])
        onClicked: console.log(downloadStack.get(table.currentIndex)["statusCol"], DownloadList.Downloading, table.anythingSelected && (downloadStack.get(table.currentIndex)["statusCol"] == DownloadList.Downloading))
    }
}
