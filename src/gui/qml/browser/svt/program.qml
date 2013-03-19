import QtQuick 1.1
import Components 1.0

import "../../common.js" as Common

BarredListView {
    property int status: XmlListModel.Loading

    anchors.fill: parent

    Component.onCompleted: {
        var doc = new XMLHttpRequest();
        doc.onreadystatechange = function() {
            if (doc.readyState == XMLHttpRequest.DONE) {
                status = XmlListModel.Ready;
                programModel.xml = "<root>" + doc.responseText.replace(/<![^>]*>/g, "").replace(/<\/?((img)|(input))[^>]+>/g, "").replace(/<\/noscript>\n<\/head>/, "</noscript>").replace(/<\/body>\n<\/html>/, "</body></body></html></html></html></html></html>").replace(/\s&\s/g, "&amp;").replace(/&/g, "").replace(/document.write\('[^']+'/g, "").replace(/&rdquo;/g, "").replace(/&hellip;/g, "...").replace(/&nbsp;/g, " ").replace(/&auml;/g, "ä").replace(/&Auml;/g, "Ä").replace(/&ouml;/g, "ö").replace(/&Ouml;/g, "Ö").replace(/&aring;/g, "å").replace(/&Aring;/g, "Å").replace(/&aacute;/g, "á").replace(/&eacute;/g, "é").replace(/&ndash;/g, "-") + "</root>";
            }
        }
        doc.open("GET", currentArgs.url);
        doc.send();
    }

    model: XmlListModel {
        id: programModel

        query: '//div[@data-tabname="episodes" or @data-tabname="recommended"]//article'

        XmlRole {
            name: "text"
            query: "@data-title/string()"
        }
        XmlRole {
            name: "link"
            query: "div/a[1]/@href/string()"
        }
        XmlRole {
            name: "thumb"
            query: "div//img/@src/string()"
            //query: "div//noscript/img/@src/string()"
        }
    }

    delegate: ListDelegate {
        //imgSource: model.thumb.startsWith("http://") ? model.thumb : "http://svtplay.se" + model.thumb
        text: model.text.slim()
        onClicked: {
            var base = (model.link.startsWith("http") ? "" : "http://svtplay.se");
            go( Qt.resolvedUrl("../pirateplay.qml"),
               { url: base + model.link, programTitle: model.text.slim(), programName: currentArgs.programName },
               model.index);
        }
    }

    XmlListModelStatusMessage { target: parent }
}
