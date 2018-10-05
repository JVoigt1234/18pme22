import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1

ApplicationWindow {
    width: 1024
    height: 768

    StackView {
        id: mainStackView
        anchors.fill: parent

        initialItem: Qt.resolvedUrl("ExamplePage.qml")
    }

    Component.onCompleted: show()
}
