#include "ExitCommand.h"
#include "CommandFactory.h"
#include <cstdlib>

namespace {
    // רישום ה-exit לסינגלטון
    bool regExit = [](){
        CommandFactory<Command>::instance()
            .registerType("exit", [](const auto&){ 
                return std::make_unique<ExitCommand>(std::vector<std::string>{}); 
            });
        return true;
    }();
}

void ExitCommand::execute() {
    std::exit(0);
}
