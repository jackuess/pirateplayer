import QtQuick 1.1
import Components 1.0

import "../../HttpREModel"
import "../../common.js" as Common

BarredListView {
    id: svtProgram

    model: HttpREModel {
        id: liveModel

        source: "http://svtplay.se/kanaler"
        regExp: "(<li[^>]+data-tab[^>]+>(.|\n)*?<\/li>)"

        roles: [
            ReRole {
                name: "channel"
                regExp: 'alt="([^"]+)"'
                function decode(s) { return s.slim().decodeHTMLEntities(); }
            },
            ReRole {
                name: "title"
                regExp: "<span[^>]+class=\"playChannelMenuTitle\"[^>]*>([^<]+)"
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
        text: "<strong>" + model.channel + "</strong><br /><small>" + model.title + "</small>"
        imgSource: "http://svtplay.se" + model.thumb
        onClicked: {
            go( Qt.resolvedUrl("../pirateplay.qml"),
               { url: "http://svtplay.se" + model.link, programTitle: model.title, programName: model.channel },
               model.index);
        }
    }

    XmlListModelStatusMessage { target: liveModel }
}
