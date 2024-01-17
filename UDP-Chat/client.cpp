#include "client.h"
#include "qcoreapplication.h"
#include <QVariant>

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

void Client::Init()
{
    QByteArray data;
    data.clear();
    data.append("DISCOVERY=");
    data.append(username.toUtf8());
    socketBroadcast->setSocketOption(QAbstractSocket::MulticastTtlOption, QVariant(1));
    socketBroadcast->joinMulticastGroup(QHostAddress("239.255.0.1"));

    qint64 written = socketClient->writeDatagram(data, QHostAddress::Broadcast, BROADCAST_PORT);
    if (written == -1)
    {
        qDebug() << "Error sending datagram: " << socketClient->errorString();
    }
}

void Client::CleanUp()
{
    QByteArray data;
    data.clear();
    data.append("DISCONNECT");
    qint64 written = socketClient->writeDatagram(data, QHostAddress::Broadcast, BROADCAST_PORT);
    if (written == -1)
    {
        qDebug() << "Error sending datagram: " << socketClient->errorString();
    }
    socketClient->close();
    socketBroadcast->close();
    QCoreApplication::quit();
}

void Client::ProcessBroadcast()
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
                ClientNew(senderPort, data);
            }
            if (data.startsWith("DISCONNECT"))
            {
                ClientRemove(senderPort);
            }
        }
    }
}

void Client::ProcessMessage()
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
            ClientAdd(senderPort, data);
        }
        else if (data.startsWith("MSG="))
        {
            ClientMessage(senderPort, data);
        }
    }
}

void Client::ClientRemove(quint16 senderPort)
{
    if (clients.contains(senderPort))
    {
        QString removedUsername = clients.value(senderPort);
        clients.remove(senderPort);
        qDebug().noquote().nospace() << "[-] " << removedUsername;
    }
    else
    {
        qDebug() << "No client found with port:" << senderPort;
    }
}

void Client::ClientNew(quint16 senderPort, QString data)
{
    QString senderUsername = data.mid(10);
    if (!clients.contains(senderPort) || clients.value(senderPort) != senderUsername)
    {
        clients.insert(senderPort, senderUsername);
        qDebug().noquote().nospace() << "[+] " << senderUsername;
        ClientRespond(senderPort);
    }
    else
    {
        qDebug() << "Client with port:" << port << "and username:" << senderUsername << "already exists.";
    }
}

void Client::ClientAdd(quint16 senderPort, QString senderUsername)
{
    senderUsername = senderUsername.mid(15);
    if (!clients.contains(senderPort) || clients.value(senderPort) != senderUsername)
    {
        qDebug().noquote().nospace() << "[=] " << senderUsername;
        clients.insert(senderPort, senderUsername);
    }
}

void Client::ClientMessage(quint16 senderPort, QString message)
{
    message = message.mid(4);
    QString senderUsername = clients.value(senderPort);
    qDebug().noquote().nospace() << "(" << senderUsername <<") >> " << message;
}

void Client::SendMessage(QString message)
{
    QByteArray data;
    data.clear();
    data.append("MSG=");
    data.append(message.toUtf8());
    for (auto it = clients.constBegin(); it != clients.constEnd(); ++it) {
        quint16 port = it.key();
        qint64 written = socketClient->writeDatagram(data, address, port);
        if (written == -1)
        {
            qDebug() << "Error sending datagram: " << socketClient->errorString();
        }
    }
}

void Client::ClientRespond(quint16 senderPort)
{
    QByteArray data;
    data.clear();
    data.append("DISCOVERY_RESP=");
    data.append(username.toUtf8());
    socketClient->writeDatagram(data, address, senderPort);
}
