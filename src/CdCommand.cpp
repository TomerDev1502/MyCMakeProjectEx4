#include "CdCommand.h"
#include "CommandFactory.h"

#include <unistd.h>    // chdir, getcwd
#include <iostream>    // std::cerr
#include <cstdlib>     // getenv

CdCommand::CdCommand(const std::vector<std::string>& args)
  : args(args)
{}

void CdCommand::execute() {
    const char* path = (args.size() > 1 ? args[1].c_str() : getenv("HOME"));
    if (!path) {
        std::cerr << "cd: HOME not set\n";
        return;
    }
    if (chdir(path) != 0) {
        perror("cd failed");
    }
}

// --- כאן קבור ה-static registration ---
namespace {
    bool registeredCd = [](){
        CommandFactory<Command>::instance()
            .registerType("cd", [](const std::vector<std::string>& tokens){
                return std::make_unique<CdCommand>(tokens);
            });
        return true;
    }();
}
