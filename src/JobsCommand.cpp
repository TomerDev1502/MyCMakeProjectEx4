#include "JobsCommand.h"
#include <iostream>

void JobsCommand::execute() {
    for (const auto& job : jobs_) {
        std::cout << "[" << job.id << "] "
                  << job.pid << " "
                  << (job.status == JobStatus::Running ? "Running" : "Done")
                  << " " << job.cmd << "\n";
    }
}
