#include "ExternalCommand.h"
#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
#include <cstring>
#include <cstdlib>   // getenv
#include <sstream>
#include <sys/stat.h>

ExternalCommand::ExternalCommand(const std::vector<std::string>& args,
                                 bool background,
                                 std::vector<Job>& jobs)
  : args_(args), background_(background), jobs_(jobs)
{}

std::string ExternalCommand::findExecutable(const std::string& cmd) {
    if (cmd.find('/') != std::string::npos) {
        // נתיב מלא
        struct stat st;
        if (stat(cmd.c_str(), &st) == 0 && (st.st_mode & S_IXUSR))
            return cmd;
        return "";
    }

    const char* pathEnv = getenv("PATH");
    if (!pathEnv) return "";

    std::istringstream ss(pathEnv);
    std::string path;
    while (std::getline(ss, path, ':')) {
        std::string fullPath = path + "/" + cmd;
        // ניטור ניסיון
        std::cout << "Trying: " << fullPath << std::endl;
        if (access(fullPath.c_str(), X_OK) == 0)
            return fullPath;
    }
    return "";
}

void ExternalCommand::execute() {
    auto execPath = findExecutable(args_[0]);
    if (execPath.empty()) {
        std::cerr << "Command not found: " << args_[0] << std::endl;
        return;
    }

    // בונים argv
    std::vector<char*> argv;
    argv.push_back(const_cast<char*>(execPath.c_str()));
    for (size_t i = 1; i < args_.size(); ++i) {
        argv.push_back(const_cast<char*>(args_[i].c_str()));
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
        std::exit(1);
    } else {
        if (background_) {
            // יצירת Job חדש
            int id = static_cast<int>(jobs_.size()) + 1;
            std::string cmdLine;
            for (size_t i = 0; i < args_.size(); ++i) {
                cmdLine += args_[i] + (i + 1 < args_.size() ? " " : "");
            }
            jobs_.push_back({ id, pid, cmdLine, JobStatus::Running });
            std::cout << "[" << id << "] " << pid << std::endl;
        } else {
            // פקודה רגילה – מחכה לסיום
            waitpid(pid, nullptr, 0);
        }
    }
}
