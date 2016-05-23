import QtQuick 1.1
import Components 1.0

import "../../common.js" as Common

BarredListView {
    model: XmlListModel {
		id: programModel
        source: currentArgs.url
        query: '//div[@class="svt-display-table-xs"]'
		
        XmlRole {
            name: "title"
            query: "div/div/*/a/string()"
        }
        XmlRole {
            name: "link"
            query: "div/div/*/a/@href/string()"
        }
        XmlRole {
            name: "date"
            query: "div/div/p/time/string()"
        }
        XmlRole {
            name: "thumb"
            query: "div/figure/img/@src/string()"
        }
    }
    delegate: ListDelegate {
        imgSource: model.thumb
        text: "<strong>" + model.title.slim() + "</strong> - " + model.date.slim()

        onClicked: {
            go( Qt.resolvedUrl("../pirateplay.qml"),
                {
					url: "http://www.oppetarkiv.se" + model.link,
					programName: currentArgs.programName,
                    programTitle: model.title.slim(),
                    programTime: model.date 
				},
                model.index);
        }
    }

    XmlListModelStatusMessage { target: programModel }
}
