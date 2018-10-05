import QtQuick 2.2
import QtQuick.Controls 1.2

ApplicationWindow {
    id: mainWindow
    width: 1080/2; height: 1920/2

    Rectangle {
        id: root
        anchors.fill: parent

        IntroPage { id: introPage }
        PersonPage { id: personPage }
        DailyRoutinePage {id: dailyRoutinePage }
        BreakfastPage {id: breakfastPage }
        LeisureTime {id: leisureTime }
        Sportactivity {id: sportactivity }

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
            initialItem: introPage // Qt.resolvedUrl("IntroPage.qml")         
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

/*
delegate: StackViewDelegate {
    function transitionFinished(properties) {
        properties.exitItem.x = 0
    }

    pushTransition: StackViewTransition {
        PropertyAnimation {
            target: enterItem
            property: "x"
            easing.type: Easing.InOutBack
            duration: 500

            from: enterItem.width
            to: 0
        }
        PropertyAnimation {
            target: exitItem
            property: "x"
            duration: 500
            easing.type: Easing.InOutBack
            from: 0
            to: -exitItem.width/3
        }
    }
    popTransition: StackViewTransition {

        PropertyAnimation {
            target: enterItem
            property: "x"
            duration: 500
            easing.type: Easing.InOutBack
            from: -enterItem.width
            to: 0
        }
        PropertyAnimation {
            target: exitItem
            property: "x"
            duration: 500
            easing.type: Easing.InOutBack
            from: 0
            to: exitItem.width
        }
    }
}
*/

