# Define the minimum CMake version (required)
cmake_minimum_required(VERSION 3.10)

# Define the project name
project(computational_geometry_template)
include(ExternalProject)

# Default to C++17
set(CMAKE_CXX_STANDARD 17)

include_directories(include)

# OpenMesh integration
find_package(Git REQUIRED)
ExternalProject_Add(OpenMesh
  GIT_REPOSITORY https://www.graphics.rwth-aachen.de:9000/OpenMesh/OpenMesh.git
  GIT_TAG OpenMesh-9.0
  PREFIX ${CMAKE_BINARY_DIR}/external/OpenMesh
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/external/OpenMesh/install
)

include_directories(${CMAKE_BINARY_DIR}/external/OpenMesh/src/OpenMesh/src)
link_directories(${CMAKE_BINARY_DIR}/external/OpenMesh/install/lib)
SET(CMAKE_EXE_LINKER_FLAGS 
          "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath -Wl,${CMAKE_BINARY_DIR}/external/OpenMesh/install/lib")

# Geometric Tools integration
ExternalProject_Add(GeometricTools
  GIT_REPOSITORY https://github.com/davideberly/GeometricTools.git
  GIT_TAG GTE-version-6.4
  PREFIX ${CMAKE_BINARY_DIR}/external/GeometricTools
  SOURCE_SUBDIR "GTE"
  INSTALL_COMMAND cmake -E echo "Skipping install step."
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/external/GeometricTools/install
)

include_directories(${CMAKE_BINARY_DIR}/external/GeometricTools/src/GeometricTools/GTE)
link_directories(${CMAKE_BINARY_DIR}/external/GeometricTools/src/GeometricTools/GTE/lib/Static)

file(
  GLOB_RECURSE APP_SRC_FILES
  src/computational_geometry_template_main.cc
  src/mesh_loader.cc
  src/minimum_volume_box_calculator.cc)

# Add computational_geometry_template exec.
add_executable(computational_geometry_template ${APP_SRC_FILES})
# Add dependencies
target_link_libraries(computational_geometry_template stdc++fs pthread OpenMeshCore OpenMeshTools gtapplications gtgraphics gtmathematicsgpu)
add_dependencies(computational_geometry_template OpenMesh GeometricTools)

# Install targets defined above
install(TARGETS computational_geometry_template DESTINATION bin)
