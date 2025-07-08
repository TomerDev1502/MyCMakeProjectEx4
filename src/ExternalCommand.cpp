#include "ExternalCommand.h"
#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
#include <cstring>
#include <cstdlib>   // getenv
#include <sstream>

ExternalCommand::ExternalCommand(const std::vector<std::string>& args) : args(args) {}

std::string ExternalCommand::findExecutable(const std::string& cmd) {
    if (cmd.find('/') != std::string::npos) {
        return cmd;  // נתיב מלא, לא מחפשים ב־PATH
    }

    const char* pathEnv = getenv("PATH");
    if (!pathEnv) {
        return "";  // PATH לא מוגדר
    }

    std::istringstream ss(pathEnv);
    std::string path;

    while (std::getline(ss, path, ':')) {
        std::string fullPath = path + "/" + cmd;
        std::cout << "Trying: " << fullPath << std::endl;
        if (access(fullPath.c_str(), X_OK) == 0) {
            return fullPath;
        }
    }

    return "";  // לא נמצא
}

void ExternalCommand::execute() {
    std::string execPath = findExecutable(args[0]);

    if (execPath.empty()) {
        std::cerr << "Command not found: " << args[0] << std::endl;
        return;
    }

    std::vector<char*> argv;
    argv.push_back(const_cast<char*>(execPath.c_str()));
    for (size_t i = 1; i < args.size(); ++i) {
        argv.push_back(const_cast<char*>(args[i].c_str()));
    }
    argv.push_back(nullptr);

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        return;
    }

    if (pid == 0) {
        execv(execPath.c_str(), argv.data());
        perror("exec failed");
        exit(1);
    } else {
        waitpid(pid, nullptr, 0);
    }
}
