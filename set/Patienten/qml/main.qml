import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1

ApplicationWindow {
    width: 1024/2
    height: 800/2

    //Page

    Rectangle{
        id: root;
        anchors.fill: parent

    PatientData {id: patientData }
    PatientPage {id: patientPage }
    IntroPage {id: introPage }
    ExamplePage {id: examplePage }

    Rectangle {
                id: titel
                anchors { top: parent.top; left: parent.left; right: parent.right }
                height: 80

                Text {
                    anchors.centerIn: parent
                    text: mainStackView.currentItem.titel
                    font.pointSize: 24
                }
            }

    StackView {
        id: mainStackView
        anchors{ top: titel.bottom; left: parent.left; right: parent.right; bottom: menuPersonPage.top }
        initialItem: introPage;//Qt.resolvedUrl("ExamplePage.qml")
    }

    MyMenu {
                id: menuPersonPage
                anchors{ left: parent.left; right: parent.right; bottom: parent.bottom }
                height: 80
                buttonsText: mainStackView.currentItem.menuButtonsText
                buttonsIcons: mainStackView.currentItem.menuButtonsText
                onButtonClicked: mainStackView.currentItem.menuButtonClicked( buttonIndex )
            }
    }

    Component.onCompleted: show()
}
