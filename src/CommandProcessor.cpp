#include "Utils.hpp"
#include "Logger.hpp"
#include <algorithm>
#include <cctype>
#include "CommandProcessor.hpp"

CommandProcessor::CommandProcessor() {
    Logger::getInstance().log("[DEBUG] Registering commands...");

    commands["start"] = Utils::make_unique<StartCommand>();  // ✅ Fix: Use our custom make_unique
    commands["stop"] = Utils::make_unique<StopCommand>();

    Logger::getInstance().log("[DEBUG] Registered commands:");
    for (const auto& pair : commands) {
        Logger::getInstance().log("[DEBUG] - " + pair.first);
    }
}

void CommandProcessor::processCommand(std::string command) {
    // 🔥 Trim whitespace (leading/trailing spaces)
    command.erase(0, command.find_first_not_of(" \t\n\r"));
    command.erase(command.find_last_not_of(" \t\n\r") + 1);

    Logger::getInstance().log("[CORE] Processing command: '" + command + "'");

    Logger::getInstance().log("[DEBUG] Commands registered at runtime:");
    for (const auto& pair : commands) {
        Logger::getInstance().log("[DEBUG] - '" + pair.first + "'");
    }

    auto it = commands.find(command);
    if (it != commands.end()) {
        Logger::getInstance().log("[DEBUG] Found command in registry: '" + command + "'");
        
        if (it->second) {
            Logger::getInstance().log("[DEBUG] Command object exists, executing now...");
            it->second->execute();
            Logger::getInstance().log("[DEBUG] Execution complete.");
        } else {
            Logger::getInstance().log("[ERROR] Command object is NULL!");
        }
    } else {
        Logger::getInstance().log("[CORE] Unknown command received: '" + command + "'");
    }
}
