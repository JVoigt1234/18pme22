import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import "Schablonen"

Rectangle{

  id: stud;
  anchors {left: parent.left; right: parent.right;}
  height: column.implicitHeight + 5;
  border.width:  2;
  color: "grey"
  border.color: "darkblue";
  radius: 5;
  clip: true;

  Row{
     id: row
     spacing: 20;

     Image{
          id: image;
          source: modelData.bild;
          smooth: true;
          opacity: 0.3;
     }

     Column{
          id: column
          MyText{label: 'Vorname: ' + modelData.vorname; }
          MyText{label: 'Nachname: ' + modelData.nachname; }
          MyText{label: 'Kurs: ' + modelData.course; }
          MyText{label: 'Alter: ' + modelData.age; }
          MyText{label: 'Matrikelnummer: ' + modelData.mat_nummer; }
          MyText{label: 'Semester: ' + modelData.semester; }
     }

     ListView{
         id: viewModules;
         model: modelData.modules
         spacing: 10;
         height: 400;
         width: 300;
         visible: false;
         cacheBuffer: 50;
         delegate:
             ColumnLayout{
             id: columnviewModules
                     MyText{label: 'Vorlesung: ' + modelData.cours; }
                     MyText{label: 'Abkürzung: ' + modelData.identificationCode; }
                     MyText{label: 'Schwierigkeit: ' + modelData.severity; }
                     MyText{label: 'Lerngruppe: ' + modelData.lernenGroup; }
                }
         }

  }

  MouseArea{
      anchors.fill:  parent;

      onPressed: {
          parent.scale = 0.9;
          image.opacity = 1 //transparent in %
          image.scale = 0.96
          viewModules.visible = true;
          stud.height = 500;
          stud.color = "white";

      }//onPressed

      onReleased: {
          parent.scale = 1.0;
          image.opacity = 0.3;
          viewModules.visible = false;
          stud.height = column.implicitHeight + 5
          stud.color = "grey";
      }
  }

}
