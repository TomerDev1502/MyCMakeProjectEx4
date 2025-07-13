#include "CommandParser.h"
#include "CommandFactory.h"
#include "ExternalCommand.h"
#include "JobsCommand.h"

#include <sstream>
#include <algorithm>

CommandParser::CommandParser(std::vector<Job>& jobs)
    : jobs_(jobs)
{}

std::unique_ptr<Command> CommandParser::parse(const std::string& line) {
    std::string str = line;
    str.erase(0, str.find_first_not_of(" \t\r\n"));
    str.erase(str.find_last_not_of(" \t\r\n") + 1);

    if (str.empty())
        return nullptr;

    bool background = false;
    if (str.back() == '&') {
        background = true;
        str.pop_back();
        if (!str.empty() && str.back() == ' ')
            str.pop_back();
    }

    std::istringstream iss(str);
    std::vector<std::string> tokens;
    std::string tok;
    while (iss >> tok)
        tokens.push_back(tok);

    if (tokens.empty())
        return nullptr;

    if (tokens[0] == "myjobs") {
        return std::make_unique<JobsCommand>(jobs_);
    }

    if (auto cmd = CommandFactory<Command>::instance().create(tokens))
        return cmd;

    return std::make_unique<ExternalCommand>(tokens, background, jobs_);
}
