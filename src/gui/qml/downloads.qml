import QtQuick 1.1
import QtDesktop 0.1
import Pirateplayer 1.0
import Components 1.0

DownloadsWidget {
    anchors.fill: parent

    Component.onCompleted: {
//                downloadStack.addDownload("http://wrutschkow.org:8080/tmp/tmp.rar", "/home/chucky/tmp.rar");
//                downloadStack.addDownload("http://wrutschkow.org/", "/home/chucky/tmp.html");
    }

    model: downloadStack
}
