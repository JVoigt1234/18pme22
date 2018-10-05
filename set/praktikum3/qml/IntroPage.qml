import QtQuick 2.2
import "Schablonen"

BasePage {
    height: parent.height; width: parent.width
    visible:false

    titel: "Intro"
    menuButtonsText: [ "next" ]

    Timer { interval: 1000; running: true; onTriggered: mainStackView.push( personPage ); }

    Rectangle {
        anchors.fill: parent
        color: "blue"

        Image {
            id: image
            anchors.centerIn: parent
            source: "qrc:/hmannheim_1.jpg";
            scale: 0.9
        }

        MyText{
            anchors.top: image.bottom
            /*text:"intro Page"
            font.pixelSize: 80;
            color: "white"*/
            label: "intro Page";
            font.pixelSize: 80;
        }
    }

    onMenuButtonClicked: {
        if ( buttonIndex == 0 )
            mainStackView.push( personPage );
    }
}

