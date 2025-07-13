#pragma once
#include "Command.h"
#include <vector>
#include <string>

/// builtin exit
class ExitCommand : public Command {
public:
    explicit ExitCommand(const std::vector<std::string>& args) {}
    void execute() override;
};
