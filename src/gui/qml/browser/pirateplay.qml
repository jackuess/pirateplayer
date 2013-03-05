import QtQuick 1.1
import QtDesktop 0.1
import Components 1.0


Item {
    anchors.fill: parent

    XmlListModel {
        id: streams
        source: userSettings.pirateplaySeBase + "/api/get_streams.xml?url=" + currentArgs.url
        query: "/streams/stream"
        onStatusChanged: {
            if (status === XmlListModel.Ready) {
                var reqPlayer = get(0).requiredplayerVersion;
                //reqPlayer = "1";
                if (reqPlayer != "1")
                    streamSelect.sourceComponent = streamDialog;
                else
                    streamSelect.sourceComponent = hdsError;
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

    Component {
        id: streamDialog
//        Window {
//            width: 720; height: 480
//            visible: true
            StreamDialog { /*anchors.verticalCenter: parent.verticalCenter*/
                programMetaInfo: { 'title': currentArgs.programTitle ? currentArgs.programTitle : "",
                                   'name': currentArgs.programName ? currentArgs.programName : "",
                                   'season': currentArgs.programSeason ? currentArgs.programSeason : "",
                                    'time': currentArgs.programTime ? currentArgs.programTime : "",
                                    'description': currentArgs.programDescription ? currentArgs.programDescription : "" }
            }
        //}
    }

    Component {
        id: hdsError

        Item {
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            width: childrenRect.width
            height: childrenRect.height

            Label {
                text: "Pirateplayer kan inte ladda ner denna typ av strömmar.<br />En <a href=\"" + userSettings.pirateplaySeBase + "/hds_guide.html\">guide</a> finns på Pirateplay.se till hur man laddar ner HDS-strömmar manuellt."
                onLinkActivated: Qt.openUrlExternally(link)
            }
        }
    }

    Component.onDestruction: root.focus = true

    Loader {
        id: streamSelect

        anchors.fill: parent
        //focus: true

        property variant streamsModel: streams
    }

    XmlListModelStatusMessage { target: streams }
}
