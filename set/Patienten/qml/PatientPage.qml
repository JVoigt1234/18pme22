import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1

import "Schablonen"
import PatientenData 1.0

BasePage {
    height: parent.height; width: parent.width
    visible:false

    titel: "Patientendaten"
    menuButtonsText: [ "back" , "next" ]

    Rectangle{
        anchors.fill: parent;

        Patient{
            id: patient

            name: name.textFieldText;
            vorname: vorname.textFieldText;
            sex: sex.textFieldText;
            birth: birth.textFieldText;
        }

        ColumnLayout{
            anchors {top: parent.top; left: parent.left; topMargin: 20; leftMargin: 30; }
            spacing: 20;
            width: parent.width

            MyText_TextField{
                id: name
                text: qsTr("Name:");

                //TextField
                placeholderText: "Max"
                textFieldWidth: parent.width * .5;
            }//name

            MyText_TextField{
                id: vorname
                text: qsTr("Vorname:");

                //TextField
                placeholderText: "Mustermann"
                textFieldWidth: parent.width * .5;
            }//vorname

            MyText_TextField{
                id: birth
                text: qsTr("Geburtstag:");

                //TextField
                placeholderText: "01.01.1991"
                textFieldWidth: parent.width * .5;
            }//geburtstag

            MyText_TextField{
                id: sex
                text: qsTr("Geschlecht:");

                //TextField
                placeholderText: "männlich"
                textFieldWidth:  parent.width * .5;
            }//sex

        }

        Button{
            id: save
            anchors {bottom: parent.bottom; bottomMargin: 20; }
            height: parent.height * .1;
            width: parent.width;
            text: qsTr("Save as .txt File");
            onClicked: patient.saveData();
        }

    }//Rectangle

    onMenuButtonClicked: {
        switch ( buttonIndex ) {
        case 0: mainStackView.pop(); break;
        case 1: mainStackView.push(patientData); break;
        default: console.log("error")
        }
    }
}

