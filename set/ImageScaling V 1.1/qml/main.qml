import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1

import ImageScaling 1.0

ApplicationWindow {
    id: window
    width: 1024
    height: 768

    ImageScaling{
        id: scaling;
        width: horizontal.value;
        height: vertikal.value;

        onWidthChanged: image.width = width;
        onHeightChanged: image.height = height;
    }

    Image{
        id: image
        source: "nature.jpg"
        width: parent.width/2;
        height: parent.height/2;
    }

    Slider{
        id: horizontal
        anchors { bottom: parent.bottom; bottomMargin: 10; leftMargin: 25 }
        width: parent.width - 25;
        maximumValue: window.width;
        stepSize: 1;
        orientation: Qt.Horizontal
    }

    Slider{
        id: vertikal
        anchors {right: parent.right; rightMargin: 10; topMargin: 25 }
        height: parent.height - 25;
        maximumValue: window.height;
        stepSize: 1;
        orientation: Qt.Vertical
    }

    Component.onCompleted: show()
}
