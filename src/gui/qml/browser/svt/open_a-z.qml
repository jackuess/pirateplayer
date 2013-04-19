import QtQuick 1.1
import Components 1.0

import "../../common.js" as Common

AzListView {
    model: XmlListModel {
        source: "tidy://www.oppetarkiv.se/kategori/titel"
        query: "//li[@class=\"svtoa-anchor-list-item\"]/a"

        XmlRole {
            name: "text"
            query: "string()"
        }
        XmlRole {
            name: "link"
            query: "@href/string()"
        }
    }
    delegate: ListDelegate {
        text: model.text.slim()

        onClicked: {
            go( Qt.resolvedUrl("open_program.qml"),
               { url: decodeURIComponent(model.link.replace("http://", "tidy://")),
                   programName: model.text.slim() },
               model.index );
        }
    }
}
