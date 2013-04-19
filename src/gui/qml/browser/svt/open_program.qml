import QtQuick 1.1
import Components 1.0

import "../../common.js" as Common

BarredListView {
    model: XmlListModel {
        id: indexModel

        Component.onCompleted: source = currentArgs.url
        query: "//div[@class=\"svt-display-table-xs\"]"

        XmlRole {
            name: "title"
            query: "div/div/h2/a/string()"
        }
        XmlRole {
            name: "link"
            query: "div/div/h2/a/@href/string()"
        }
        XmlRole {
            name: "date"
            query: "div/div//time/string()"
        }
        XmlRole {
            name: "thumb"
            query: "div/figure/img/@data-imagename/string()"
        }
    }

    delegate: ListDelegate {
        imgSource: model.thumb
        text: "<strong>" + model.title.slim() + "</strong> - " + model.date.slim()

        onClicked: {
            go( Qt.resolvedUrl("../pirateplay.qml"),
               { url: model.link,
               programName: currentArgs.programName,
                   programTitle: model.title.slim(),
                    programTime: model.date },
               model.index);
        }
    }

    XmlListModelStatusMessage { target: indexModel }
}
