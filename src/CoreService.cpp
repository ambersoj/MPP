#include "CoreService.hpp"
#include "Logger.hpp"
#include <unistd.h>

CoreService::CoreService() : socketManager("/tmp/mpp_core.sock") {}

void CoreService::start() {
    Logger::getInstance().log("MPP-Core starting...");
    socketManager.startListening();
    listenForCommands();
}

void CoreService::listenForCommands() {
    while (true) {
        int clientSock = socketManager.acceptConnection();
        if (clientSock < 0) continue;

        std::string command = socketManager.receiveMessage(clientSock);
        if (!command.empty()) {
            Logger::getInstance().log("Received command: " + command);
            commandProcessor.processCommand(command);
        }

        // 🔥 Ensure we properly close the client socket after use
        if (clientSock >= 0) {
            close(clientSock);
        }
    }
}
