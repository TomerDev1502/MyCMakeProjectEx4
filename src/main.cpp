#include "Shell.h"
#include "CommandParser.h"
#include "CdCommand.h"              // כדי שהTU ירוץ
#include "CommandFactory.h"
#include <iostream>

int main() {
    // אופציונלי: הדפסה של כל הפקודות שנרשמו
    auto cmds = CommandFactory<Command>::instance().list();
    std::cout<<"Registered commands:";
    for (auto& name:cmds) std::cout<<' '<<name;
    std::cout<<"\n\n";

    Shell shell;
    shell.run();
    return 0;
}
