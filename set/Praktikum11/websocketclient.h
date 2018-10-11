#ifndef WEBSOCKETCLIENT_H
#define WEBSOCKETCLIENT_H

#include <QObject>
#include <QtCore>
#include <QtWebSockets/QtWebSockets>
#include <QDebug>

class WebSocketClient : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool connecting READ getconnecting NOTIFY stateChanged)
    Q_PROPERTY(bool connected READ getconnected NOTIFY stateChanged)
    Q_PROPERTY(QString username READ getusername WRITE setUsername NOTIFY usernameChanged)
    Q_PROPERTY(QString url READ geturl WRITE setUrl NOTIFY urlChanged)

    QWebSocket m_socket;
    QString m_username;
    QString m_url;

public:
    explicit WebSocketClient(QObject *parent = 0);
    virtual ~WebSocketClient();

    Q_INVOKABLE void sendMessage(const QString& textMessage);
    Q_INVOKABLE void open();
    Q_INVOKABLE void close();

    bool getconnecting() const;
    bool getconnected() const;

    QString getusername() const;
    void setUsername(QString username);

    QString geturl() const;
    void setUrl(QString url);

signals:
    void newMessageReceived(QJsonObject messageObject);
    void stateChanged();
    void usernameChanged(QString username);
    void urlChanged(QString url);

public slots:
};

#endif // WEBSOCKETCLIENT_H
