import QtQuick 1.1
import Components 1.0

import "../../common.js" as Common

BarredListView {
    anchors.fill: parent

    model: XmlListModel {
        id: indexModel

        Component.onCompleted: source = currentArgs.url
        query: "//div[@class=\"season\"]"

        XmlRole {
            name: "text"
            query: "h2/string()"
        }
        XmlRole {
            name: "link"
            query: "div[@class=\"season-info\"]/a/@href/string()"
        }
    }
    delegate: ListDelegate {
        text: model.text.slim()

        onClicked: {
            go( Qt.resolvedUrl("program.qml"),
               { url: "tidy://www.kanal" + currentArgs.n + "play.se" + model.link,
                   n: currentArgs.n,
                   programSeason: parseInt(model.text.slim().replace("Säsong ", "")),
                   programName: currentArgs.programName },
               model.index );
        }
    }

    XmlListModelStatusMessage { target: indexModel }
}
