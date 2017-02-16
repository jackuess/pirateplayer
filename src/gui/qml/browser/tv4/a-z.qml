import QtQuick 1.1
import Components 1.0

import "../../JSONListModel"
import "../../common.js" as Common

AzListView {

    model: JSONListModel {
        id: indexModel

        source: "http://webapi.tv4play.se/play/programs?is_premium=false"
		query: "$[*]"
    }

    delegate: ListDelegate {
        text: model.name
        onClicked: {
            go( Qt.resolvedUrl("program.qml"), 
			    {
					url: "http://webapi.tv4play.se/play/video_assets?is_live=false&platform=web&per_page=99999&node_nids=" + model.nid,
                    programName: model.name
				},
                model.index 
			);
        }
    }

}
