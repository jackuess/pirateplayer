import QtQuick 1.1
import Components 1.0

import "../../HttpREModel"
import "../../common.js" as Common

AzListView {
    model: HttpREModel {
        id: indexModel

        source: "https://www.oppetarkiv.se/program"
		regExp: "<li data-genre=[^>]*>(<a[^>]*>[^<]*</a>)</li>"

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
        text: model.text.slim()

        onClicked: {
            go( Qt.resolvedUrl("open_program.qml"),
                {
					url: "https://www.oppetarkiv.se" + decodeURI(model.link),
					programName: model.text.slim() 
				},
                model.index );
        }
    }
}

