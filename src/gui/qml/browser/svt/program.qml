import QtQuick 1.1
import Components 1.0

import "../../JSONListModel"
import "../../common.js" as Common

BarredListView {
    anchors.fill: parent

    model: JSONListModel {
        id: programModel

        source: currentArgs.url ? currentArgs.url : ""
		regExp: "__svtplay'] = ({.*});"
		query: [
			'$.videoTitlePage.relatedVideosTabs[?(@.type!="RELATED_VIDEO_TABS_LATEST" && @.type!="RELATED_VIDEO_TABS_CLIP")].videos[*]',
			'$.videoTitlePage.relatedVideosTabs[?(@.type=="RELATED_VIDEO_TABS_CLIP")].videos[*]'
		]
    }

    delegate: ListDelegate {
        text: 
			"<strong>" + (model.season > 0 ? "Säsong " + model.season + " - " : "")
			+ model.title + "</strong>"
			+ (model.broadcastDate ? " - " + model.broadcastDate.replace(/:..\+.*/, "").replace("T", " ") : "")
        onClicked: {
            var pageUrl = "http://www.svtplay.se" + encodeURI(model.contentUrl);
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
