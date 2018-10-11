import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1

Text{
    id: myTextId
    text: qsTr(label)
    font.pixelSize: 30;
    color: "yellow";
    style: Text.Outline;
    font.weight: Font.Bold;
    styleColor: "black"
    width: 250;

    TextField{
        id: myTextField;
        anchors.left: myTextId.right;
        anchors.margins: 10;
        Layout.fillWidth: true;
        width: 400;
        text: qsTr(myTextId.setTextFieldText)
        placeholderText: qsTr(myTextId.placeholderText);
        font.pixelSize: myTextId.font.pixelSize;
    }

    property string label: "Text"
    property string placeholderText : "Text"
    property string setTextFieldText : myTextField.text
    property string getTextFieldText : myTextField.text;

}

