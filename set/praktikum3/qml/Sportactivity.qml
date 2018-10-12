import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1

BasePage {
    height: parent.height; width: parent.width

    titel: "Sportactivity"

    menuButtonsText: ["back" ]

    ListModel{
        id: sportModel;
        ListElement { activity: "Schwimmen"; date: "10.10.2015" }
        ListElement { activity: "Laufen"; date: "11.10.2015" }
    }

    Rectangle{
        anchors.fill: parent
        //color: "lightsteelblue"
        Image {
            id: image
            anchors.centerIn: parent
            source: "walk.jpg";
            height: parent.height
            width:  parent.width
        }

        ColumnLayout{
                spacing: parent.height * .09;
                y: parent.height * .02;
                x: parent.width * .05;

                Row{
                    spacing: parent.height * .25;

                    ComboBox{
                        id: cBox;
                        editable: false;
                        width: parent.height * .5;

                        model: ["Aerobic","Andere", "Badminton", "Biathlon", "Cross Skating", "Curling", "Eislaufen", "Eisstockschießen", "Ellipsentrainer",
                        "Ergometer", "Frisbee", "Fußball", "Gehen", "Golgen", "Handbiking", "Inline Skaten", "Kajaking", "Kampfsport", "Kite Skiing", "Kite Surfen", "Klettern",
                        "Krafttraining", "Langlaufen", "Laufband", "Laufen", "Motorradfahren", "Mountain Biking", "Nordic Walking", "Paragleiten", "Pilates", "Radfahren",
                        "Reiten", "Rennradfahren", "Rudern", "Schlittenfahren", "Schneeschuhwandern", "Schwimmen", "Segeln", "Skilaufen", "Skitour", "Snowboarding", "Speed Skiing",
                        "Spinning", "Squash", "Surfen", "Tauchen", "Tennis", "Tischtennis", "Wakeboarding", "Wandern", "Windsurfen", "Yoga"]

                        onCurrentIndexChanged: sportModel.append( { activity: cBox.currentText } )
                    }

                    Calendar{
                        weekNumbersVisible: true;
                        onClicked: sportModel.setProperty( sportModel.count -1, "date", date.toLocaleDateString("dd.MM.YYYY")  )        //wichtig macht ein update von der Zeile
                    }
                }

                TableView{
                    width: 400;
                    Layout.fillWidth: true;
                    Layout.fillHeight: true;
                    TableViewColumn{ role: "activity"; title: qsTr("Activity"); width: parent.width/2; }
                    TableViewColumn{ role: "date"; title: qsTr("Datum"); width: parent.width/2; }
                    model: sportModel;
                }

    }

}
    onMenuButtonClicked: {
        if(buttonIndex == 0){
            mainStackView.pop();
        }
    }
}


