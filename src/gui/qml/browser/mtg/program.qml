import QtQuick 1.1
import Components 1.0

import "../../common.js" as Common

BarredListView {
    model: XmlListModel {
        id: programModel

        source: currentArgs.url.replace("http://", "tidy://")

        query: '//section[@data-section="episodelist"]//div[@data-title]'

        XmlRole {
            name: "title"
            query: "div/a/div/div/h3/string()"
        }
        XmlRole {
            name: "time"
            query: "div/em/time/string()"
        }
        XmlRole {
            name: "link"
            query: "div/a/@href/string()"
        }
    }

    delegate: ListDelegate {
        text: "<strong>" + model.title.slim() + "</strong><br/>" + model.time

        onClicked: {
            go( Qt.resolvedUrl("../pirateplay.qml"),
               { url: model.link,
				   programName: currentArgs.programName,
				   programTitle: model.title.slim(),
                   programTime: model.time },
               model.index);
        }
    }

    XmlListModelStatusMessage { target: programModel }
}
