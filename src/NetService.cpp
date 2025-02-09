#include "NetService.hpp"
#include "Logger.hpp"
#include <unistd.h>

NetService::NetService() : socketManager("/tmp/mpp_net.sock") {}

void NetService::start() {
    Logger::getInstance().log("MPP-Net starting...");
    socketManager.startListening();
    listenForCommands();
}

void NetService::listenForCommands() {
    while (true) {
        int clientSock = socketManager.acceptConnection();
        if (clientSock < 0) continue;

        std::string command = socketManager.receiveMessage(clientSock);
        if (!command.empty()) {
            Logger::getInstance().log("Received NET command: " + command);
            processCommand(command);
        }

        // 🔥 Ensure we properly close the client socket after use
        if (clientSock >= 0) {
            close(clientSock);
        }
    }
}

void NetService::processCommand(const std::string& command) {
    if (command == "start") {
        Logger::getInstance().log("[NET] Starting capture...");
    } else if (command == "stop") {
        Logger::getInstance().log("[NET] Stopping capture...");
    } else {
        Logger::getInstance().log("[NET] Unknown command.");
    }
}
