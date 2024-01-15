#include <QCoreApplication>
#include <iostream>
#include "receiver.h"
#include "sender.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if (a.arguments().size() < 2) {
        qWarning() << "Usage: " << argv[0] << " <username>";
        return 1;
    }

    QString username = a.arguments().at(1);

    Client client(username);
    client.init();

    //Receiver dataReceiver(&client);
    //dataReceiver.moveToThread(&dataReceiver);
    Sender dataSender;

    //QObject::connect(&dataReceiver, &Receiver::receiveBroadcast, &client, &Client::receiveBroadcast);
    QObject::connect(&dataSender, &Sender::sendMessage, &client, &Client::sendMessage);

    //dataReceiver.start();
    dataSender.start();

    return a.exec();
}
