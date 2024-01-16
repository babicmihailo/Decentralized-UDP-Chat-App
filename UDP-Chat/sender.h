#ifndef SENDER_H
#define SENDER_H

#include "qtimer.h"
#include <QObject>
#include <QThread>
#include <QTextStream>

class Sender : public QThread
{
    Q_OBJECT
public:
    Sender();

protected:
    void run() override;

private:
    QTextStream textStream;
    QTimer timer;

signals:
    void sendMessage(QString message);
    void cleanup();
};

#endif // SENDER_H
