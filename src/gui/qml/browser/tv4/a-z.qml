import QtQuick 1.1
import Components 1.0

import "../../common.js" as Common

AzListView {
    model: XmlListModel {
        source: "tidy://www.tv4play.se/program?per_page=999&per_row=4&page=1&content-type=a-o&is_premium=false"
        query: "//ul[@class=\"a-o-list js-show-more-content\"]/li/ul/li/a"

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
               { url: "tidy://www.tv4play.se" + decodeURIComponent(model.link),
                   programName: model.text.slim() },
               model.index );
        }
    }
}
