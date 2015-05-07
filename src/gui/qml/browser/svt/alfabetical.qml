import QtQuick 1.1
import Components 1.0

import "../../HttpREModel"
import "../../common.js" as Common

AzListView {
    model: HttpREModel {
        id: indexModel

        source: "http://www.svtplay.se/program"
        regExp: "(<a href=\"[^\"]+\" title=\"[^\"]+\" class=\"play_link-list__link[^\"]*\">(.|\n)*?</a>)"

        roles: [
            ReRole {
                name: "text"
                regExp: ">([^<]+)<"
                function decode(s) { return s.slim().decodeHTMLEntities(); }
            },
            ReRole {
                name: "link"
                regExp: "href=\"([^\"]+)\""
            }
        ]
    }

    delegate: ListDelegate {
        text: model.text
        onClicked: {
            go( Qt.resolvedUrl("program.qml"),
               { url: "http://www.svtplay.se" + model.link + "?sida=999",
                   programName: model.text },
               model.index);
        }
    }
}
