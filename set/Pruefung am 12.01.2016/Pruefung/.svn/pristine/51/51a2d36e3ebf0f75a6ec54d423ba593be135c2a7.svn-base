import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1

import Simulator 1.0

ApplicationWindow {
    id: window
    width: 1024
    height: 768

    Simulator{
        id: simu
    }

    Rectangle{
    id: iRect
    anchors.centerIn: parent;
    color: "transparent"

    width: 420;
    height: 105

        Image{
            id: image
            anchors.centerIn: parent;
            source: "qrc:/logo4.gif";

            transform: [
                            Rotation{ origin.x: iRect.width/2; origin.y: iRect.height/2; axis{ x: 1; y:0.0; z: 0.0 } angle: simu.rotX },
                            Rotation{ origin.x: iRect.width/2; origin.y: iRect.height/2; axis{ x: 0; y:1.0; z: 0.0 } angle: simu.rotY },
                            Rotation{ origin.x: iRect.width/2; origin.y: iRect.height/2; axis{ x: 0; y:0.0; z: 1.0 } angle: simu.rotZ }
                        ]

        }//Image

    }//Rectangle

    Row{
        spacing: 10;
        anchors{top: window.top; left: window.left;}

        Text{
            id: xname
            text: "X-Step:";
        }

        SpinBox{
            id: xSpin
            decimals: 2;
            onEditingFinished: simu.stepSizeX(xSpin.value)
        }//x

        Text{
            id: yname
            text: "Y-Step:";
        }

        SpinBox{
            id: ySpin
            decimals: 2;
            onEditingFinished: simu.stepSizeY(ySpin.value)
        }//y

        Text{
            id: zname
            text: "Z-Step:";
        }

        SpinBox{
            id: zSpin
            decimals: 2;
            onEditingFinished: simu.stepSizeZ(zSpin.value)
        }//z
    }

    Component.onCompleted: show()
}
