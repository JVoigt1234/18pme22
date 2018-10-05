#ifndef CHATSERVER_H
#define CHATSERVER_H

//#include <QObject>
#include <QtCore>
#include <QtWebSockets>

class ChatServer : public QObject
{
    Q_OBJECT

    QWebSocketServer m_server;
    QSet<QWebSocket*> m_connectedClients;
    QHash<QWebSocket*, QJsonObject> m_clientInfos;

public:
    explicit ChatServer(QObject *parent = 0);
    virtual ~ChatServer();

signals:

protected slots:
    void onNewConnection();
    void onNewMessage(QWebSocket *client, const QByteArray &message);

    QJsonObject createErrorMessage(const QString& errorString);

public slots:
    void sendMessage(QWebSocket *client, const QJsonObject &messageObject);
    void broadcastMessage(const QJsonObject &messageObject);
};

#endif // CHATSERVER_H
