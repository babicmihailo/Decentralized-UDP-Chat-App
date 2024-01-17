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
    void SendMessage(QString message);
    void CleanUp();
};

#endif // SENDER_H
