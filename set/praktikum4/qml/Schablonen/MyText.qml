import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1

Text{
    id: myTextId
    text: qsTr(myTextId.label)
    font.pixelSize: 15;
    color: "black"
    property string label : "Text"
}
