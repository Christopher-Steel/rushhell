cmake_minimum_required(VERSION 2.8)
project(hell-step1)

set(BIN_NAME "test_fsm")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror")

add_executable(${BIN_NAME} main.cpp Machine.cpp)