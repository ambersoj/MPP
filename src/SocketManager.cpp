#include "SocketManager.hpp"
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <cstring>
#include <sys/stat.h>

SocketManager::SocketManager(const std::string& path) : socketPath(path) {
    serverSocket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        std::cerr << "[SOCKET] ERROR: Failed to create socket\n";
        return;
    }

    sockaddr_un addr{};
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, socketPath.c_str(), sizeof(addr.sun_path) - 1);
    unlink(socketPath.c_str()); // Ensure old socket is removed before binding

    if (bind(serverSocket, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        std::cerr << "[SOCKET] ERROR: Could not bind socket\n";
        return;
    }

    chmod(socketPath.c_str(), 0777); // Set full permissions
    listen(serverSocket, 5);
}

SocketManager::~SocketManager() {
    close(serverSocket);
    unlink(socketPath.c_str());
}

void SocketManager::startListening() {
    std::cout << "[SOCKET] Listening on: " << socketPath << std::endl;
}

int SocketManager::acceptConnection() {
    sockaddr_un clientAddr{};
    socklen_t clientAddrLen = sizeof(clientAddr);
    int clientSock = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
    if (clientSock < 0) {
        std::cerr << "[SOCKET] ERROR: Failed to accept connection\n";
    }
    return clientSock;
}

void SocketManager::sendMessage(int clientSocket, const std::string& message) {
    send(clientSocket, message.c_str(), message.size(), 0);
}

std::string SocketManager::receiveMessage(int clientSocket) {
    char buffer[512] = {0};
    ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    
    if (bytesRead < 0) {
        std::cerr << "[SOCKET] ERROR: Failed to receive message\n";
        return "";
    }

    buffer[bytesRead] = '\0';
    return std::string(buffer);
}
