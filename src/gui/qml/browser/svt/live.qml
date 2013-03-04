import QtQuick 1.1
import Components 1.0

import "../../common.js" as Common

BarredListView {
    id: svtProgram

    property int status: XmlListModel.Loading

    Component.onCompleted: {
        var doc = new XMLHttpRequest();
        doc.onreadystatechange = function() {
            if (doc.readyState == XMLHttpRequest.DONE) {
                status = XmlListModel.Ready;
                programModel.xml = doc.responseText.replace("<![if !(lte IE 7)]>", "").replace("<![endif]>", "");
            }
        }
        doc.open("GET", "tidy://svtplay.se/kanaler");
        doc.send();
    }

    model: XmlListModel {
        id: programModel
        query: '//div[@class="svtTabMenu playChannelMenu"]//li/a'

        XmlRole {
            name: "channel"
            query: "div/img[1]/@alt/string()"
        }
        XmlRole {
            name: "title"
            query: "div/span[@class=\"playChannelMenuTitle\"]/string()"
        }

        XmlRole {
            name: "link"
            query: "@href/string()"
        }
        XmlRole {
            name: "thumb"
            query: "div/img[1]/@src/string()"
        }
    }

    delegate: ListDelegate {
        text: "<strong>" + model.channel.slim() + "</strong><br /><small>" + model.title + "</small>"
        imgSource: "http://svtplay.se" + model.thumb
        onClicked: {
            go( Qt.resolvedUrl("../pirateplay.qml"),
               { url: "http://svtplay.se" + model.link, title: model.title, name: model.channel },
               model.index);
//            ViewBrowser.piratePlay( "http://svtplay.se" + model.link,
//                                  { title: model.title,
//                                    name: model.channel } );
        }
    }

    XmlListModelStatusMessage { target: parent }
}
