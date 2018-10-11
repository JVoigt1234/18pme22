import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1

import Server 1.0
import WebSocket 1.0

Rectangle {
//Titel
    Text{
        id: titelId
        text: "Chat";
        //Layout
        anchors{top: parent.top; horizontalCenter: parent.horizontalCenter;}
        font.pixelSize: parent.height * .05;
    }

 //Chat
    function send()
    {
        if(messageField.length === 0)
            return;

        chatClient.sendMessage(messageField.text);
        messageField.text = "";
    }


    Connections{
        target: chatClient
        onNewMessageReceived:{
            var message = {'message': messageObject.content, 'user': messageObject.user};            
            chatModel.append(message);  //listmodel
            chatView.positionViewAtEnd();   //listview
        }
    }

   ListModel{
       id: chatModel
       //ListElement{user: "Test1"; message: "was geht?";}
       //ListElement{user: "Test2"; message: "was geht?";}
   }


   ListView{
        id: chatView;
        //Layout
        spacing: 4;
        x: 10;
        width: parent.width * .95;
        height: 300;
        anchors{top: titelId.bottom; topMargin: 10;}
        //ListenInhalt
        model: chatModel;
        delegate: ChatDelegate {}
        //------------delegate End --------------
    }//ListView

//Enter Text & send
    TextField{
        id: messageField
        //Layout
        x: 10;
        anchors{bottom: parent.bottom; bottomMargin: 10;}
        height: parent.height * .05;
        width: parent.width * .8;
        //Text
        placeholderText: qsTr("Enter your Text");
        font.pixelSize: messageField.height/2;

    }//TextField

    Button{
        id: sendId
        text: "Send";
        //Layout
        anchors{left: messageField.right; bottom: parent.bottom; leftMargin: 10; bottomMargin: 10;}
        height: messageField.height;
        width: parent.width * 0.15;
        //event
        onClicked:  send();
    }//Button
}
