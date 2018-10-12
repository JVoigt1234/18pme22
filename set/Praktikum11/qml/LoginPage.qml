import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Dialogs 1.1
import QtQuick.Layouts 1.1

import Server 1.0
import WebSocket 1.0
import "Schablonen"

Rectangle {
    id: root
    anchors.fill: parent;

    MyText_TextField{
        id: textId;
        //Layout
        anchors.verticalCenter: parent.verticalCenter;
        x: parent.width * .2;
        //Text
        text: qsTr("Nutzername:");
        placeholderText: "Max Mustermann"
    }

    Button{
        id: loginId
        //Layout
        anchors.top: textId.bottom;
        anchors.topMargin: 20;
        x: parent.width * .15;
        height: 30;
        width: parent.width * .5;
        //Text
        text: "Login";
        onClicked: {
            if(textId.textFieldLenght !== 0){
                chatClient.username = textId.textFieldText;
                chatClient.open();
                console.log("Connecting: " + chatClient.connecting);
                console.log("Connected: " + chatClient.connected);
                console.log(qsTr("Nutzername: ") + chatClient.username);
                console.log("Url: " + chatClient.url );
                mainStackView.push(chatPage);

            }//if
        }//onClicked
    }//Button

}
