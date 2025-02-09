#ifndef ICOMMAND_HPP
#define ICOMMAND_HPP

#include <string>

class ICommand {
public:
    virtual ~ICommand() = default;
    virtual void execute() = 0;
};

#endif // ICOMMAND_HPP
