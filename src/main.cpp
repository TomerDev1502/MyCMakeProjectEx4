#include "Shell.h"
#include "CommandParser.h"
#include "CdCommand.h"              // כדי שהTU ירוץ
#include "CommandFactory.h"
#include "ExitCommand.h"            // כדי שהTU ירוץ
#include <iostream>

int main() {
    auto cmds = CommandFactory<Command>::instance().list();
    std::cout<<"Registered commands:";
    for (auto& name:cmds) std::cout<<' '<<name;
    std::cout<<"\n\n";

    Shell shell;
    shell.run();
    return 0;
}
