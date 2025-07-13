#pragma once

#include "Command.h"
#include "Job.h"
#include <string>
#include <vector>

class ExternalCommand : public Command {
public:
  
    ExternalCommand(const std::vector<std::string>& args,
                    bool background,
                    std::vector<Job>& jobs);

    void execute() override;

private:
    std::vector<std::string> args_;
    bool background_;
    std::vector<Job>& jobs_;

    std::string findExecutable(const std::string& cmd);
};
