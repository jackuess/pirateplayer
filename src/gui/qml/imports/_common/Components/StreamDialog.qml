import QtQuick 1.1
import Components 1.0
import QtDesktop 0.1

Flickable {
    property variant programMetaInfo: { "title": "", "name": "", "season": "", "time": "", "description": "" }
    property string videoPageUrl: ""
    anchors.fill: parent
    contentHeight: column.height

    MouseArea {
        anchors.fill: parent
        onClicked: forceActiveFocus()
    }

    Column {
// quality
//        property variant selectedStream: qualityBox.model.get(qualityBox.selectedIndex)

        id: column
        width: parent.width; height: childrenRect.height
        spacing: 20

        Rectangle {
            color: Qt.rgba(0,0,0, .1)
            radius: 5
            width: parent.width; height: headerLabel.implicitHeight+15
            Label {
                id: headerLabel
                anchors.verticalCenter: parent.verticalCenter; anchors.horizontalCenter: parent.horizontalCenter
                font.bold: true
                text: "Spara media"
            }
        }

        FormLayout {
            id: formLayout
            width: parent.width-15

            FormRow {
                label: "Fil att spara till"

                FilePathField {
                    id: fileName
                    filter: {
                        return "MP4 (*.mp4)";
                    }
                    width: parent.width
                    path: {
                        function replaceSpecialChars(s) {
                            return s.replace ? s.replace(/[/\\?!:<>*\"|]/g, '_') : "";
                        }

                        var title = replaceSpecialChars(programMetaInfo.title);
                        var name = replaceSpecialChars(programMetaInfo.name);
                        var season = replaceSpecialChars(programMetaInfo.season);
                        var time = replaceSpecialChars(programMetaInfo.time);
                        var description = replaceSpecialChars(programMetaInfo.description);

                        var newFileName = userSettings.filenameTemplate.replace(/%title%/g, title);
                        newFileName = newFileName.replace(/%name%/g, name);
                        newFileName = newFileName.replace(/%season%/g, season);
                        newFileName = newFileName.replace(/%time%/g, time);
                        newFileName = newFileName.replace(/%description%/g, description);
                        newFileName = userSettings.startDir + "/" + newFileName + ".mp4";
                        newFileName = pathToNativeSeparators(newFileName);
                        return newFileName;
                    }
                }
            }
            FormRow {
                Message {
                    text: "Filen existerar redan och kommer skrivas över."
                    state: "info"
                    when: pathExists(fileName.path) && !isDir.when
                }
            }
            FormRow {
                Message {
                    id: isDir
                    text: "Angiven sökväg är en katalog!"
                    state: "error"
                    when: pathIsDir(fileName.path)
                }
            }
            FormRow {
                Message {
                    id: isEmpty
                    text: "Ange ett filnamn att spara till."
                    state: "info"
                    when: fileName.path === ""
                }
            }
            FormRow {
                Message {
                    id: isWritable
                    text: "Du har inte rättigheter att skriva till angiven fil!"
                    state: "error"
                    when: !pathIsWritable(fileName.path)
                }
            }

            FormRow {
                label: ""
                visible: true

                CheckBox {
                    id: saveSubs
                    text: "Spara undertexter"
                    width: parent.width
                }
            }

// quality
//            FormRow {
//                label: "Kvalitet"
//
//                ComboBox {
//                    id: qualityBox
//                    model: streamsModel
//                }
//            }

            FormRow {
                label: ""

                Button {
                    id: downloadBtn
                    text: "Starta nedladdning"
                    enabled: !isDir.when && !isEmpty.when && !isWritable.when

                    onClicked: {
                        var out = fileName.path
							.replace(/\.mp4$/i, "")
							.replace(/[åäöÅÄÖ\.]/g, function(match) {
                            return {
								"å": "a",
								"ä": "a",
								"ö": "o",
								"Å": "A",
								"Ä": "A",
								"Ö": "O",
								".": "_"
							}[match];
						});
                        var xhr = new XMLHttpRequest;
                        var url = 
						    "http://localhost:1199/download"
							+ "?url=" + encodeURIComponent(videoPageUrl)
							+ "&out=" + encodeURIComponent(out)
							+ "&sub=" + (saveSubs.checked ? "true" : "false");
                        xhr.open("GET", url);
                        xhr.send();

// old/quality
//                        downloadStack.addDownload(column.selectedStream.url, fileName.path, delay.ms, duration.ms);
//                        if (saveSubs.checked) {
//                            var subFilePath = absDir(fileName.path) + nativeSeparator() + baseName(fileName.path) + "." + suffix(column.selectedStream.subtitles).replace("?", "");
//                            downloadStack.addDownload(column.selectedStream.subtitles, subFilePath, delay.ms, duration.ms);
//                        }
                    }
                }
            }
        }
    }
}
