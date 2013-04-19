import QtQuick 1.1
import Components 1.0

import "../../common.js" as Common

BarredListView {
    XmlListModel {
        id: dataUrlModel

        source: currentArgs.url
        namespaceDeclarations: "declare default element namespace 'http://www.w3.org/1999/xhtml';"
        query: "//select[@class=\"select seasons\"]"

        onStatusChanged: {
            if (status === XmlListModel.Ready)
                seasonModel.source = source
        }

        XmlRole {
            name: "dataUrl"
            query: "@data-url/string()"
        }
    }

    model: XmlListModel {
        id: seasonModel

        namespaceDeclarations: "declare default element namespace 'http://www.w3.org/1999/xhtml';"
        query: "//select[@class=\"select seasons\"]/option"

        XmlRole {
            name: "text"
            query: "string()"
        }
        XmlRole {
            name: "n"
            query: "@value/string()"
        }
    }
    delegate: ListDelegate {
        text: model.text.slim()

        onClicked: {
            go( Qt.resolvedUrl("program.qml"),
               { url: "http://tv.aftonbladet.se" + dataUrlModel.get(0).dataUrl + "/" + model.n,
                   programName: currentArgs.programName },
               model.index );
        }
    }

    XmlListModelStatusMessage { target: seasonModel }
}
