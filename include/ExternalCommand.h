#pragma once

#include "Command.h"
#include <string>
#include <vector>

class ExternalCommand : public Command {
public:
    ExternalCommand(const std::vector<std::string>& args);
    void execute() override;

private:
    std::vector<std::string> args;

    std::string findExecutable(const std::string& cmd);
};
