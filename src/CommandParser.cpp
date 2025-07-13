#include "CommandParser.h"
#include "CommandFactory.h"
#include "ExternalCommand.h"
#include <sstream>

std::unique_ptr<Command> CommandParser::parse(const std::string& line) {
    std::istringstream iss(line);
    std::vector<std::string> tokens;
    std::string tok;
    while (iss >> tok) tokens.push_back(tok);

    // קודם נסו יצירת פקודה פנימית
    if (auto cmd = CommandFactory<Command>::instance().create(tokens))
        return cmd;

    // אחרת ExternalCommand
    return std::make_unique<ExternalCommand>(tokens);
}
