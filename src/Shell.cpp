#include "Shell.h"
#include "CommandParser.h"
#include <iostream>
#include <sys/wait.h>

void Shell::checkJobs() {
    int status;
    while (true) {
        pid_t done = waitpid(-1, &status, WNOHANG);
        if (done <= 0) break;
        for (auto& job : jobs_) {
            if (job.pid == done && job.status == JobStatus::Running) {
                job.status = JobStatus::Done;
                std::cout << "\n[" << job.id << "] Done " << job.cmd << "\n";
            }
        }
    }
}

void Shell::run() {
    std::string line;
    while (true) {
        std::cout << "$ ";
        if (!std::getline(std::cin, line)) break;

        checkJobs();

        if (auto cmd = parser_.parse(line)) {
            cmd->execute();
        }
    }
}
