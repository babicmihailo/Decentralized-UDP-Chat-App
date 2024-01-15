#include "sender.h"
#include <iostream>

Sender::Sender() : textStream(stdin){}

void Sender::run()
{
    while(1)
    {
        std::string userInput;
        std::getline(std::cin, userInput);
        //QString userInput = textStream.readLine();
        //emit sendMessage(userInput);
        emit sendMessage(QString::fromStdString(userInput));
        QThread::sleep(1000);
        std::cout << "SALJEM" << std::endl;
    }
}
