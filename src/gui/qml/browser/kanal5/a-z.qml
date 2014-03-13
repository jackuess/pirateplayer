import QtQuick 1.1
import Components 1.0

import "../../common.js" as Common

AzListView {
    model: XmlListModel {
        source: "tidy://www.kanal" + currentArgs.n + "play.se/program"
        query: "//div[@class=\"logo\" and descendant::img]/div"

        XmlRole {
            name: "text"
            query: "a[@class=\"ajax\"]/img/@alt/string()"
        }
        XmlRole {
            name: "image"
            query: "a[@class=\"ajax\"]/img/@src/string()"
        }
        XmlRole {
            name: "link"
            query: "a[@class=\"ajax\"]/@href/string()"
        }
    }
    delegate: ListDelegate {
        imgSource: model.image
        text: model.text.slim()

        onClicked: {
            go( Qt.resolvedUrl("season.qml"),
               { url: "tidy://www.kanal" + currentArgs.n + "play.se" + model.link,
                   n: currentArgs.n,
                   programName: model.text.slim() },
               model.index );
        }
    }
}
