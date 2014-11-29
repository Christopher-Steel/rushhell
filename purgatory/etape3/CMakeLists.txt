cmake_minimum_required(VERSION 2.8)
project(hell-chapter2)

set(BIN_NAME "test_dynamic_fsa")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror")

add_executable(${BIN_NAME} main.cpp Edge.cpp State.cpp FSA.cpp Matcher.cpp)