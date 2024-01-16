#include "sender.h"
#include <iostream>

Sender::Sender() : textStream(stdin){}

void Sender::run()
{
    while(1)
    {
        std::string userInput;
        std::getline(std::cin, userInput);
        emit sendMessage(QString::fromStdString(userInput));
    }
}
