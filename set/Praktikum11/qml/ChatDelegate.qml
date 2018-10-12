import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1

Rectangle{
    id: chatId

    height: 50;
    border.width:  10;
    border.color: "black";
    color: "red"
    radius: 5;


    Text{
        id: userId
        //Layout
        x: parent.width * .4
        anchors{top: parent.top; }
        font.pixelSize: messageField.height * .5
        text: user;
    }//Text

    Text{
        id: messageId
        //Layout
        anchors{top: userId.bottom; topMargin: 10;}
        x: 10;
        font.pixelSize: messageField.height * .3
        text: message;
    }//Text

}//Rectangle

