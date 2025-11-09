cmake_minimum_required(VERSION 3.19)

project(turtlepreter
    VERSION     0.0.1
    DESCRIPTION "Interpreter of commands for a turtle"
    LANGUAGES   C CXX
)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(lib)

add_subdirectory(turtlepreter)
