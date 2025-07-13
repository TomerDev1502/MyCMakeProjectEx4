#pragma once
#include "Command.h"
#include "Job.h"
#include <vector>

/// builtin myjobs
class JobsCommand : public Command {
public:
    explicit JobsCommand(std::vector<Job>& jobs) : jobs_(jobs) {}
    void execute() override;

private:
    std::vector<Job>& jobs_;
};
