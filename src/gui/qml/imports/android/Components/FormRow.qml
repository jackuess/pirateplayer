import QtQuick 1.1

Column {
    id: column

    property alias label: lbl.text
    property alias labelWidth: lbl.width
    property alias layoutDirection: stack.layoutDirection
    default property alias children: stack.children

    width: parent.width; height: childrenRect.height

    property int childWidth: width/children.length

    spacing: 5

    Component.onCompleted: {
        for (var i=0; i<children.length; i++) {
            Qt.createQmlObject( "import QtQuick 1.1; \
        Binding { \
                target: children[" + i + "]; \
                property: \"width\"; \
                value: stack.width/children.length; \
        }", column);
        }
    }

    Label {
        id: lbl
        height: text !== "" ? implicitHeight : 0; width: parent.width
    }

    Row {
        id: stack
        width: parent.width; //height: childrenRect.height
    }
}
