#include "client.h"
#include <iostream>

Client::Client(QString username)
{
    this->username = username;
    this->address = QHostAddress::LocalHost;
    this->socketClient = new QUdpSocket(this);
    this->socketBroadcast = new QUdpSocket(this);

    if (!socketClient->bind(address, 0)) {
        qDebug() << "Error binding client socket:" << socketClient->errorString();
    }

    if (!socketBroadcast->bind(address, BROADCAST_PORT, QAbstractSocket::ReuseAddressHint | QAbstractSocket::ShareAddress)) {
        qDebug() << "Error binding broadcast socket:" << socketBroadcast->errorString();
    }

    this->port = socketClient->localPort();

    connect(socketClient, SIGNAL(readyRead()), this, SLOT(processMessage()));
    connect(socketBroadcast, SIGNAL(readyRead()), this, SLOT(processBroadcast()));
}

Client::~Client()
{
    QByteArray data;
    data.clear();
    data.append("DISCONNECT");
    socketClient->writeDatagram(data, address, BROADCAST_PORT);
    socketClient->close();
    socketBroadcast->close();
}

void Client::init()
{
    QByteArray data;
    data.clear();
    data.append("DISCOVERY=");
    data.append(username.toUtf8());
    qint64 written = socketClient->writeDatagram(data, address, BROADCAST_PORT);
    if (written == -1)
    {
        qDebug() << "Error sending datagram: " << socketClient->errorString();
    }
    else
    {
        qDebug() << "Datagram sent sucessfully. written: " << written;
    }
}

void Client::processBroadcast()
{
    QByteArray datagram;
    QHostAddress sender;
    quint16 senderPort;
    while (socketBroadcast->hasPendingDatagrams()) {
        datagram.resize(socketBroadcast->pendingDatagramSize());
        socketBroadcast->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
        if (senderPort != port)
        {
            QString data = QString::fromUtf8(datagram);
            if (data.startsWith("DISCOVERY="))
            {
                clientNew(senderPort, data);
            }
            if (data.startsWith("DISCONNECT"))
            {
                clientRemove(senderPort);
            }
        }
    }
}

void Client::processMessage()
{
    qint64 datagramSize;
    QHostAddress address;
    quint16 senderPort = 0;

    while (socketClient->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagramSize = socketClient->pendingDatagramSize();
        datagram.resize(datagramSize);
        socketClient->readDatagram(datagram.data(), datagram.size(), &address, &senderPort);
        QString data = QString::fromUtf8(datagram);

        if (data.startsWith("DISCOVERY_RESP="))
        {
            clientAdd(senderPort, data);
        }
        else if (data.startsWith("MSG="))
        {
            clientMessage(senderPort, data);
        }
    }
}

void Client::clientRemove(quint16 senderPort)
{
    if (clients.contains(senderPort))
    {
        QString removedUsername = clients.value(senderPort);
        clients.remove(senderPort);

        std::cout << removedUsername.toStdString() << " :DISCONNECTED" << std::endl;;
    }
    else
    {
        qDebug() << "No client found with port:" << senderPort;
    }
}

void Client::clientNew(quint16 senderPort, QString data)
{
    QString senderUsername = data.mid(10);
    if (!clients.contains(senderPort) || clients.value(senderPort) != senderUsername)
    {
        clients.insert(senderPort, senderUsername);
        std::cout << senderUsername.toStdString() << " :NEW USER" << std::endl;
        clientRespond(senderPort);
    }
    else
    {
        qDebug() << "Client with port:" << port << "and username:" << senderUsername << "already exists.";
    }
}

void Client::clientAdd(quint16 senderPort, QString senderUsername)
{
    senderUsername = senderUsername.mid(15);
    if (!clients.contains(senderPort) || clients.value(senderPort) != senderUsername)
    {
        std::cout << senderUsername.toStdString() << " :EXISTING USER" << std::endl;
        clients.insert(senderPort, senderUsername);
    }
}

void Client::clientMessage(quint16 senderPort, QString message)
{
    message = message.mid(4);
    QString senderUsername = clients.value(senderPort);
    std::cout << message.toStdString() << " :" << senderUsername.toStdString() << std::endl;
}

void Client::sendMessage(QString message)
{
    QByteArray data;
    data.clear();
    data.append("MSG=");
    data.append(message.toUtf8());
    for (auto it = clients.constBegin(); it != clients.constEnd(); ++it) {
        quint16 port = it.key();
        socketClient->writeDatagram(data, address, port);
    }
}

void Client::clientRespond(quint16 senderPort)
{
    QByteArray data;
    data.clear();
    data.append("DISCOVERY_RESP=");
    data.append(username.toUtf8());
    socketClient->writeDatagram(data, address, senderPort);
}

