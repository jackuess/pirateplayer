import QtQuick 1.1
import Components 1.0

import "../../common.js" as Common

BarredListView {
    model: XmlListModel {
        id: indexModel

        Component.onCompleted: source = currentArgs.url
        query: "//section[@class=\"tv\" or @class=\"radio\"]/a"

        XmlRole {
            name: "title"
            query: "hgroup/h1/string()"
        }
        XmlRole {
            name: "description"
            query: "div[@class=\"details\"]/string()"
        }

        XmlRole {
            name: "thumb"
            query: "span/img/@src/string()"
        }
        XmlRole {
            name: "link"
            query: "@href/string()"
        }
    }

    delegate: ListDelegate {
        imgSource: model.thumb
        text: "<strong>" + model.title.slim() + "</strong><br/><small>" + model.description + "</small>"

        onClicked: {
            go( Qt.resolvedUrl("../pirateplay.qml"),
               { url: "http://urplay.se" + model.link,
               programName: currentArgs.programName,
                   programTitle: model.title.slim(),
                   programDescription: model.description.slim() },
               model.index);
        }
    }

    XmlListModelStatusMessage { target: indexModel }
}
