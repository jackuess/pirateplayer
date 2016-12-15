import QtQuick 1.1
import Components 1.0

import "../../common.js" as Common

BarredListView {
    model: XmlListModel {
        id: programModel

        source: currentArgs.url.replace("http://", "tidy://")

        query: '//a[@class="info-link"]'

        XmlRole {
            name: "title"
            query: "div//span[not(*)]/string()"
        }
        XmlRole {
            name: "time"
            query: "div/div[@class=thumbnail-published-at]/string()"
        }
        XmlRole {
            name: "link"
            query: "@href/string()"
        }
    }

    delegate: ListDelegate {
        text: "<strong>" + model.title.slim() + "</strong><br/>" + model.time

        onClicked: {
            go( Qt.resolvedUrl("../pirateplay.qml"),
               { url: "http://www.viafree.se" + model.link,
				   programName: currentArgs.programName,
				   programTitle: model.title.slim(),
                   programTime: model.time },
               model.index);
        }
    }

    XmlListModelStatusMessage { target: programModel }
}
