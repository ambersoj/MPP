#ifndef SOCKETMANAGER_HPP
#define SOCKETMANAGER_HPP

#include <string>

class SocketManager {
private:
    int serverSocket;
    std::string socketPath;

public:
    explicit SocketManager(const std::string& path);
    ~SocketManager();

    void startListening();
    int acceptConnection();
    void sendMessage(int clientSocket, const std::string& message);
    std::string receiveMessage(int clientSocket);
};

#endif // SOCKETMANAGER_HPP
