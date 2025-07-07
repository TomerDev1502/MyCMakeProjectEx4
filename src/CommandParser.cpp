#include "CommandParser.h"
#include "ExternalCommand.h"
#include <sstream>
#include <vector>

std::unique_ptr<Command> CommandParser::parse(const std::string& line) {
    std::istringstream iss(line);
    std::vector<std::string> tokens;
    std::string token;
    while (iss >> token) {
        tokens.push_back(token);
    }

    if (tokens.empty()) {
        return nullptr;
    }

    return std::make_unique<ExternalCommand>(tokens);
}
