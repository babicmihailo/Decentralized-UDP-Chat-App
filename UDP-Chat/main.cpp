#include <QCoreApplication>
#include "client.h"
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
    client.Init();

    Sender dataSender;
    
    QObject::connect(&dataSender, &Sender::SendMessage, &client, &Client::SendMessage);
    QObject::connect(&dataSender, &Sender::CleanUp, &client, &Client::CleanUp);

    dataSender.start();

    return a.exec();
}
