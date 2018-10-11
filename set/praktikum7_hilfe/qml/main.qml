import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1


ApplicationWindow {
    width: 1024/2
    height: 768/2
    title: qsTr("Eulersche Zahl berechnen   by M.Benz")

    Connections
    {
      target: eulerThread
      onNewEuler:
      {
          euler.text = result;         //eulerThread.writeFile()
      }
    }


Rectangle{
    anchors.fill:parent


     TextField
     {
        id: euler
        placeholderText: qsTr("Eulersche Zahl");
        anchors.centerIn: parent;
        width: 300
     }

     MyText
       {
           id: autor
           anchors.bottom: quit.top
           anchors.left: parent.left
           anchors.bottomMargin: 20;
           anchors.leftMargin: 100
           color: "black"
           label: "Marius Benz 1421645"
       }

       Button
       {
          id: berechnung
          anchors.bottom: quit.top;
          anchors.left: autor.right;
          anchors.bottomMargin: 20;
          anchors.leftMargin: 50
          width: parent.width/5
          height: parent.height/8
          MyText{anchors.centerIn: berechnung; label: "Berechnung";}
          onClicked: {eulerThread.calcEuler(); eulerThread.writef();}
       }

       Button
       {
            id: quit
            anchors.bottom: parent.bottom
            width: parent.width
            height: parent.height/5
            MyText{anchors.centerIn: quit; label: "Quit!";}
            onClicked: Qt.quit()
       }

}

    Component.onCompleted: show()
}

