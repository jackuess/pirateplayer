import QtQuick 1.1
import QtDesktop 0.1

ScrollBar {
    id: scrollBar

    onValueChanged: if (!list.moving && value > 0) list.contentY = value

    orientation: Qt.Vertical
    minimumValue: 0
    maximumValue: list.contentHeight > 0 ? list.contentHeight-list.height : 0

    opacity: list.moving ? 1 : 0
}
