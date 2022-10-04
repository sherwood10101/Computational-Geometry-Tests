// Computational geometry template application.
#include <assert.h>

#include <experimental/filesystem>
#include <iostream>

namespace fs = std::experimental::filesystem;

int main (const int argc, char **const argv) {
  if (argc != 2) {
    std::cout << "Incorrect usage, must be: computational_geometry_template <input_mesh_file>" << std::endl;
    return -1;
  }

  const auto mesh_file = argv[1];
  assert(fs::exists(mesh_file));

  std::cout << "Processing input mesh file : " << mesh_file << std::endl;

  // TODO : implement all required operations with mesh here.

  std::cout << "Done." << std::endl;

  return 0;
}
