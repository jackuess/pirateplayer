import QtQuick 1.1
import Components 1.0

import "../../common.js" as Common

BarredListView {
    model: ListModel {
        ListElement {
            title: "Program A-Ö"
            module: "alfabetical"
            url: ""
        }
        ListElement {
            title: "Rekommenderat"
            module: "program"
            url: "http://www.svtplay.se/?tab=recommended&sida=3"
        }
        ListElement {
            title: "Senaste program"
            module: "program"
            url: "http://www.svtplay.se/?tab=episodes&sida=3"
        }
        ListElement {
            title: "Kanaler (live)"
            module: "live"
            url: ""
        }
        ListElement {
            title: "Öppet arkiv"
            module: "open_a-z"
            url: ""
        }
    }

    delegate: ListDelegate {
        text: model.title

        onClicked: {
            go( Qt.resolvedUrl(model.module + ".qml"),
               model.module === "program" ? { url: model.url } : {},
               model.index);
        }
    }
}
