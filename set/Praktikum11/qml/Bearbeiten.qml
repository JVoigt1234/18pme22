import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Dialogs 1.1
import QtQuick.Layouts 1.1

import Server 1.0
import WebSocket 1.0
import "Schablonen"

Rectangle{

    MyText_TextField{
        id: textId;
        //Layout
        y: parent.height * .3;
        x: parent.width * .15;
        //Text
        text: qsTr("Nutzername:");
        placeholderText: "Nutzername ändern"
    }//MyText_TextField

    MyText_TextField{
        id: urlId;
        //Layout
        x: parent.width * .15;
        anchors{top: textId.bottom; topMargin: 50;}
        //Text
        text: qsTr("URL:");
        placeholderText: "URL ändern"
    }//MyText_TextField

    Button{
        id: saveId
        text: qsTr("speichern");
        anchors.bottom: parent.bottom;
        height: 30;
        width: parent.width;
        onClicked: {
            if(textId.textFieldLenght !== 0){
                chatClient.username = textId.textFieldText;
                textId.textFieldText = "";
                console.log(qsTr("neuer Nutzername: " + chatClient.username) )
            }
            if(urlId.textFieldLenght !== 0){
                chatClient.url = urlId.textFieldText;
                urlId.textFieldText = "";
                console.log(qsTr("Neuer Url: " + chatClient.url) )
            }
            mainStackView.pop();
        }//onClicked
    }//Button

}

