import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import "Schablonen"

BasePage {
    height: parent.height; width: parent.width

    titel: "Daily Routine"

    menuButtonsText: [ "back", "next" ]

    Rectangle {
        anchors.fill: parent
        //color: "lightsteelblue"
        Image{
            id: image;
            anchors.centerIn: parent;
            source: "dailywork.jpg";
            width: parent.width
            height: parent.height
        }

        ColumnLayout{
            x: parent.width/10;
            y: parent.height*0.3;
            spacing: 20

            MyText{
                id: morning
                label: "Aufgestanden:";
                //placeholderText: "z.B. 06:00";
                Slider{
                    id: slidMorning;
                    anchors{left: morning.right; margins: 10;}
                    maximumValue: 24;
                    stepSize: .25;
                    orientation: Qt.Horizontal;
                    Layout.minimumWidth: 300;

                    MyText{
                        label: slidMorning.value + " Uhr";
                        anchors {left: slidMorning.right; margins: 10;}
                    }
                }
            }

            MyText{
                id: house
                label: "Haus verlassen um:";
                //placeholderText: "z.B. 07:00";
                Slider{
                    id: slidhouse;
                    anchors{left: house.right; margins: 10;}
                    maximumValue: 24;
                    stepSize: .25;
                    orientation: Qt.Horizontal;
                    Layout.minimumWidth: 300;

                    MyText{
                        label: slidhouse.value + " Uhr";
                        anchors {left: slidhouse.right; margins: 10;}
                    }
                }
            }

            MyText{
                id: work
                label: "Arbeitsbeginn:";
                //placeholderText: "z.B. 08:00";
                Slider{
                    id: slidwork;
                    anchors{left: work.right; margins: 10;}
                    maximumValue: 24;
                    stepSize: .25;
                    orientation: Qt.Horizontal;
                    Layout.minimumWidth: 300;

                    MyText{
                        label: slidwork.value + " Uhr";
                        anchors {left: slidwork.right; margins: 10;}
                    }
                }
            }

            MyText{
                id: lunch
                label: "Mittagessen:";
                //placeholderText: "z.B. 12:00";
                Slider{
                    id: slidlunch;
                    anchors{left: lunch.right; margins: 10;}
                    maximumValue: 24;
                    stepSize: .25;
                    orientation: Qt.Horizontal;
                    Layout.minimumWidth: 300;

                    MyText{
                        label: slidlunch.value + " Uhr";
                        anchors {left: slidlunch.right; margins: 10;}
                    }
                }
            }

            MyText{
                id: endwork
                label: "Feierabend:";
                //placeholderText: "z.B. 16:00";
                Slider{
                    id: slidendwork;
                    anchors{left: endwork.right; margins: 10;}
                    maximumValue: 24;
                    stepSize: .25;
                    orientation: Qt.Horizontal;
                    Layout.minimumWidth: 300;

                    MyText{
                        label: slidendwork.value + " Uhr";
                        anchors {left: slidendwork.right; margins: 10;}
                    }
                }
            }

            MyText{
                id: dinner
                label: "Abendessen:";
                //placeholderText: "z.B. 19:00";
                Slider{
                    id: sliddinner;
                    anchors{left: dinner.right; margins: 10;}
                    maximumValue: 24;
                    stepSize: .25;
                    orientation: Qt.Horizontal;
                    Layout.minimumWidth: 300;

                    MyText{
                        label: sliddinner.value + " Uhr";
                        anchors {left: sliddinner.right; margins: 10;}
                    }
                }
            }

            MyText{
                id: sleep
                label: "Schlafen:";
                //placeholderText: "z.B. 22:30";
                Slider{
                id: slidsleep;
                anchors{left: sleep.right; margins: 10;}
                maximumValue: 24;
                stepSize: 0.25;
                orientation: Qt.Horizontal;
                Layout.minimumWidth: 300;

                MyText{
                label: slidsleep.value + " Uhr";
                anchors {left: slidsleep.right; margins: 10;}
                }
              }

            }
        }
    }

    onMenuButtonClicked: {
        switch ( buttonIndex ) {
        case 0: mainStackView.pop(); break;
        case 1: mainStackView.push(breakfastPage); break;
        default: console.log("error")
        }
    }
}

