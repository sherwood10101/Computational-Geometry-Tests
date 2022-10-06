// Computational geometry template application.
#include <mesh_loader.h>

#include <assert.h>

#include <experimental/filesystem>
#include <iostream>

namespace fs = std::experimental::filesystem;

int main (const int argc, char **const argv) 
{
  if (argc != 2) {
    std::cout << "Incorrect usage, must be: computational_geometry_template <input_mesh_file>" << std::endl;
    return -1;
  }

  const auto mesh_file = argv[1];
  assert(fs::exists(mesh_file));

  std::cout << "Processing input mesh file : " << mesh_file << std::endl;

  // 1. Load mesh.
  computational_geometry::MeshLoader mesh_loader(mesh_file);

  // 2. Do mesh statistics report.
  mesh_loader.getMesh().reportStats();

  // 3. TODO: Display the mesh using polyscope.

  // 4. TODO: To demonstrate integration of Geometric Tools, compute 3D bounding box of the mesh and reports it's volume.

  // 5. Perform 2X scaling of the mesh and then outputs a resulting mesh file.

  std::cout << "Done." << std::endl;

  return 0;
}
