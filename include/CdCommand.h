#pragma once

#include "Command.h"    // בסיס לכל Command
#include <vector>
#include <string>

class CdCommand : public Command {
public:
    explicit CdCommand(const std::vector<std::string>& args);
    void execute() override;

private:
    std::vector<std::string> args;
};

