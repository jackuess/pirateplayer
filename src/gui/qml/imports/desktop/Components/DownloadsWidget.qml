import QtQuick 1.1
import QtDesktop 0.1

Item {
    property alias model: table.model
    property int delegateHeight: 30

    TableView {
        id: table

        property bool anythingSelected: currentIndex > -1

        anchors { left: parent.left; right: actionButtons.left; top: parent.top; bottom: parent.bottom; margins: 5 }

        TableColumn{ role: "statusCol" ; title: "Status" }
        TableColumn{ role: "fileName" ; title: "Filnamn" }
        TableColumn{ role: "progressCol" ; title: "Förlopp" }
        TableColumn{ role: "downloadedSize" ; title: "Nedladdade (MB)" }

        itemDelegate: Loader {
            property variant _itemValue: itemValue
            property variant _itemForeground: itemForeground

            sourceComponent: {
                if (role == "progressCol")
                    return progressDelegate;
                else if(role == "statusCol")
                    return statusDelegate;
                else
                    return standardDelegate;
            }
        }


    }

    Component {
        id: standardDelegate

        Item {
            height: delegateHeight

            Text {
                anchors.verticalCenter: parent.verticalCenter
                color: _itemForeground
                elide: Text.ElideRight
                text: _itemValue
            }
        }
    }
    Component {
        id: progressDelegate

        Item {
            height: delegateHeight
            ProgressBar {
                anchors.fill: parent
                anchors.margins: 4

                minimumValue: 0; maximumValue: 100
                value: typeof _itemValue == "string" ? 0 : _itemValue

                Text {
                    anchors.verticalCenter: parent.verticalCenter; anchors.horizontalCenter: parent.horizontalCenter
                    text: _itemValue + "%"
                }
            }
        }
    }
    Component {
        //        enum Status {
        //            NotStarted = 0,
        //            Downloading,
        //            Finished,
        //            Error,
        //            Aborted
        //        };

        id: statusDelegate

        Item {
            property variant status: ["Ej startad", "Laddar ner", "Färdig", "Fel uppstod", "Abruten"]
            height: delegateHeight

            Text {
                anchors.verticalCenter: parent.verticalCenter
                x: 5
                color: _itemForeground
                elide: Text.ElideRight
                text: status[_itemValue]
            }
        }
    }

    DownloadActionButtons {
        id: actionButtons
        anchors.top: parent.top
        anchors.right: parent.right
    }
}
