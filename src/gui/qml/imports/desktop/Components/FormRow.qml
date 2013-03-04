import QtQuick 1.1

Row {
    id: root

    property alias label: lbl.text
    property alias labelWidth: lbl.width
    property alias layoutDirection: stack.layoutDirection
    default property alias children: stack.children

    height: stack.height; width: parent.width

    signal widthUpdate()
    onWidthUpdate: labelWidth = Math.max(labelWidth, parent.labelColumnWidth);

    spacing: 10

    Text {
        id: lbl
        horizontalAlignment: Qt.AlignRight; verticalAlignment: Qt.AlignVCenter
        height: stack.height
    }

    Row {
        id: stack
        width: parent.width-labelWidth-spacing*2; height: childrenRect.height
    }
}
