import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import "Schablonen"

BasePage {
    height: parent.height; width: parent.width

    titel: qsTr("Freizeit")

    menuButtonsText: [ "back", "next" ]

    Rectangle {
        anchors.fill: parent
        //color: "lightsteelblue"
        Image {
            id: image
            anchors.centerIn: parent
            source: "bike.jpg";
            height: parent.height
            width:  parent.width
        }


        ColumnLayout{
            spacing: 35;
            x: 10
            y: 10

            Rectangle {
                width: 600;
                height: 100;
                color: "transparent"

                ColumnLayout{
                    spacing: 20;

                    Switch{
                        id: switchTVId;
                        MyText{label: "TV"; anchors.left: switchTVId.right; anchors.margins: 10;}
                        checked: true;
                        onCheckedChanged: slidTVId.enabled = spinTVId.enabled = checked;
                    }
                    Slider{
                        id: slidTVId;
                        value: spinTVId.value;
                        maximumValue: 100 - slidFilmId.value - slidSportId.value - slidReadId.value - slidPCId.value - slidVideoId.value;
                        stepSize: 0.25;
                        orientation: Qt.Horizontal;
                        Layout.minimumWidth: 500;
                    }
                    SpinBox{
                        id: spinTVId;
                        value: slidTVId.value;
                        decimals: 2
                        Layout.minimumHeight: 24;
                    }
                }
            }
//------------------------------------------------------------------------------------------
            Rectangle {
                width: 600;
                height: 100;
                color: "transparent"

                ColumnLayout{
                    spacing: 20;
                    Switch{
                        id: switchPCId;
                        MyText{label: "PC"; anchors.left: switchPCId.right; anchors.margins: 10;}
                        checked: true;
                        onCheckedChanged: slidPCId.enabled = spinPCId.enabled = checked;
                    }
                    Slider{
                        id: slidPCId;
                        value: spinPCId.value;
                        maximumValue: 100 - slidFilmId.value - slidSportId.value - slidReadId.value - slidVideoId.value - slidTVId.value;
                        stepSize: 0.25;
                        orientation: Qt.Horizontal;
                        Layout.minimumWidth: 500;
                    }
                    SpinBox{
                        id: spinPCId;
                        value: slidPCId.value;
                        decimals: 2
                        Layout.minimumHeight: 24;
                    }
                }
            }
//------------------------------------------------------------------------------------------
            Rectangle {
                width: 600;
                height: 100;
                color: "transparent"

                ColumnLayout{
                    spacing: 20;
                    Switch{
                        id: switchReadId;
                        MyText{label: "Lesen"; anchors.left: switchReadId.right; anchors.margins: 10;}
                        checked: true;
                        onCheckedChanged: slidReadId.enabled = spinReadId.enabled = checked;
                    }
                    Slider{
                        id: slidReadId;
                        value: spinReadId.value;
                        maximumValue: 100 - slidFilmId.value - slidSportId.value - slidVideoId.value - slidPCId.value - slidTVId.value;
                        stepSize: 0.25;
                        orientation: Qt.Horizontal;
                        Layout.minimumWidth: 500;
                    }
                    SpinBox{
                        id: spinReadId;
                        value: slidReadId.value;
                        decimals: 2
                        Layout.minimumHeight: 24;
                    }
                }
            }
//------------------------------------------------------------------------------------------
            Rectangle {
                width: 600;
                height: 100;
                color: "transparent"

                ColumnLayout{
                    spacing: 20;
                    Switch{
                        id: switchVideoId;
                        MyText{label: "Zocken"; anchors.left: switchVideoId.right; anchors.margins: 10;}
                        checked: true;
                        onCheckedChanged: slidVideoId.enabled = spinVideoId.enabled = checked;
                    }
                    Slider{
                        id: slidVideoId;
                        value: spinVideoId.value;
                        maximumValue: 100 - slidFilmId.value - slidSportId.value - slidReadId.value - slidPCId.value - slidTVId.value;
                        stepSize: 0.25;
                        orientation: Qt.Horizontal;
                        Layout.minimumWidth: 500;
                    }
                    SpinBox{
                        id: spinVideoId;
                        value: slidVideoId.value;
                        decimals: 2
                        Layout.minimumHeight: 24;
                    }
                }
            }
//------------------------------------------------------------------------------------------
            Rectangle {
                width: 600;
                height: 100;
                color: "transparent"

                ColumnLayout{
                    spacing: 20;
                    Switch{
                        id: switchFilmId;
                        MyText{label: "Film schauen"; anchors.left: switchFilmId.right; anchors.margins: 10;}
                        checked: true;
                        onCheckedChanged: slidFilmId.enabled = spinFilmId.enabled = checked;
                    }
                    Slider{
                        id: slidFilmId;
                        value: spinFilmId.value;
                        maximumValue: 100 - slidSportId.value - slidVideoId.value - slidReadId.value - slidPCId.value - slidTVId.value;
                        stepSize: 0.25;
                        orientation: Qt.Horizontal;
                        Layout.minimumWidth: 500;
                    }
                    SpinBox{
                        id: spinFilmId;
                        value: slidFilmId.value;
                        decimals: 2
                        Layout.minimumHeight: 24;
                    }
                }
            }
//------------------------------------------------------------------------------------------
            Rectangle {
                width: 600;
                height: 100;
                color: "transparent"

                ColumnLayout{
                    spacing: 20;
                    Switch{
                        id: switchSportId;
                        MyText{label: "Sport"; anchors.left: switchSportId.right; anchors.margins: 10;}
                        checked: true;
                        onCheckedChanged: slidSportId.enabled = spinSportId.enabled = checked;
                    }
                    Slider{
                        id: slidSportId;
                        value: spinSportId.value;
                        maximumValue: 100 - slidFilmId.value - slidVideoId.value - slidReadId.value - slidPCId.value - slidTVId.value;
                        stepSize: 0.25;
                        orientation: Qt.Horizontal;
                        Layout.minimumWidth: 500;
                    }
                    SpinBox{
                        id: spinSportId;
                        value: slidSportId.value;
                        decimals: 2
                        Layout.minimumHeight: 24;
                    }
                }
            }
//------------------------------------------------------------------------------------------
        }

    }

    onMenuButtonClicked: {
        switch ( buttonIndex ) {
        case 0: mainStackView.pop(); break;
        case 1: mainStackView.push( sportactivity ); break;
        default: console.log("error")
        }
    }
}


