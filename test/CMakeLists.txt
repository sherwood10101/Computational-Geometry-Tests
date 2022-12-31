# Define the minimum CMake version (required)
cmake_minimum_required(VERSION 3.10)

# Define the project name
project(tool_path_test)

# Default to C++17
set(CMAKE_CXX_STANDARD 17)

# ToolPath test executable
include_directories(${Boost_INCLUDE_DIRS})
add_executable(tool_path_test tool_path_test_main.cc)
target_link_libraries (tool_path_test ToolPath ${Boost_LIBRARIES})

enable_testing()
add_test(tool_path_test tool_path_test)
# make "make test" run the test program
add_custom_target(test
     DEPENDS tool_path_test
     COMMAND tool_path_test
)