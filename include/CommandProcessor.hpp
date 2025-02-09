#ifndef COMMANDPROCESSOR_HPP
#define COMMANDPROCESSOR_HPP

#include <unordered_map>
#include <memory>
#include <string>
#include "ICommand.hpp"
#include "Logger.hpp"
#include "Commands.hpp"

class CommandProcessor {
private:
    std::unordered_map<std::string, std::unique_ptr<ICommand>> commands;

public:
    CommandProcessor();
    void processCommand(std::string command);
};

#endif // COMMANDPROCESSOR_HPP
