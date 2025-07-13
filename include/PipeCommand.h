#pragma once

#include "Command.h"
#include "Job.h"
#include <vector>
#include <string>

/// Executes commands connected by pipes,
/// with optional input‐file (<), output‐file (> or >>), background (&), and job tracking.
class PipeCommand : public Command {
public:
    PipeCommand(const std::vector<std::vector<std::string>>& segments,
                const std::string& inputFile,
                const std::string& outputFile,
                bool append,
                bool background,
                std::vector<Job>& jobs);

    void execute() override;

private:
    std::vector<std::vector<std::string>> segments_;
    std::string inputFile_;
    std::string outputFile_;
    bool append_;
    bool background_;
    std::vector<Job>& jobs_;
};
