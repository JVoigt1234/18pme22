import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import "Schablonen"

BasePage {
    height: parent.height; width: parent.width

    titel: "Personal"

    menuButtonsText: [ "back", "next" ]

    Rectangle {
        anchors.fill: parent
        //color: "lightsteelblue"
        Image {
            id: image
            anchors.centerIn: parent
            source: "rossi.jpg";
            height: parent.height
            width:  parent.width
        }

        /*Text{
            id: textId
            anchors.centerIn: parent
            text:"person Page"
            font.pixelSize: 24
        }*/

        ColumnLayout{
            x: parent.width/10;
            y: parent.height*0.3;
            spacing: 30

            MyText_TextField{
                label: "Vorname:";
                placeholderText: "z.B. Max";
            }

            MyText_TextField{
                label: "Familienname:";
                placeholderText: "z.B Mustermann";
            }

            MyText_TextField{
                label: "Geburtstag:";
                placeholderText: "z.B. 01.01.2000";
            }

            MyText_TextField{
                label: "Geschlecht:";
                placeholderText: "z.B. männlich"
            }

        }

    }

    onMenuButtonClicked: {
        switch ( buttonIndex ) {
        case 0: mainStackView.pop(); break;
        case 1: mainStackView.push( dailyRoutinePage ); break;
        default: console.log("error")
        }
    }
}

