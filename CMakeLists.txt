cmake_minimum_required(VERSION 3.10)

project(MyShell)

include_directories(include)

add_executable(MyShell 
    src/main.cpp
    src/Shell.cpp
    src/CommandParser.cpp
    src/ExternalCommand.cpp
)
