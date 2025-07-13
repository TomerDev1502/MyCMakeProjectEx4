#pragma once

#include "Command.h"
#include "Job.h"
#include <string>
#include <vector>

/// Executes a single command with optional input/output redirection and background support.
class RedirectionCommand : public Command {
public:
    RedirectionCommand(const std::vector<std::string>& args,
                       const std::string& inputFile,
                       const std::string& outputFile,
                       bool append,
                       bool background,
                       std::vector<Job>& jobs);

    void execute() override;

private:
    std::vector<std::string> args_;
    std::string inputFile_;
    std::string outputFile_;
    bool append_;
    bool background_;
    std::vector<Job>& jobs_;
};
