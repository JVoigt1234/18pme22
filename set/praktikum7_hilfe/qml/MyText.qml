import QtQuick 2.0

Text
{
    id: myText
    text: qsTr(myText.label)
    property string label: "Label: "
    font.pixelSize: 16
    color: "black"

}
