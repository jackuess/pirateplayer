import QtQuick 1.1
import Components 1.0

import "../../common.js" as Common

Item {
    id: list

    XmlListModel {
        id: programModel

        Component.onCompleted: source = currentArgs.url
        query: "//a[@class=\"js-show-more btn secondary full\" and contains(@data-more-from, 'type=video')]"

        onStatusChanged: {
            if (status === XmlListModel.Ready) {
                try {
                    episodesList.model.source = "tidy://www.tv4play.se" + decodeURIComponent(get(0).link) + "&page=0&per_page=400";
                } catch (err) {
                    if (err.name === 'TypeError')
                        episodesList.model.source = decodeURIComponent(programModel.source);
                    else
                        throw err;
                }
            }
        }

        XmlRole {
            name: "link"
            query: "@data-more-from/string()"
        }
    }
    BarredListView {
        id: episodesList

        anchors.fill: parent

        model: XmlListModel {
            query: "//div[@class=\"js-search-page\"]//li[not(descendant::p[starts-with(@class, 'requires-authorization')])]"

            XmlRole {
                name: "text"
                query: "div/h3/string()"
            }
            XmlRole {
                name: "link"
                query: "div/h3/a/@href/string()"
            }
            XmlRole {
                name: "date"
                query: "div/div[@class=\"video-meta\"]/p[@class=\"date\"]/string()"
            }
            XmlRole {
                name: "image"
                query: "p[@class=\"video-picture\"]/a/img/@src/string()"
            }
        }
        delegate: ListDelegate {
            imgSource: {
                var url = model.image;
                var pairs = url.split("&amp;");
                for (var i=0; i<pairs.length; i++) {
                    var pair = pairs[i].split("=");
                    if (pair[0] === "source")
                        return pair[1];
                }
            }
            text: model.text.slim() + "<br/>" + model.date

            onClicked: {
                go( Qt.resolvedUrl("../pirateplay.qml"),
                   { url: "http://tv4play.se" + model.link,
                       title: model.text.slim(),
                       name: currentArgs.programName,
                       time: model.date},
                   model.index );
            }
        }
    }

    XmlListModelStatusMessage { target: episodesList.model }
}
