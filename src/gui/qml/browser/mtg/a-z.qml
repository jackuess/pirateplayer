import QtQuick 1.1
import Components 1.0

import "../../common.js" as Common

AzListView {
    model: XmlListModel {
        source: "tidy://www.viafree.se/program"
        query: '//a[@class="thumbnail-format format-title className"]'

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
            go( Qt.resolvedUrl("program.qml"),
               { url: "http://www.viafree.se" + model.link,
                   programName: model.text.slim() },
               model.index);
        }
    }
}
