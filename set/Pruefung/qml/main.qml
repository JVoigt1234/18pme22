import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1

import Fisibubele 1.0

ApplicationWindow {
    width: 1024
    height: 768

    Pruefung{
        id: pruefungId
    }

    StackView {
        id: mainStackView
        anchors.fill: parent

        initialItem: Qt.resolvedUrl("ExamplePage.qml")
    }

    Component.onCompleted: show()
}
