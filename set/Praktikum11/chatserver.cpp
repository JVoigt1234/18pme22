#include "chatserver.h"
#include <algorithm>

ChatServer::ChatServer(QObject *parent) : QObject(parent)
  ,m_server(QStringLiteral("chatserver"), QWebSocketServer::NonSecureMode)      //was sagt das Komma??
{
    if(qEnvironmentVariableIsSet("PORT") && !qEnvironmentVariableIsEmpty("PORT")) {
        m_server.listen(QHostAddress::Any, qgetenv("PORT").toInt());
    }
    else{
        m_server.listen(QHostAddress::Any, 8080);   //Port festlegen über welchen kommuniziert wird
    }

    QObject::connect(&m_server, &QWebSocketServer::newConnection, this, &ChatServer::onNewConnection);
}

ChatServer::~ChatServer()
{

}

void ChatServer::onNewConnection()
{
    while(m_server.hasPendingConnections() ){
        QWebSocket* connectedClient = m_server.nextPendingConnection();

        //disconnect handler
        QObject::connect(connectedClient, &QWebSocket::disconnected, [this, connectedClient]() {
            m_connectedClients.remove(connectedClient);
            m_clientInfos.remove(connectedClient);
            qDebug() << "Client disconnected:" << connectedClient->peerAddress().toString();
            connectedClient->deleteLater();
        });

        //new data handler
        QObject::connect(connectedClient, &QWebSocket::textMessageReceived, [this, connectedClient](const QString& message){
            onNewMessage(connectedClient, message.toUtf8() );
        });

        m_connectedClients.insert(connectedClient);
        qDebug() << "Client connected:" << connectedClient->peerAddress().toString();
        QJsonArray clientList;
        Q_FOREACH(QJsonObject clientInfo, m_clientInfos.values() ){
            clientList << clientInfo;
        }
        sendMessage(connectedClient, { {QStringLiteral("registredUsers"), QJsonValue(clientList)} } );

    }
}

void ChatServer::onNewMessage(QWebSocket *client, const QByteArray &message)
{
    QJsonParseError parseError;
    QJsonObject messageObject = QJsonDocument::fromJson(message, &parseError).object();
    if(parseError.error != QJsonParseError::NoError){
        sendMessage(client, createErrorMessage(parseError.errorString() ) );
        return;
    }

    //check if it is a 'login info' message
    if(messageObject.value(QStringLiteral("type")).toString() == QStringLiteral("login")) {
        QJsonObject clientInfos = messageObject.value(QStringLiteral("content")).toObject();
        clientInfos.insert(QStringLiteral("user"), messageObject.value(QStringLiteral("user")));
        clientInfos.insert(QStringLiteral("address"),client->peerAddress().toString());
        clientInfos.insert(QStringLiteral("login"), QDateTime::currentDateTime().toString() );
        m_clientInfos.insert(client, clientInfos);
    }

    //messages of type 'text message from a user are just broadcasted
    else if(messageObject.value(QStringLiteral("type")).toString() == QStringLiteral("message")) {
        messageObject.insert(QStringLiteral("timestamp"), QDateTime::currentDateTime().toString());
        broadcastMessage(messageObject);
    }
    else{
        sendMessage(client, createErrorMessage(QStringLiteral("Unkown message type")));
    }
}

QJsonObject ChatServer::createErrorMessage(const QString &errorString)
{
    return{
        {QStringLiteral("type"), QStringLiteral("error") },
        {QStringLiteral("timestamp"), QDateTime::currentDateTime().toString() },
        {QStringLiteral("content"), errorString }
    };
}

void ChatServer::sendMessage(QWebSocket *client, const QJsonObject &messageObject)
{
    QByteArray message = QJsonDocument(messageObject).toJson(QJsonDocument::Compact);
    client->sendTextMessage(QString::fromUtf8(message));
}

void ChatServer::broadcastMessage(const QJsonObject &messageObject)
{
    std::for_each(m_connectedClients.constBegin(), m_connectedClients.constEnd(), [this, messageObject] (QWebSocket* client) {
        sendMessage(client, messageObject);
    });

}
