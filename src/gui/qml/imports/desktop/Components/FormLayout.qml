import QtQuick 1.1

Column {
    property Component currentComponent
    property int labelColumnWidth: 0
    property bool customLabelColumnWidth: false

    //columns: 2
    spacing: 10

    onChildrenChanged: {
        var lastChild = children[children.length-1];
        labelColumnWidthChanged.connect(lastChild.widthUpdate)
        lastChild.Component.completed.connect(function () {
            if (!customLabelColumnWidth)
                labelColumnWidth = Math.max(lastChild.labelWidth, labelColumnWidth);
        });
    }
}
