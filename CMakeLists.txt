cmake_minimum_required(VERSION 3.10)

project(MyCMakeProject_Headers)

include_directories(include)

add_executable(MyCMakeProject_Headers 
    src/main.cpp
    src/MyFunctions.cpp
)
