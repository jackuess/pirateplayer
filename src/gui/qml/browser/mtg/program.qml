import QtQuick 1.1
import Components 1.0

import "../../common.js" as Common

BarredListView {
    model: XmlListModel {
        id: programModel

        Component.onCompleted: source = currentArgs.url

        query: "//table[@class=\"clearfix clip-list video-tree\"]//tr[descendant::th[a]]"
        namespaceDeclarations: "declare default element namespace 'http://www.w3.org/1999/xhtml';"

        XmlRole {
            name: "title"
            query: "th[@class=\"col1\"]/a/string()"
        }
        XmlRole {
            name: "epNo"
            query: "td[@class=\"col2\"]/string()"
        }
        XmlRole {
            name: "time"
            query: "td[@class=\"col4\"]/string()"
        }
        XmlRole {
            name: "link"
            query: "th[@class=\"col1\"]/a/@href/string()"
        }
    }

    delegate: ListDelegate {
        text: model.title.slim() + " avsnitt <strong>" + model.epNo + "</strong><br/>" + model.time

        onClicked: {
            go( Qt.resolvedUrl("../pirateplay.qml"),
               { url: "http://www.tv" + currentArgs.n + "play.se" + model.link,
                   title: model.epNo,
                   name: model.title.slim(),
                   time: model.time },
               model.index);
        }
    }

    XmlListModelStatusMessage { target: programModel }
}
