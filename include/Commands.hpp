#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "ICommand.hpp"
#include "Logger.hpp"

class StartCommand : public ICommand {
public:
    void execute() override {
        Logger::getInstance().log("[CORE] Starting packet capture...");
    }
};

class StopCommand : public ICommand {
public:
    void execute() override {
        Logger::getInstance().log("[CORE] Stopping packet capture...");
    }
};

#endif // COMMANDS_HPP
