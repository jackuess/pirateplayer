import QtQuick 1.1
import Components 1.0

import "../../HttpREModel"
import "../../common.js" as Common

BarredListView {
    anchors.fill: parent

    model: HttpREModel {
        id: programModel

        source: currentArgs.url
        regExp: "(<article class=\"svtUnit[^>]+>(.|\n)*?</article>)"

        roles: [
            ReRole {
                name: "text"
                regExp: 'data-title="([^"]+)"'
                function decode(s) { return s.slim().decodeHTMLEntities(); }
            },
            ReRole {
                name: "link"
                regExp: "href=\"([^\"]+)\""
            },
            ReRole {
                name: "thumb"
                regExp: "src=\"([^\"]+)\""
            }
        ]
    }



    delegate: ListDelegate {
        imgSource: model.thumb.startsWith("http://") ? model.thumb : "http://svtplay.se" + model.thumb
        text: model.text
        onClicked: {
            var base = (model.link.startsWith("http") ? "" : "http://svtplay.se");
            go( Qt.resolvedUrl("../pirateplay.qml"),
               { url: base + model.link, programTitle: model.text, programName: currentArgs.programName },
               model.index);
        }
    }

    XmlListModelStatusMessage { target: programModel }
}
