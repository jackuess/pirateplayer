import QtQuick 1.1

Item {
    id: scrollBar

    property real value: 0.0
    property real minimumValue: 0.0
    property real maximumValue: 1.0
    property int orientation : Qt.Vertical

    property variant target
    property int thickness: 4

    width: orientation == Qt.Vertical ? thickness : parent.width-thickness
    height: orientation == Qt.Vertical ? parent.height-thickness : thickness

    Rectangle {
        id: slider
        x: scrollBar.orientation == Qt.Vertical ? 1 : (target.visibleArea.xPosition * (scrollBar.width-2) + 1)
        y: (scrollBar.height - height) * (scrollBar.value / scrollBar.maximumValue)
        width: {
            if (scrollBar.orientation == Qt.Vertical)
                return parent.width-2;
            else
                return ((target.visibleArea.widthRatio > 0 && target.visibleArea.widthRatio < 1.0) ? target.visibleArea.widthRatio : 1) * (scrollBar.width-2)
        }
        height: scrollBar.maximumValue > 0 ? Math.max(scrollBar.height / scrollBar.maximumValue, 30) : scrollBar.height
        radius: scrollBar.orientation == Qt.Vertical ? (width/2 - 1) : (height/2 - 1)
        color: "#0099cc"
        //opacity: 0.2

        MouseArea {
            anchors.fill: parent
            drag.target: parent
            drag.axis: Drag.YAxis
            drag.minimumY: 0
            drag.maximumY: scrollBar.height - height

            onPositionChanged: {
                if (pressedButtons == Qt.LeftButton)
                    scrollBar.value = slider.y / (scrollBar.height-slider.height) * scrollBar.maximumValue;
            }
        }
    }
}
