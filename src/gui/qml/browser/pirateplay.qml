import QtQuick 1.1
import QtDesktop 0.1
import Components 1.0


Item {
    anchors.fill: parent

// quality
/*
    XmlListModel {
        id: streams
        source: ppBase() + "/api/get_streams.xml?url=" + currentArgs.url
        query: "/streams/stream"
        onStatusChanged: {
            if (status === XmlListModel.Ready) {
                streamSelect.sourceComponent = streamDialog;
            }
        }

        XmlRole {
            name: "text"
            query: "concat( @quality/string(), \
                            substring('Okänd', 1, number(not(@quality/string())) * string-length('Okänd')) )"
        }
        XmlRole {
            name: "url"
            query: "string()"
        }
        XmlRole {
            name: "suffixHint"
            query: "@suffix-hint/string()"
        }
        XmlRole {
            name: "subtitles"
            query: "@subtitles/string()"
        }
        XmlRole {
            name: "requiredPlayerVersion"
            query: "@required-player-version/string()"
        }
    }
*/
    Component {
        id: streamDialog
		StreamDialog { 
			programMetaInfo: { 
				'title': currentArgs.programTitle ? currentArgs.programTitle : "",
				'name': currentArgs.programName ? currentArgs.programName : "",
				'season': currentArgs.programSeason ? currentArgs.programSeason : "",
				'time': currentArgs.programTime ? currentArgs.programTime : "",
				'description': currentArgs.programDescription ? currentArgs.programDescription : "" 
			}
			videoPageUrl: currentArgs.url
		}
    }

    Component.onDestruction: root.focus = true

    Loader {
        id: streamSelect

        anchors.fill: parent

        sourceComponent: streamDialog

// quality
//        property variant streamsModel: streams
    }

// quality
//    XmlListModelStatusMessage { target: streams }
}
