import QtQuick 1.1
import "../../components"

import "../../common.js" as Common

BarredListView {
    model: XmlListModel {
        id: programModel

        namespaceDeclarations: currentArgs.needsNSDecl ? "declare default element namespace 'http://www.w3.org/1999/xhtml';" : ""
        Component.onCompleted: source = currentArgs.url
        query: '//div[@class="k5-video-teasers-grid"]//table[not(@width)]//td//a'

        XmlRole {
            name: "title"
            query: "@title/string()"
        }
        XmlRole {
            name: "link"
            query: "@href/string()"
        }

        XmlRole {
            name: "thumb"
            query: "img/@src/string()"
        }
    }
    delegate: ListDelegate {
        imgSource: model.thumb
        text: model.title.slim()

        onClicked: {
            go( Qt.resolvedUrl("../pirateplay.qml"),
               { url: "http://kanal9play.se" + model.link,
                   title: model.title,
                   name: currentArgs.programName,
                   season: currentArgs.seasonNumber },
               model.index );
        }
    }

    XmlListModelStatusMessage { target: programModel }
}
