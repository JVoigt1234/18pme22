import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1

Text{
    id: myTextId
    font.pointSize: 16;
    color: "black"
    style: Text.Outline;
    styleColor: "black"

    TextField{
        id: myTextFieldId
        anchors.left: myTextId.right;
        anchors.margins: 10;
        Layout.preferredWidth: 250;
        width: myTextId.textFieldWidth;
        placeholderText: qsTr(myTextId.placeholderText);
        font.pointSize: myTextId.font.pointSize;
    }

    //Für äußeren Zugriff
    property string placeholderText : "Text"
    property int textFieldLenght : myTextFieldId.length;
    property string textFieldText : myTextFieldId.text;
    property int textFieldWidth : myTextFieldId.width;

}
