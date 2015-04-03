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
                regExp: 'class="play_vertical-list__header-link">([^<]+)</a>'
                function decode(s) { return s.slim().decodeHTMLEntities(); }
            },
            ReRole {
                name: "link"
                regExp: '<a href="([^"]+)" class="play_vertical-list__header-link"'
            },
            ReRole {
                name: "thumb"
                regExp: '<img .*? src="([^"]+)"'
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
