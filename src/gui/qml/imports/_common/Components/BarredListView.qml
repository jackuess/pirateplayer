import QtQuick 1.1
import Components 1.0

import "../../../common.js" as Common

Item {
    id: pirateListView

    property alias currentIndex: list.currentIndex
    property alias currentItem: list.currentItem
    property alias currentSection: list.currentSection
    property alias delegate: list.delegate
    property alias header: list.header
    property alias model: list.model
    property alias section: list.section

    function positionViewAtIndex (index, mode) { list.positionViewAtIndex(index, mode) }

    focus: true
    Keys.forwardTo: list

    ListView {
        id: list

        property int previousIndex: 0

        onContentYChanged: if (moving) scrollBar.value = contentY
        onContentHeightChanged: if (contentHeight > 0 && (typeof currentArgs !== "undefined") && currentArgs.selectedIndex) currentIndex = currentArgs.selectedIndex

        anchors { left: parent.left; right: scrollBar.left; top: parent.top; bottom: parent.bottom }

        keyNavigationWraps: true

        Keys.onReturnPressed: currentItem.clicked()

        Keys.onPressed: {
            var pageDown = currentIndex+10;
            var pageUp = currentIndex-10;
            if (event.key == Qt.Key_PageDown && event.modifiers == Qt.NoModifier) {
                currentIndex = pageDown >= count ? count-1 : pageDown;
                event.accepted = true;
            }
            if (event.key == Qt.Key_PageUp && event.modifiers == Qt.NoModifier) {
                currentIndex = pageUp < 0 ? 0 : pageUp;
                event.accepted = true;
            }
        }

        highlight: HighlightDelegate { }
        highlightMoveDuration: 500
    }

    ScrollBarWidget {
        id: scrollBar
        anchors { right: parent.right; top: parent.top; bottom: parent.bottom }
    }
}
