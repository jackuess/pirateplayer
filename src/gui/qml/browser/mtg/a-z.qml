import QtQuick 1.1
import Components 1.0

import "../../common.js" as Common

AzListView {
    model: XmlListModel {
        source: "tidy://www.tv3play.se/program"
        query: '//a[@class="list-item"]'

        XmlRole {
            name: "text"
            query: "div/string()"
        }
        XmlRole {
            name: "link"
            query: "@href/string()"
        }
    }
    delegate: ListDelegate {
        text: model.text.slim()

        onClicked: {
            go( Qt.resolvedUrl("program.qml"),
               { url: model.link,
                   programName: model.text.slim() },
               model.index);
        }
    }
}
