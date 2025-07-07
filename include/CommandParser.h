#pragma once

#include "Command.h"
#include <memory>
#include <string>

class CommandParser {
public:
    std::unique_ptr<Command> parse(const std::string& line);
};
