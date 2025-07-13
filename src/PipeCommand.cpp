#include "PipeCommand.h"
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <iostream>
#include <cstdlib>
#include <sstream>   // for istringstream

// Helper to locate an executable in $PATH
static std::string locateExecutable(const std::string& cmd) {
    if (cmd.find('/') != std::string::npos) return cmd;
    const char* pathEnv = getenv("PATH");
    if (!pathEnv) return "";
    std::istringstream ss(pathEnv);
    std::string dir;
    while (std::getline(ss, dir, ':')) {
        std::string full = dir + "/" + cmd;
        if (access(full.c_str(), X_OK) == 0)
            return full;
    }
    return "";
}

PipeCommand::PipeCommand(const std::vector<std::vector<std::string>>& segments,
                         const std::string& inputFile,
                         const std::string& outputFile,
                         bool append,
                         bool background,
                         std::vector<Job>& jobs)
  : segments_(segments)
  , inputFile_(inputFile)
  , outputFile_(outputFile)
  , append_(append)
  , background_(background)
  , jobs_(jobs)
{}

void PipeCommand::execute() {
    int n = (int)segments_.size();
    if (n == 0) return;

    // 1) create pipes
    std::vector<int> fds(2 * (n - 1));
    for (int i = 0; i < n - 1; ++i)
        pipe(&fds[2*i]);

    // 2) fork each segment
    for (int i = 0; i < n; ++i) {
        pid_t pid = fork();
        if (pid == 0) {
            // child: set up stdin from inputFile_ on first segment
            if (i == 0 && !inputFile_.empty()) {
                int in = open(inputFile_.c_str(), O_RDONLY);
                if (in < 0) { perror("open inputFile"); _exit(1); }
                dup2(in, STDIN_FILENO);
                close(in);
            }
            // child: connect previous pipe -> stdin
            if (i > 0) {
                dup2(fds[2*(i-1)], STDIN_FILENO);
            }
            // child: connect stdout -> next pipe OR to outputFile_
            if (i < n - 1) {
                dup2(fds[2*i + 1], STDOUT_FILENO);
            } else if (!outputFile_.empty()) {
                int flags = O_WRONLY | O_CREAT | (append_ ? O_APPEND : O_TRUNC);
                int out = open(outputFile_.c_str(), flags, 0644);
                if (out < 0) { perror("open outputFile"); _exit(1); }
                dup2(out, STDOUT_FILENO);
                close(out);
            }
            // close all pipe fds
            for (int fd : fds) close(fd);

            // exec
            const auto& args = segments_[i];
            std::string execPath = locateExecutable(args[0]);
            if (execPath.empty()) {
                std::cerr << "Command not found: " << args[0] << "\n";
                _exit(1);
            }
            std::vector<char*> argv;
            for (auto& a : args)
                argv.push_back(const_cast<char*>(a.c_str()));
            argv.push_back(nullptr);
            execv(execPath.c_str(), argv.data());
            perror("execv failed");
            _exit(1);
        }
        // parent continues
    }

    // 3) parent: close fds
    for (int fd : fds) close(fd);

    // 4) wait or record background
    if (!background_) {
        for (int i = 0; i < n; ++i)
            wait(nullptr);
    } else {
        int id = (int)jobs_.size() + 1;
        jobs_.push_back({ id, -1, "<pipeline>", JobStatus::Running });
        std::cout << "[" << id << "] <pipeline>\n";
    }
}
