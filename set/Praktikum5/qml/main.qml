import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import QtMultimedia 5.0
import "Schablonen"
import Power 1.0

ApplicationWindow {
    id: appWindow
    width: 1024
    height: 400


    PowerSupply{
        id: powerSupply
        volt: slidVolt.value;
        power: slidPower.value;
        watt: slidWatt.value;

        onVoltChanged: { slidVolt.value = volt; textVolt.label = volt + " mV"; }
        onPowerChanged: { slidPower.value = power; textPower.label = power + " mA"; }
        onWattChanged: {slidWatt.value = watt; textWatt.label = watt + " mWat"; }
    }


    MyText{
        //anchors.centerIn: parent;
        //anchors.top: appWindow.top;
        x: parent.width/3;
        id: titel
        font.pixelSize: 50;
        color: "red"
        label: "Power Supply"
    }


    ColumnLayout{
        spacing: 20;
        anchors {top: titel.bottom; topMargin: 50;}
        x: parent.width/20;
        width: parent.width/50;
        height: parent.height/10;

        //Power
            RowLayout{
                spacing: 5;

                MyText{label: "Power: ";}

                Slider{
                    id: slidPower
                    maximumValue: 2000;
                    stepSize: 1
                    orientation: Qt.Horizontal;
                    Layout.minimumWidth: 700;
                    Layout.fillWidth: true;
                }

                MyText{id: textPower;}
            }

            //Volt
            RowLayout{
                spacing: 5;

                MyText{label: "Volt: ";}

                Slider{
                    id: slidVolt
                    maximumValue: 24000;
                    stepSize: 1;
                    orientation: Qt.Horizontal;
                    Layout.minimumWidth:700;
                    Layout.fillWidth: true;
                }

                MyText{id: textVolt;}
            }

            //Wattage-Result
            RowLayout{
                spacing: 5;

                MyText{label: "Wattage: ";}
                MyText{id: textWatt;}
            }

            Slider{
                id: slidWatt;
                orientation: Qt.Horizontal;
                maximumValue: 48000;
                stepSize: 1;
                Layout.minimumWidth: 700
                Layout.fillWidth: true;
            }
        }

    Button{
        id: closeButton
        anchors.bottom: parent.bottom;
        width: appWindow.width;
        height: appWindow.height/5;
        text: qsTr("close");
        onClicked: {timer.running = true;image.visible = true;sound.play();}//Qt.quit();
    }

//Langeweile----------------------------------------
        Image{
            id: image
            source: "pic.jpg"   //pic2
            visible: false;
            width: parent.width;
            height: parent.height;
        }

        SoundEffect{
            id: sound;
            source: "scream.wav";
        }

        Timer{
            id: timer;
            interval: 500;
            onTriggered: {timer3.running = true; timer1.running = true;}
        }

        Timer{
            id: timer1
            interval: 100 //in ms
            onTriggered: {image.visible = true; timer2.running = true;}
            repeat: true;
        }

        Timer{
            id: timer2;
            interval: 60;
            onTriggered: image.visible = false;
        }

        Timer{
            id: timer3
            interval: 2000; //in ms
            onTriggered: Qt.quit();
        }

    Component.onCompleted:
        show()
}
