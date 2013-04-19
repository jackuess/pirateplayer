import QtQuick 1.1
import Components 1.0

import "../../common.js" as Common

AzListView {
    model: XmlListModel {
        source: "tidy://tv.aftonbladet.se/webbtv/"
        namespaceDeclarations: "declare default element namespace 'http://www.w3.org/1999/xhtml';"
        query: "//div[@class=\"expanded_nav_programs\"]//a[@class=\"group\"]"

        XmlRole {
            name: "text"
            query: "span/strong/string()"
        }
        XmlRole {
            name: "thumb"
            query: "span/img/@data-src/string()"
        }
        XmlRole {
            name: "link"
            query: "@href/string()"
        }
    }
    delegate: ListDelegate {
        text: model.text.slim()
        imgSource: decodeURIComponent(model.thumb)

        onClicked: {
            go( Qt.resolvedUrl("season.qml"),
               { url: model.link.replace('http://', 'tidy://'),
                   programName: model.text.slim() },
               model.index );
        }
    }
}
