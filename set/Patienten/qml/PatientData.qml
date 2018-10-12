import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1



BasePage {
    height: parent.height; width: parent.width
    visible:false

    titel: "Patientendaten"
    menuButtonsText: [ "back" , "next" ]

    ListView{
        id: patientView
        width: parent.width;
        height: parent.height * .75;

        delegate: PatientDataDelegate {}
        spacing: 4;
        cacheBuffer: 50;
    }

    Button{
        id: loadData
        width: parent.width;
        height: parent.height * .1
        anchors { bottom: parent.bottom; bottomMargin: 10;}
        text: qsTr("Load Patientdaten");

        onClicked: readFile();
    }


    onMenuButtonClicked: {
        switch ( buttonIndex ) {
        case 0: mainStackView.pop(); break;
        case 1: mainStackView.push(examplePage); break;
        default: console.log("error")
        }
    }

    //Funktion zum lesen der .txt
    function readFile(){
            var request = new XMLHttpRequest()

            request.open('Get', "qrc:/PatientenDaten.txt")

            request.onreadystatechange = function(event) {

                         if(request.readyState == XMLHttpRequest.DONE){
                             console.log(request.responseText)
                             patientView.model = JSON.parse( request.responseText);
                         }
                }
                request.send();
        }
}


