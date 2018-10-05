import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1

import "Schablonen"

Rectangle {

    id: patient;
    anchors {left: parent.left; right: parent.right;}
    height: column.implicitHeight + 5;
    border.width:  2;
    color: "grey"
    border.color: "darkblue";
    radius: 5;

    ColumnLayout{
        spacing: 5;

        MyText{ text: "Name: " + modelData.Name; }
        MyText{ text: "Vorname: " + modelData.Vorname; }
        MyText{ text: "Geschlecht: " + modelData.Geschlecht; }
        MyText{ text: "Geburtstag: " + modelData.Geburtstag; }
    }

}

