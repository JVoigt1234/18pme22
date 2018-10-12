import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1

//import "mybutton.qml" //zurerst . dann gibt er Vorschläge
//import "../qml"

ApplicationWindow {
    id: window
    width: 500
    height: 200

    Image{
        source: "hs-mannheim.jpg";
        width: parent.width;
        y: 0;
        x: 0;
    }

    Row{
        x: ((window.width-300)/3)/2
        y: 120;
        spacing: (window.width-300)/3;

        MyButton{
            id: button1
            label: "Button 1"
            state: "on"

            onButtonClick: {console.log(label + " clicked " + state) }      //on steht dafür das ich buttonClick aufruft, in der main noch on davor und den ersten Burchstaben groß
        }

        MyButton{
            id: button2
            label: "Button 2"
            state: "on"

            onButtonClick: {console.log(label + " clicked " + state) }
        }

        MyButton{
            id: button3
            label: "Button 3"
            state: "on"

            onButtonClick: {console.log(label + " clicked " + state) }
        }

    }


    Component.onCompleted: show()
}
