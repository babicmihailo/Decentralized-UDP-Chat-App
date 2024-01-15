#include "receiver.h"
#include <iostream>

Receiver::Receiver(Client *client)
    : client(client)
{
    //moveToThread(QCoreApplication::instance()->thread());
    //socketBroadcast.bind(client->broadcastAddress, 0);
    //qDebug() << client->getSocket().state();
    //connect(client->getClientSocket(), SIGNAL(readyRead()), this, SLOT(processBroadcast()));
}

void Receiver::run()
{
    exec();
}

void Receiver::processBroadcast()
{
    std::cout << "receiver processBroadcast";
    /*QByteArray datagram;
    QHostAddress sender;
    quint16 senderPort;
    while (client->getClientSocket()->hasPendingDatagrams()) {

        datagram.resize(client->getClientSocket()->pendingDatagramSize());
        client->getClientSocket()->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
        QString data = QString::fromUtf8(datagram);
        emit receiveBroadcast(senderPort, data);
    }*/
}
