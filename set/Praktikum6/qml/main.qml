import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import NumberConvert 1.0
import "Schablonen"

ApplicationWindow {
    width: 1000
    height: 400

//_______________________________________________________________________________________________________________________
    menuBar : MenuBar{
         Menu{
             title: "Datei";
             MenuItem{ text: "Close Programm"; onTriggered: Qt.quit();}
         }

         Menu{
             title: "Background";
             MenuItem{ text: "Nature"; onTriggered: {nature.visible = true; it.visible = false; moto.visible = false; audi.visible = false;} }
             MenuItem{ text: "IT"; onTriggered: {it.visible = true; nature.visible = false; moto.visible = false; audi.visible = false;} }
             MenuItem{ text: "Motorsport"; onTriggered: {moto.visible = true; it.visible = false; nature.visible = false; audi.visible = false;} }
             MenuItem{ text: "Audi"; onTriggered: {audi.visible = true; moto.visible = false; it.visible = false; nature.visible = false;} }
         }
    }

    Image{id: nature; source: "pic/nature.jpg"; anchors{fill: parent} visible: true; }
    Image{id: it; source: "pic/it.jpg"; anchors{fill: parent;} visible: false; }
    Image{id: moto; source: "pic/motorsport.jpg"; anchors{fill: parent;} visible: false; }
    Image{id: audi; source: "pic/audi.jpg"; anchors{fill: parent;} visible: false; }
//_______________________________________________________________________________________________________________________

    NumberConverter{
        id: numberConverterID;
        bin: textbin.getTextFieldText;
        dec: textdec.getTextFieldText;
        oct: textoct.getTextFieldText;
        hex: texthex.getTextFieldText;

        onBinChanged: textbin.setTextFieldText = bin;
        onDecChanged: textdec.setTextFieldText = dec;
        onOctChanged: textoct.setTextFieldText = oct;
        onHexChanged: texthex.setTextFieldText = hex;
    }

    MyText{
        id: titel;
        anchors{topMargin: 20; horizontalCenter: parent.horizontalCenter;}
        label: "NumberConverter";
        font.underline: true;
    }

    Column{
        spacing: 30;
        anchors{top: titel.bottom; topMargin: 100; fill: parent; leftMargin: 25; }
        
        MyText_TextField{
            id: textbin;
            label: "Binary: ";
            placeholderText: "0000 0111 1101 1111";
            //setTextFieldText: numberConverterID.bin;
        }

        MyText_TextField{
            id: textdec;
            label: "Decimal: ";
            placeholderText: "2015";
            //setTextFieldText: numberConverterID.dec;
        }

        MyText_TextField{
            id: textoct;
            label: "Octal: ";
            placeholderText: "3 737";
            //setTextFieldText: numberConverterID.oct;
        }

        MyText_TextField{
            id:texthex;
            label: "Hexadecimal: ";
            placeholderText: "7DF";
            //setTextFieldText: numberConverterID.hex;
        }
    }

    Component.onCompleted: show()
}
