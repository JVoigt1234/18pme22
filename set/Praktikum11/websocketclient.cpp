#include "websocketclient.h"

WebSocketClient::WebSocketClient(QObject *parent) : QObject(parent)
{
    QObject::connect(&m_socket, &QWebSocket::stateChanged, this, &WebSocketClient::stateChanged);

    QObject::connect(&m_socket, &QWebSocket::textMessageReceived, [this](const QString &message ){
        QJsonObject messageObject = QJsonDocument::fromJson(message.toUtf8()).object();
        if(messageObject.isEmpty())
            return;

        if(messageObject.value(QStringLiteral("type")) == QStringLiteral("message"))
            emit newMessageReceived(messageObject);
        else
            qDebug() << "Unhandled message recived:" << messageObject;
    } );    //runde Klammer von connect!!!!

}

WebSocketClient::~WebSocketClient()
{

}

void WebSocketClient::sendMessage(const QString &textMessage)
{
    QJsonObject messageObject;
    messageObject.insert(QStringLiteral("type"), QStringLiteral("message"));
    messageObject.insert(QStringLiteral("user"), getusername());
    messageObject.insert(QStringLiteral("content"), textMessage);
    m_socket.sendTextMessage( QString::fromUtf8(QJsonDocument(messageObject).toJson() ));
}

void WebSocketClient::open()
{
    m_socket.open(m_url);
}

void WebSocketClient::close()
{
    m_socket.close();
}

bool WebSocketClient::getconnecting() const
{
    return m_socket.state() == QAbstractSocket::ConnectingState;
}

bool WebSocketClient::getconnected() const
{

    return m_socket.state() == QAbstractSocket::ConnectedState;
}

QString WebSocketClient::getusername() const
{
    return m_username;
}

void WebSocketClient::setUsername(QString username)
{
    if(m_username == username)
        return;

    m_username = username;
    emit usernameChanged(username);
}

QString WebSocketClient::geturl() const
{
    return m_url;
}

void WebSocketClient::setUrl(QString url)
{
    if(m_url == url)
        return;

    m_url = url;
    emit urlChanged(url);
}
