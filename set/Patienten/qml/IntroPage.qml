import QtQuick 2.2

BasePage {
    height: parent.height; width: parent.width
    visible:false

    titel: "Intro"
    menuButtonsText: [ "next" ]

    Timer { interval: 1000; running: true; onTriggered: mainStackView.push( patientPage ); }

    Rectangle {
        anchors.fill: parent
        color: "blue"

        Text{
            anchors.centerIn: parent
            /*text:"intro Page"
            font.pixelSize: 80;
            color: "white"*/
            text: "Willkomen bei Sv Patient";
            font.pointSize: 20;
        }
    }

    onMenuButtonClicked: {
        if ( buttonIndex == 0 )
            mainStackView.push( patientPage );
    }
}

