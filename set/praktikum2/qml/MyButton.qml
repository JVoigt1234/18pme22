import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1

Rectangle{
    id: button;
    height: 50;
    width: 100;
    radius: 5;
    color: "red";
    property string label : "Button"
    //property var onButtonClick //was für Datentyp ist onButtonClick??     //var = variant (The var type is a generic property type that can refer to any data type)
    signal buttonClick


    Text {
           id: buttonText
           //text: button.label     //nimmt den Text wo ich ihm hinter label: in der main hinschreibe
           anchors.centerIn: parent
           font.pixelSize: parent.height * .25
           style: Text.Sunken        //Schriftart????
           color: "black"            //Textfarbe
           styleColor: "black"      //Umrandung von dem Text
       }

    Behavior on scale{ NumberAnimation {duration: 200} }

    states:[
        State{
            name: "on"
            PropertyChanges { target:buttonText; text: button.label + " on"}
        },

        State{
            name: "off";
            PropertyChanges { target:buttonText; text: button.label + " off"; color: "white"}
            PropertyChanges { target: button; color: "blue";}
        },

        State{
            name: "disable"
            PropertyChanges { target:buttonText; text: button.label + " disable"; color: "grey";}
            PropertyChanges { target: button; color: "blue"; scale: 0.9; /*enabled: false;*/ }     //enabled = false sperrt Button
        }

    ]



    /*transitions:[
        Transition{
            from: "off"; to: "on"
            ColorAnimation {    duration: 200 }
        },

        Transition{
            from: "on"; to: "off"
            ColorAnimation {    duration: 200 }
        },

        Transition{
            from: "*"; to: "disable"
            ColorAnimation {    duration: 200 }
        }
    ]*/

    MouseArea{
        anchors.fill: parent;           //sagt wo ich überall klicken darf!!!!
        onClicked:{ switch(button.state){
                       case "on": button.state = "off";
                           break;
                       case "off": button.state = "disable";
                           break;
                       case "disable": button.state = "on";
                           break;
                       default: visible = false; // :D
                   }
            button.buttonClick()
        }
    }


}

