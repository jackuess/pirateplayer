import QtQuick 1.1
import Components 1.0

import "../../JSONListModel"
import "../../common.js" as Common

BarredListView {
    anchors.fill: parent

    model: JSONListModel {
        id: programModel

        source: currentArgs.url ? currentArgs.url : ""
		query: [
			"$.results[?(@.full_program)]",
			"$.results[?(!@.full_program)]"
		]
    }

    delegate: ListDelegate {
        text: 
			"<strong>" + (model.season > 0 ? "Säsong " + model.season + " " : "")
			+ model.title + "</strong> - "
			+ model.broadcast_date_time.replace(/:..\+.*/, "").replace("T", " ")
        onClicked: {
            var pageUrl = "http://www.tv4play.se/program/" + encodeURI(model.program_nid) + "?video_id=" + model.id;
            go( Qt.resolvedUrl("../pirateplay.qml"),
                {
					url: pageUrl, 
					programTitle: model.title, 
					programName: currentArgs.programName,
					programSeason: (model.season > 0 ? "Säsong " + model.season : "")
				},
                model.index);
        }
    }

    XmlListModelStatusMessage { target: programModel }
}
