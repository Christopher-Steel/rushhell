cmake_minimum_required(VERSION 2.8)
project(hell-step1)

set(BIN_NAME "test_functor_type")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror")

find_package(Boost 1.55.0 COMPONENTS system program_options REQUIRED)

add_executable(${BIN_NAME} main.cpp)

if (Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  target_link_libraries(${BIN_NAME} ${Boost_LIBRARIES})
endif()