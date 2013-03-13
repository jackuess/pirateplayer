import QtQuick 1.1
import Components 1.0

import "../../common.js" as Common

BarredListView {
    model: XmlListModel {
        id: indexModel

        Component.onCompleted: source = currentArgs.url
        query: "//div[@class=\"sbs-video-season-episode-teaser\"]"

        XmlRole {
            name: "title"
            query: "table//h4[@class=\"title\"]/a/string()"
        }
        XmlRole {
            name: "description"
            query: "table//p[@class=\"description\"]/string()"
        }

        XmlRole {
            name: "thumb"
            query: "table//img/@src/string()"
        }
        XmlRole {
            name: "link"
            query: "table//h4[@class=\"title\"]/a/@href/string()"
        }
    }
    delegate: ListDelegate {
        imgSource: model.thumb
        text: "<strong>" + model.title.slim() + "</strong><br/><small>" + model.description + "</small>"

        onClicked: {
            go( Qt.resolvedUrl("../pirateplay.qml"),
               { url: "http://kanal" + currentArgs.n + "play.se" + model.link,
                   programTitle: model.title.slim(),
                   programName: currentArgs.programName,
                   programSeason: currentArgs.programSeason,
                   programDescription: model.description.slim() },
               model.index );
        }
    }

    XmlListModelStatusMessage { target: indexModel }
}
