import QtQuick 1.1
import Components 1.0

Rectangle {
    id: listItem

    property alias text: label.text
    property alias textColor: label.color
    property alias fontBold: label.font.bold
    property alias fontPointSize: label.font.pointSize
    property alias imgSource: img.source

    signal clicked()

    height: Math.max(label.height + 20, 70)
    anchors.left: parent.left
    width: parent.width
    anchors.horizontalCenter: parent.horizontalCenter
    color: "transparent"

    Image {
        id: img

        anchors.verticalCenter: parent.verticalCenter
        anchors.right: parent.right
        anchors.rightMargin: 10
        height: 50
        y: label.height > img.height ? label.height/2 : 10

        fillMode: Image.PreserveAspectFit
    }

    Label {
        id: label
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 10
        width: parent.width - img.width - img.anchors.rightMargin - anchors.leftMargin
        wrapMode: Text.WordWrap
    }

    MouseArea {
        anchors.fill: parent
        onClicked: listItem.clicked()
    }
}
