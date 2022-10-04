# Define the minimum CMake version (required)
cmake_minimum_required(VERSION 3.10)

# Define the project name
project(computational_geometry_template)

# Default to C++17
set(CMAKE_CXX_STANDARD 17)

include_directories(include)

file(
  GLOB_RECURSE APP_SRC_FILES
  src/computational_geometry_template_main.cc)

# Add computational_geometry_template exec.
add_executable(computational_geometry_template ${APP_SRC_FILES})
# Add dependencies
target_link_libraries(computational_geometry_template stdc++fs)

# Install targets defined above
install(TARGETS computational_geometry_template DESTINATION bin)
