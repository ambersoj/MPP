#ifndef NETSERVICE_HPP
#define NETSERVICE_HPP

#include "SocketManager.hpp"

class NetService {
private:
    SocketManager socketManager;

public:
    NetService();
    void start();
    void listenForCommands();
    void processCommand(const std::string& command);
};

#endif // NETSERVICE_HPP
