import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1

Text{
    id: myTextId
    font.pixelSize: 24;
    color: "black"
    style: Text.Outline;
    styleColor: "black"

    TextField{
        id: myTextFieldId
        anchors.left: myTextId.right;
        anchors.margins: 10;
        Layout.preferredWidth: 250;
        width: 250;
        placeholderText: qsTr(myTextId.placeholderText);
        font.pixelSize: 24;
    }

    //Für äußeren Zugriff
    property string placeholderText : "Text"
    property int textFieldLenght : myTextFieldId.length;
    property string textFieldText : myTextFieldId.text;

}
