import QtQuick 1.1
import Components 1.0

import "../../common.js" as Common

AzListView {
    id: list

    model: XmlListModel {
        id: azModel

        source: "tidy://urplay.se/A-O"
        query: '//*[@id="alphabet"]/ul/li/a'

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
        id: item
        text: model.text.slim().replace(/(TV)|(Radio)$/, "")

        onClicked: {
            go( Qt.resolvedUrl("program.qml"),
               { url: "tidy://www.urplay.se" + model.link,
                   programName: model.text.slim() },
               model.index);
        }
    }
}
