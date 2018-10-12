import QtQuick 2.2

Rectangle {
    id: menuId
    height: 80; radius: 20; color:"#888888";
    border.color: "lightsteelblue"

    property variant buttonsText: []
    property variant buttonsIcons: []
    property int buttonCount: buttonsText.length

    signal buttonClicked( int buttonIndex );

    Row{

        Repeater {
            model: buttonCount

            Rectangle {
                width: menuId.width/buttonCount
                height: menuId.height
                border.width: 1
                border.color: "lightsteelblue"
                color: "#888888"
                Text{
                    anchors.centerIn: parent
                    text: buttonsText[ index ]
                }

                MouseArea {
                    anchors.fill: parent
                    onPressed: buttonClicked( index )
                }
            }
        }
    }
}

