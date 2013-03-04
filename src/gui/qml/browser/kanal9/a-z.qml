import QtQuick 1.1
import "../../components"

import "../../common.js" as Common

AzListView {
    model: XmlListModel {
        source: "tidy://www.kanal9play.se/program"
        query: '//a[@class="k5-AToZPanel-program k5-AToZPanel-channel-KANAL9" or @class="k5-AToZPanel-program k5-AToZPanel-channel-KANAL9 k5-AToZPanel-program-topical"]'

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
            go( Qt.resolvedUrl("season.qml"),
               { url: "tidy://www.kanal9play.se" + model.link,
                   programName: model.text.slim() },
               model.index );
        }
    }
}
