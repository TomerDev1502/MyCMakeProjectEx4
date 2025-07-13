#pragma once

#include "Command.h"
#include "Job.h"
#include <memory>
#include <string>
#include <vector>


class CommandParser {
public:
    explicit CommandParser(std::vector<Job>& jobs);
    std::unique_ptr<Command> parse(const std::string& line);

private:
    std::vector<Job>& jobs_;
};
