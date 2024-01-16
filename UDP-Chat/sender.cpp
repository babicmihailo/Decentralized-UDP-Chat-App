#include "sender.h"
#include <iostream>

Sender::Sender() : textStream(stdin){}

void Sender::run()
{
    while(1)
    {
        std::string userInput;
        std::getline(std::cin, userInput);
        QString message = QString::fromStdString(userInput);
        if(message.startsWith("quit"))
        {
            emit cleanup();
            break;
        }
        else
        {
            emit sendMessage(message);
        }
    }
    quit();
}
