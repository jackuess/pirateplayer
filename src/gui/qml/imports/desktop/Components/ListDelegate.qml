import QtQuick 1.1
import Components 1.0

AbstractListDelegate {
    color: [Qt.rgba(0, 0, 0, 0.05),Qt.rgba(0, 0, 0, 0)][index%2]
}
