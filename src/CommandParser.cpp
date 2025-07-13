#include "CommandParser.h"
#include "ExternalCommand.h"
#include "JobsCommand.h"
#include "RedirectionCommand.h"
#include "PipeCommand.h"
#include "CommandFactory.h"

#include <sstream>
#include <algorithm>

CommandParser::CommandParser(std::vector<Job>& jobs)
  : jobs_(jobs)
{}

std::unique_ptr<Command> CommandParser::parse(const std::string& line) {
    // trim
    std::string s = line;
    s.erase(0, s.find_first_not_of(" \t\r\n"));
    s.erase(s.find_last_not_of(" \t\r\n") + 1);
    if (s.empty()) return nullptr;

    // background?
    bool background = false;
    if (s.back() == '&') {
        background = true;
        s.pop_back();
        if (!s.empty() && s.back() == ' ')
            s.pop_back();
    }

    // tokenize
    std::istringstream iss(s);
    std::vector<std::string> tokens;
    std::string tok;
    while (iss >> tok) tokens.push_back(tok);
    if (tokens.empty()) return nullptr;

    // extract I/O redirection
    std::string inFile, outFile;
    bool append = false;
    for (auto it = tokens.begin(); it != tokens.end(); ++it) {
        if (*it == "<" && std::next(it) != tokens.end()) {
            inFile = *std::next(it);
            tokens.erase(it, std::next(it,2));
            break;
        }
    }
    for (auto it = tokens.begin(); it != tokens.end(); ++it) {
        if ((*it == ">" || *it == ">>") && std::next(it) != tokens.end()) {
            append = (*it == ">>");
            outFile = *std::next(it);
            tokens.erase(it, std::next(it,2));
            break;
        }
    }

    // pipeline?
    if (std::find(tokens.begin(), tokens.end(), "|") != tokens.end()) {
        std::vector<std::vector<std::string>> segments;
        auto start = tokens.begin();
        for (auto it = tokens.begin(); it != tokens.end(); ++it) {
            if (*it == "|") {
                segments.emplace_back(start, it);
                start = it + 1;
            }
        }
        if (start != tokens.end())
            segments.emplace_back(start, tokens.end());

        return std::make_unique<PipeCommand>(
            segments,
            inFile,       // <-- now passes inputFile
            outFile,
            append,
            background,
            jobs_);
    }

    // myjobs
    if (tokens[0] == "myjobs")
        return std::make_unique<JobsCommand>(jobs_);

    // redirection (no pipe)
    if (!inFile.empty() || !outFile.empty()) {
        return std::make_unique<RedirectionCommand>(
            tokens,
            inFile,
            outFile,
            append,
            background,
            jobs_);
    }

    // built‐in via factory
    if (auto cmd = CommandFactory<Command>::instance().create(tokens))
        return cmd;

    // external
    return std::make_unique<ExternalCommand>(
        tokens,
        background,
        jobs_);
}
