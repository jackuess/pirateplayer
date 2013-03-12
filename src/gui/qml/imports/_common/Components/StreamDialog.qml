import QtQuick 1.1
import Components 1.0
import QtDesktop 0.1

Flickable {
    property variant programMetaInfo: { "title": "", "name": "", "season": "", "time": "", "description": "" }
    anchors.fill: parent
    contentHeight: column.height

    MouseArea {
        anchors.fill: parent
        onClicked: forceActiveFocus()
    }

    Column {
        property variant selectedStream: qualityBox.model.get(qualityBox.selectedIndex)

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
                text: "Spara eller spela upp ström"
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
                        if (column.selectedStream.suffixHint == "mp4")
                            return "MPEG4 (*.mp4);;TeleStream (*.ts)";
                        else if (column.selectedStream.suffixHint == "flv")
                            return "Flashvideo (*.flv)";
                        else
                            return "Alla filer (*.*)";
                    }
                    width: parent.width
                    path: {
                        function replaceSpecialChars(s) {
                            return s.replace(/[/\\?]/g, '_');
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
                        newFileName = userSettings.startDir + "/" + newFileName + "." + streamsModel.get(0).suffixHint;
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
                visible: column.selectedStream.subtitles

                CheckBox {
                    id: saveSubs
                    text: "Spara undertexter (till: " + (pathIsDir(fileName.text) ? "" : baseName(fileName.path)) + "." + suffix(column.selectedStream.subtitles) + ")"
                    width: parent.width
                }
            }
            FormRow {
                label: "Kvalitet"

                ComboBox {
                    id: qualityBox
                    model: streamsModel
                }
            }

            ToggleBox {
                label: "timeralternativ"
                width: parent.width

                FormLayout {
                    labelColumnWidth: formLayout.labelColumnWidth; customLabelColumnWidth: true
                    FormRow {
                        label: "Fördröjning"

                        TimerField { id: delay }
                    }
                    FormRow {
                        label: "Längd"

                        TimerField { id: duration }
                    }
                    FormRow {
                        label: ""
                        Label { text: "Ange <em>noll</em> för obestämd längd" }
                    }
                }
            }

            FormRow {
                label: ""

                Button {
                    id: playBtn
                    text: "Spela upp"

                    onClicked: play(column.selectedStream.url, column.selectedStream.subtitles, userSettings.playerCmd)
                }
                Button {
                    id: downloadBtn
                    text: "Starta nedladdning"
                    enabled: !isDir.when && !isEmpty.when && !isWritable.when

                    onClicked: {
                        downloadStack.addDownload(column.selectedStream.url, fileName.path, delay.ms, duration.ms);
                        if (saveSubs.checked) {
                            var subFilePath = absDir(fileName.path) + nativeSeparator() + baseName(fileName.path) + "." + suffix(column.selectedStream.subtitles);
                            downloadStack.addDownload(column.selectedStream.subtitles, subFilePath, delay.ms, duration.ms);
                        }
                    }
                }
            }
        }
    }
}
