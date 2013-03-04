import QtQuick 1.1
import Components 1.0

import "../../common.js" as Common

AzListView {
    model: XmlListModel {
        id: indexModel
        source: "tidy://www.svtplay.se/program"
        query: "//li[contains(@class, \"playListItem\")]/a"

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
               { url: "tidy://www.svtplay.se" + model.link + "?tab=episodes&sida=999",
                   programName: model.text.slim() },
               model.index);
        }
    }
}
