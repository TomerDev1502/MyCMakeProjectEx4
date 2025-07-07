#pragma once

#include "CommandParser.h"

class Shell {
public:
    void run();

private:
    CommandParser parser;
};
