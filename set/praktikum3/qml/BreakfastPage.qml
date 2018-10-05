import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import "Schablonen"

BasePage {
    height: parent.height; width: parent.width

    titel: "Brakfast (have a break have a KitKat)"

    menuButtonsText: [ "back", "next" ]

    Rectangle {
        anchors.fill: parent
        //color: "lightsteelblue"
        Image {
            id: image
            anchors.centerIn: parent
            source: "breakfast.jpg";
            height: parent.height
            width:  parent.width
        }

        ColumnLayout{
            x: parent.width/10;
            y: parent.height*0.1;
            spacing: 30

            MyText_TextField{
                label: "Getrunken:"
                placeholderText: "z.B. Kaffee"
            }
//------------------------------------------------------------------------------------------
            MyText{
                id: muesliId
                label: "Müsli:";

                RowLayout{
                     anchors.left: muesliId.right
                     anchors.margins: 50
                     x: parent.width * .01;


                     ExclusiveGroup {id: exclusiveGroup1;}

                     RadioButton{
                         text: "true";
                         checked: true;
                         exclusiveGroup: exclusiveGroup1;
                     }

                     RadioButton{
                         text: "false";
                         exclusiveGroup: exclusiveGroup1;
                     }
                 }
            }
//------------------------------------------------------------------------------------------
            MyText{
                id: breadId
                label: "Brot:"

                RowLayout{
                     anchors.left: breadId.right
                     anchors.margins: 50
                     x: parent.width * .01;

                     ExclusiveGroup {id: exclusiveGroup2;}

                     RadioButton{
                         text: "true";
                         checked: true;
                         exclusiveGroup: exclusiveGroup2;
                     }

                     RadioButton{
                         text: "false";
                         exclusiveGroup: exclusiveGroup2;
                     }
                 }
            }
//------------------------------------------------------------------------------------------
            MyText{
                id: eggId
                label: "Ei:"

                RowLayout{
                     anchors.left: eggId.right
                     anchors.margins: 50
                     x: parent.width * .01;

                     ExclusiveGroup {id: exclusiveGroup3;}

                     RadioButton{
                         text: "true";
                         checked: true;
                         exclusiveGroup: exclusiveGroup3;
                     }

                     RadioButton{
                         text: "false";
                         exclusiveGroup: exclusiveGroup3;
                     }
                 }
            }
//------------------------------------------------------------------------------------------
            MyText{
                id: eatId
                label: "Marmelade:"

                RowLayout{
                     anchors.left: eatId.right
                     anchors.margins: 50
                     x: parent.width * .01;

                     ExclusiveGroup {id: exclusiveGroup4;}

                     RadioButton{
                         text: "true";
                         checked: true;
                         exclusiveGroup: exclusiveGroup4;
                     }

                     RadioButton{
                         text: "false";
                         exclusiveGroup: exclusiveGroup4;
                     }
                 }
            }
//------------------------------------------------------------------------------------------
            MyText{
                id: kaeseId
                label: "Käse:"

                RowLayout{
                     anchors.left: kaeseId.right
                     anchors.margins: 50
                     x: parent.width * .01;

                     ExclusiveGroup {id: exclusiveGroup5;}

                     RadioButton{
                         text: "true";
                         checked: true;
                         exclusiveGroup: exclusiveGroup5;
                     }

                     RadioButton{
                         text: "false";
                         exclusiveGroup: exclusiveGroup5;
                     }
                 }
            }
//------------------------------------------------------------------------------------------
            MyText{
                id: honeyId
                label: "Honig:"

                RowLayout{
                     anchors.left: honeyId.right
                     x: parent.width * .01;
                     anchors.margins: 50

                     ExclusiveGroup {id: exclusiveGroup6;}

                     RadioButton{
                         text: "true";
                         checked: true;
                         exclusiveGroup: exclusiveGroup6;
                     }

                     RadioButton{
                         text: "false";
                         exclusiveGroup: exclusiveGroup6;
                     }
                 }
            }
//------------------------------------------------------------------------------------------
        }

    }

    onMenuButtonClicked: {
        switch ( buttonIndex ) {
        case 0: mainStackView.pop(); break;
        case 1: mainStackView.push( leisureTime ); break;
        default: console.log("error")
        }
    }
}
