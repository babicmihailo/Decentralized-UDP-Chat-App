#ifndef RECEIVER_H
#define RECEIVER_H

#include <QObject>
#include <QThread>
#include "client.h"

class Receiver : public QThread
{
    Q_OBJECT
public:
    Receiver(Client* client);

private:
    Client* client;
    QUdpSocket socketBroadcast;

protected:
    void run() override;

private slots:
    void processBroadcast();

signals:
    void receiveBroadcast(quint16 port, QString data);

};

#endif // RECEIVER_H
