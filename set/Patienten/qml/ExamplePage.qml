import QtQuick 2.0

BasePage {
    height: parent.height; width: parent.width
    visible:false

    titel: "ExamplePage"
    menuButtonsText: [ "back"]

    Rectangle{
        anchors.fill: parent;

        Text{
            anchors.centerIn: parent;
            text: "ExamplePage";
            font.pointSize: 20;
        }
    }

    onMenuButtonClicked: {
        switch ( buttonIndex ) {
        case 0: mainStackView.pop(); break;
        //case 1: mainStackView.push(/*nächste Seite*/); break;
        default: console.log("error")
        }
    }
}

