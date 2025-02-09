#ifndef CORESERVICE_HPP
#define CORESERVICE_HPP

#include "SocketManager.hpp"
#include "CommandProcessor.hpp"

class CoreService {
private:
    SocketManager socketManager;
    CommandProcessor commandProcessor;

public:
    CoreService();
    void start();
    void listenForCommands();
};

#endif // CORESERVICE_HPP
