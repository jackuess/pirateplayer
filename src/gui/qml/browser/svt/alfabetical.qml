import QtQuick 1.1
import Components 1.0

import "../../common.js" as Common

AzListView {
    //property int status: XmlListModel.Loading

    Component.onCompleted: {
        var doc = new XMLHttpRequest();
        doc.onreadystatechange = function() {
            if (doc.readyState == XMLHttpRequest.DONE) {
                //status = XmlListModel.Ready;
                indexModel.xml = "<root>" + doc.responseText.replace(/<head>(.|\n)+<\/head>/, "").replace(/<script(.|\n)+<\/script>/, "").replace(/<\/?[^a\/][^>]+>/g, "").replace(/&auml;/g, "ä").replace(/&Auml;/g, "Ä").replace(/&ouml;/g, "ö").replace(/&Ouml;/g, "Ö").replace(/&aring;/g, "å").replace(/&Aring;/g, "Å").replace(/&aacute;/g, "á").replace(/&eacute;/g, "é").replace(/&ndash;/g, "-").replace(/\s&\s/g, "&amp;") + "</root>";
            }
        }
        doc.open("GET", "http://svtplay.se/program");
        doc.send();
    }

    model: XmlListModel {
        id: indexModel
        query: "//a[@class=\"playAlphabeticLetterLink\"]"

        XmlRole {
            name: "text"
            query: "string()"
        }
        XmlRole {
            name: "link"
            query: "@href/string()"
        }
    }

    delegate: ListDelegate {
        text: model.text.slim()
        onClicked: {
            go( Qt.resolvedUrl("program.qml"),
               { url: "tidy://www.svtplay.se" + model.link + "?tab=episodes&sida=999",
                   programName: model.text.slim() },
               model.index);
        }
    }
}
