#include <QGuiApplication>
#include <QtQuick>
#include "chatserver.h"
#include "websocketclient.h"

int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);

    //Verbindung zwischen C++ und QML
    //Für WebSocketClient
    qmlRegisterType<WebSocketClient>("WebSocket",1,0, "WebSocketClient");
    //Für ChatServer
    qmlRegisterType<ChatServer>("Server", 1,0,"ChatServer");


    QScopedPointer<QQmlApplicationEngine> engine(new QQmlApplicationEngine);
    engine->addImportPath(":/");

    //muss hier stehen wegen der Engine!!!!
    WebSocketClient chatClient;
    engine->rootContext()->setContextProperty( "chatClient", &chatClient);
    
    engine->load(QUrl("qrc:/main.qml"));

    return app.exec();
}
