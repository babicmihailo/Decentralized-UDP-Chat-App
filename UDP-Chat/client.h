#ifndef CLIENT_H
#define CLIENT_H

#define BROADCAST_PORT 12345
#define MAX_BUFFER 1024

#include <QObject>
#include <QtNetwork/QUdpSocket>
#include <QMap>
#include <QDebug>

class Client : public QObject
{
    Q_OBJECT
public:
    Client(QString username);
    void Init();

public slots:
    void SendMessage(QString message);
    void CleanUp();

private:
    QString username;
    QUdpSocket* socketBroadcast;
    QUdpSocket* socketClient;
    QHostAddress address;
    quint16 port;
    QMap<quint16, QString> clients;

    void ReadMessage();
    void ClientNew(quint16 senderPort, QString data);
    void ClientAdd(quint16 senderPort, QString senderUsername);
    void ClientRemove(quint16 senderPort);
    void ClientMessage(quint16 senderPort, QString message);
    void ClientRespond(quint16 senderPort);

private slots:
    void ProcessMessage();
    void ProcessBroadcast();


};

#endif // CLIENT_H
