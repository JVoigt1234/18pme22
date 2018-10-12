import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1

Text{
    id: myTextId
    text: qsTr(myTextId.label)
    font.pixelSize: 50;
    color: "red"
    style: Text.Outline;
    font.weight: Font.DemiBold;
    styleColor: "black"
    property string label : "Text"
}
