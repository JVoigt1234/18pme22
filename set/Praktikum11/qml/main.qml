import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1

import WebSocket 1.0
import Server 1.0
import "Schablonen"

ApplicationWindow {
    id: rootWindow;
    width: 1080/2;
    height: 1920/2
//-------------------------------------------------------------------------------------
    menuBar : MenuBar{
        id: menuBarId
            Menu{
                title: "Datei";
                MenuItem{
                    text: "Close Programm";
                    onTriggered:{
                        Qt.quit();
                        chatClient.close();
                        console.log("Connect: " + chatClient.connecting);
                    }//onTriggered
                }//MenuItem

                MenuItem{
                    text: qsTr("Name & URL ändern");
                    onTriggered:{
                        mainStackView.push(bearbeitenPage);
                    }//onTriggered
                }//MenuItem
            }
    }//MenuBar
//--------------------------------------------------------------------------------------

    LoginPage {id: loginPage }
    ChatPage {id: chatPage }
    Bearbeiten {id: bearbeitenPage}

    WebSocketClient{
        id: chatClient;
        url:"ws://mar-eu-1-a8ytfdvt.qtcloudapp.com";
    }


    StackView {
        id: mainStackView
        anchors.fill: parent

        initialItem: loginPage //Qt.resolvedUrl("LoginPage.qml")
    }

    Component.onCompleted: show()
}
