import QtQuick 1.1
import Components 1.0

import "../../HttpREModel"
import "../../common.js" as Common

BarredListView {
    anchors.fill: parent

    model: HttpREModel {
        id: programModel

        source: currentArgs.url ? currentArgs.url : ""
        regExp: '<article class="play_vertical-list__video-element play_js-svtplayer-teaser-container">(.|\n)*?</article>'

        roles: [
            ReRole {
                name: "text"
                regExp: '<a [^>]*>([^>]*)</a>'
                function decode(s) { return s.slim().decodeHTMLEntities(); }
            },
            ReRole {
                name: "link"
                regExp: ' href="([^"]+)"'
            },
            ReRole {
                name: "thumb"
                regExp: ' src="([^"]+)"'
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
