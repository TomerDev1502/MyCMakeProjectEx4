#include "RedirectionCommand.h"
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <iostream>
#include <cstdlib>
#include <sstream>

static std::string locateExecutable(const std::string& cmd) {
    if (cmd.find('/') != std::string::npos)
        return cmd;
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

RedirectionCommand::RedirectionCommand(const std::vector<std::string>& args,
                                       const std::string& inputFile,
                                       const std::string& outputFile,
                                       bool append,
                                       bool background,
                                       std::vector<Job>& jobs)
  : args_(args)
  , inputFile_(inputFile)
  , outputFile_(outputFile)
  , append_(append)
  , background_(background)
  , jobs_(jobs)
{}

void RedirectionCommand::execute() {
    auto execPath = locateExecutable(args_[0]);
    if (execPath.empty()) {
        std::cerr << "Command not found: " << args_[0] << "\n";
        return;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return;
    }
    if (pid == 0) {
        // child: set up redirection
        if (!inputFile_.empty()) {
            int in = open(inputFile_.c_str(), O_RDONLY);
            if (in < 0) { perror("open input"); _exit(1); }
            dup2(in, STDIN_FILENO);
            close(in);
        }
        if (!outputFile_.empty()) {
            int flags = O_WRONLY | O_CREAT | (append_ ? O_APPEND : O_TRUNC);
            int out = open(outputFile_.c_str(), flags, 0644);
            if (out < 0) { perror("open output"); _exit(1); }
            dup2(out, STDOUT_FILENO);
            close(out);
        }
        std::vector<char*> argv;
        for (auto& a : args_) argv.push_back(const_cast<char*>(a.c_str()));
        argv.push_back(nullptr);
        execv(execPath.c_str(), argv.data());
        perror("exec failed");
        _exit(1);
    }
    // parent
    if (background_) {
        int id = static_cast<int>(jobs_.size()) + 1;
        std::string cmdLine;
        for (size_t i = 0; i < args_.size(); ++i)
            cmdLine += args_[i] + (i + 1 < args_.size() ? " " : "");
        jobs_.push_back({ id, pid, cmdLine, JobStatus::Running });
        std::cout << "[" << id << "] " << pid << "\n";
    } else {
        waitpid(pid, nullptr, 0);
    }
}
