import QtQuick 1.1
import Components 1.0

import "../../HttpREModel"
import "../../common.js" as Common

AzListView {
    model: HttpREModel {
        id: indexModel

        source: "http://www.svtplay.se/program"
		regExp: "<li class=\"play_alphabetic-list__item\"[^>]*>(<a[^>]*>[^<]*</a>)</li>"

        roles: [
            ReRole {
                name: "text"
                regExp: ">([^<]+)</a>"
                function decode(s) { return s.slim().decodeHTMLEntities(); }
            },
            ReRole {
                name: "link"
                regExp: 'href="([^"]+)"'
            }
        ]
    }

    delegate: ListDelegate {
        text: model.text
        onClicked: {
            go( Qt.resolvedUrl("program.qml"),
               { url: "http://www.svtplay.se" + model.link,
                   programName: model.text },
               model.index);
        }
    }
}
