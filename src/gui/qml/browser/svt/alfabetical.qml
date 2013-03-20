import QtQuick 1.1
import Components 1.0

import "../../HttpREModel"
import "../../common.js" as Common

AzListView {
    model: HttpREModel {
        id: indexModel

        source: "http://svtplay.se/program"
        regExp: "(<a href=\"[^\"]+\" class=\"playAlphabeticLetterLink\">(.|\n)*?</a>)"

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
               { url: "http://www.svtplay.se" + model.link + "?tab=episodes&sida=999",
                   programName: model.text },
               model.index);
        }
    }
}
