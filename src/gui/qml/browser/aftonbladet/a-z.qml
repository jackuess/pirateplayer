import QtQuick 1.1
import Components 1.0

import "../../common.js" as Common

AzListView {
    model: XmlListModel {
        source: "tidy://www.aftonbladet.se/webbtv/"
        namespaceDeclarations: "declare default element namespace 'http://www.w3.org/1999/xhtml';"
        query: '//*[@id="abNavProgrambox"]/div/ul/li[@class="level1"]/a'

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
               { url: model.link.replace('http://', 'tidy://'),
                   programName: model.text.slim() },
               model.index );
        }
    }
}
