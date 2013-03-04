import QtQuick 1.1
import Components 1.0

import "../../common.js" as Common

BarredListView {
    model: XmlListModel {
        id: indexModel

        namespaceDeclarations: "declare default element namespace 'http://www.w3.org/1999/xhtml';"
        Component.onCompleted: source = currentArgs.url
        query: '//*[@id="abArtikelytaContainer"]/div[@class="abLinkBlock"][position() <= 2]//div[@class="abTeaser"]'

        XmlRole {
            name: "title"
            query: 'div//span[@class="abLink"]/a/string()'
        }
        XmlRole {
            name: "description"
            query: 'div[@class="abTeaserText"]/text()[2]/string()'
        }
        XmlRole {
            name: "thumb"
            query: "span//img/@src/string()"
        }
        XmlRole {
            name: "link"
            query: 'div//span[@class="abLink"]/a/@href/string()'
        }
    }
    delegate: ListDelegate {
        imgSource: decodeURIComponent(model.thumb)
        text: model.title.slim() + " - <small>" + model.description.slim() + "</small>"

        onClicked: {
            go( Qt.resolvedUrl("../pirateplay.qml"),
               { url: model.link,
                   programTitle: model.title.slim(),
                   programDescription: model.description.slim(),
                   programName: currentArgs.programName },
               model.index );
        }
    }

    XmlListModelStatusMessage { target: indexModel }
}
