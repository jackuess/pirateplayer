import QtQuick 1.1
import Components 1.0

import "../../common.js" as Common

AzListView {
    model: XmlListModel {
        source: "tidy://www.tv" + /*ViewBrowser.currentView.args.n*/currentArgs.n + "play.se/program"
        query: "//a[@class=\"list-item\"]"

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
               { url: "tidy://www.tv" + currentArgs.n + "play.se" + model.link,
                   n: currentArgs.n,
                   programName: model.text.slim() },
               model.index);
        }
    }
}
