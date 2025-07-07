#include "Shell.h"
#include <iostream>

void Shell::run() {
    std::string line;

    while (true) {
        std::cout << "$ ";
        if (!std::getline(std::cin, line)) break;

        if (line == "exit") break;

        auto cmd = parser.parse(line);
        if (cmd) {
            cmd->execute();
        }
    }
}
