import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import EulerThread 1.0

ApplicationWindow {
    width: 700
    height: 500

    Connections{
        target: eulerThread

        onEulerFinish: { textID.text = "Eulerzahl:\n" + result; }
    }

    Image{source: "pic.jpg"; anchors.fill: parent}

    Text{
        id: textID;
        y: parent.height*.63;
        anchors.leftMargin: 75;
        font.pixelSize: 24;
        color: "red"
    }

    Button{
        id: buttonID;
        x: parent.width / 2.517985611;
        y: parent.height / 1.34408602
        width: 144
        height: 30;

        onClicked: { eulerThread.calcEuler(); eulerThread.writef(); buttonID.visible = false;}

        Text{
            text: "Euler-Berechnen";
            anchors.verticalCenterOffset: 0
            anchors.horizontalCenterOffset: 1
            font.pixelSize: buttonID.height*.6
            anchors.centerIn: buttonID
        }
    }

    Component.onCompleted: show()
}
