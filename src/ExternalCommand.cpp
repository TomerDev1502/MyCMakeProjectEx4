#include "ExternalCommand.h"
#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
#include <cstring>

ExternalCommand::ExternalCommand(const std::vector<std::string>& args) : args(args) {}

void ExternalCommand::execute() {
    std::vector<char*> argv;
    for (const auto& arg : args) {
        argv.push_back(const_cast<char*>(arg.c_str()));
    }
    argv.push_back(nullptr);

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        return;
    }

    if (pid == 0) {
        execv(argv[0], argv.data());
        perror("exec failed");
        exit(1);
    } else {
        waitpid(pid, nullptr, 0);
    }
}
