cmake_minimum_required(VERSION 3.10)

project(MyShell)

add_executable(MyShell
    src/main.cpp
    src/Shell.cpp
    src/CommandParser.cpp
    src/ExternalCommand.cpp
    src/CdCommand.cpp
    src/ExitCommand.cpp
    src/JobsCommand.cpp
)

target_include_directories(MyShell PRIVATE include)
