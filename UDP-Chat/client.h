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
    void init();

public slots:
    void sendMessage(QString message);
    void cleanup();

private:
    QString username;
    QUdpSocket* socketBroadcast;
    QUdpSocket* socketClient;
    QHostAddress address;
    quint16 port;
    QMap<quint16, QString> clients;

    void readMessage();
    void clientNew(quint16 senderPort, QString data);
    void clientAdd(quint16 senderPort, QString senderUsername);
    void clientRemove(quint16 senderPort);
    void clientMessage(quint16 senderPort, QString message);
    void clientRespond(quint16 senderPort);

private slots:
    void processMessage();
    void processBroadcast();


};

#endif // CLIENT_H
