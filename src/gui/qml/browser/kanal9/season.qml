import QtQuick 1.1
import "../../components"

import "../../common.js" as Common

BarredListView {
    id: list

    model: XmlListModel {
        id: seasonModel

        onStatusChanged: {
            if (status === XmlListModel.Ready && seasonModel.count < 1) {
                go( Qt.resolvedUrl("program.qml"),
                   { url: decodeURIComponent(seasonModel.source),
                       programName: currentArgs.programName,
                       needsNSDecl: true },
                   0,
                   true );
            }
        }

        namespaceDeclarations: "declare default element namespace 'http://www.w3.org/1999/xhtml';"
        Component.onCompleted: source = currentArgs.url
        query: '//div[@class="k5-tab selected"]/table//div[@class="buttons"]/div'

        XmlRole {
            name: "seasonNumber"
            query: "text()[1]/string()"
        }
        XmlRole {
            name: "link"
            query: "span/string()"
        }
    }
    delegate: ListDelegate {
        text: "Säsong " + model.seasonNumber

        onClicked: {
            go( Qt.resolvedUrl("program.qml"),
               { url: "tidy://www.kanal9play.se" + model.link,
                   programName: currentArgs.programName,
                   seasonNumber: model.seasonNumber,
                   needsNSDecl: false },
               model.index );
        }
    }

    XmlListModelStatusMessage { target: seasonModel }
}
