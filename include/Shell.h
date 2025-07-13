#pragma once

#include <vector>
#include <string>
#include "CommandParser.h"
#include "Job.h"

class Shell {
public:
    void run();

private:
    void checkJobs();

    std::vector<Job> jobs_;
    CommandParser parser_{ jobs_ };
};
