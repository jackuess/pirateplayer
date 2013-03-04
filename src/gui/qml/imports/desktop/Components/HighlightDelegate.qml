import QtQuick 1.1

Rectangle {
    color: "#0099cc"
    radius: 3
    width: list.width

    onYChanged:  {
        var animationFinished = y == currentItem.y;
        if (list.contentY > scrollBar.value && animationFinished)
            scrollBar.value = list.contentY + currentItem.height;
        else if (list.contentY < scrollBar.value && animationFinished)
            scrollBar.value = list.contentY -currentItem.height;
    }
}
