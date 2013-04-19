import QtQuick 1.1
import Components 1.0

import "../../JSONListModel"
import "../../common.js" as Common

BarredListView {
    id: listView

    JSONListModel {
        id: jsonModel

        source: currentArgs.url
        query: "$.episodes[*]"
    }

    model: jsonModel.model

    delegate: ListDelegate {
        imgSource: model.image.teaserUri
        text: "<strong>Avsnitt " + model.episode + "</strong> - " + model.description

        onClicked: {
            go( Qt.resolvedUrl("../pirateplay.qml"),
               { url: model.url,
                   programTitle: "Avsnitt " + model.episode,
                   programDescription: model.description,
                   programName: currentArgs.programName },
               model.index );
        }
    }

    XmlListModelStatusMessage { target: jsonModel.model }
}
