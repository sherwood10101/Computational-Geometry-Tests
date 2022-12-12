// Computational geometry template application.
#include <minimum_volume_box_calculator.h>
#include <mesh_loader.h>
#include <mesh_visualizer.h>
#include <poisson_recon.h>

#include "polyscope/polyscope.h"

#include <assert.h>
#include <stdio.h>

#include <experimental/filesystem>
#include <fstream>
#include <iostream>


namespace fs = std::experimental::filesystem;

void writePointsFile(const std::string& points_file, computational_geometry::Mesh& mesh)
{
  std::ofstream ofs(points_file);
  assert(ofs.is_open());

  std::cout << "Writing out points file: " << points_file << "..." << std::endl;
  computational_geometry::CGMesh& cg_mesh = mesh.getMesh();
  for (auto v_it = cg_mesh.vertices_begin(); v_it != cg_mesh.vertices_end(); ++v_it) {
    computational_geometry::CGMesh::Point point = cg_mesh.point(*v_it);
    computational_geometry::CGMesh::Normal normal = cg_mesh.normal(*v_it);
    //computational_geometry::CGMesh::Normal normal;
    //normal[0] = normal[1] = normal[2] = 0.0;
    //computational_geometry::CGMesh::Scalar n_faces = 0;
    //for (auto vf_it = cg_mesh.vf_iter(*v_it); vf_it.is_valid(); ++vf_it) {
    //    normal += cg_mesh.normal(*vf_it);
    //    std::cout << "normal = " << normal[0] << " " << normal[1] << " " << normal[2]  << std::endl;
    //    ++n_faces;
    //}

    //normal /= n_faces;
    if (normal.length() > 1e-06) {
      normal.normalize();
    }
    ofs << point[0] << " " << point[1] << " " << point[2] << " " << normal[0] << " " << normal[1] << " " << normal[2] << std::endl;
  }
}

int main (const int argc, char **const argv) 
{
  std::cout << "MODS 2.  Press enter to continue..." << std::endl;
  std::cin.get();

  if (argc != 3) {
    std::cout << "Incorrect usage, must be: computational_geometry_template <input_mesh_file> <poisson_reconstructed_ply_file>" << std::endl;
    return -1;
  }

  const auto mesh_file = argv[1];
  assert(fs::exists(mesh_file));

  std::cout << "Processing input mesh file : " << mesh_file << std::endl;

  // 1. Load mesh using OpenMesh library.
  computational_geometry::MeshLoader mesh_loader(mesh_file);

  // 2. Do mesh statistics report.
  mesh_loader.getMesh().reportStats();

  // 5. Generate temporary point cloud file and reconstruct mesh using PoissonRecon library.
  const auto output_ply_file = argv[2];
  if (fs::exists(output_ply_file)) {
    remove(output_ply_file);
  }

  std::string points_file("points.txt");
  writePointsFile(points_file, mesh_loader.getMesh());
  int depth = 5;

  computational_geometry::PoissonParams paramsTest;
  paramsTest.programName = "computational_geometry_template_main.cc";
  paramsTest.inputFilename = points_file;
  paramsTest.outputFilename = argv[2];
  paramsTest.depth = 6;
  
  // test creating synthetic argc and argv
  //std::vector<std::string> arguments = {"--dir", "/some_path"}; // worked

  std::string tmp = std::to_string(paramsTest.depth);
	char const *depth_char = tmp.c_str();
  std::vector<std::string> arguments = { "PoissonRecon", "--in", const_cast<char*>(paramsTest.inputFilename.c_str()),
	                 "--out", const_cast<char*>( paramsTest.outputFilename.c_str()),
	                 "--depth", const_cast<char*>(depth_char) };
  int argctest = 0;
  //char** argvtest;
  //computational_geometry::testArgvCreator(argctest, argvtest, arguments);

  std::vector<char*> argvtest;
  for (const auto& arg : arguments)
      argvtest.push_back((char*)arg.data());
  argvtest.push_back(nullptr);
  argctest = argvtest.size() - 1;
  //f.bar(argv.size() - 1, argv.data());

  //char* argvSynthetic[(argvtest.size() - 1)] = argvtest.data();
  char** argv2 = argvtest.data();

  std::cout << "number of arguments: " << argctest << std::endl;
  std::cout << "printing synthetic argvtest: " << std::endl;
  for (int i = 0; i < argctest-1; i++)
    std::cout << argvtest[i] << std::endl;
  std::cout << "press enter to continue" << std::endl;
  std::cin.get();

    std::cout << "printing synthetic argv2: " << std::endl;
  for (int i = 0; i < argctest-1; i++)
    std::cout << argv2[i] << std::endl;
  std::cout << "press enter to continue" << std::endl;
  std::cin.get();

  //computational_geometry::PoissonMeshReconstructor mesh_reconstructor(paramsTest);
  computational_geometry::PoissonMeshReconstructor mesh_reconstructor(paramsTest, points_file, output_ply_file, depth, argctest, argv2);

  mesh_reconstructor.Run();
  if (fs::exists(points_file)) {
    remove(points_file.c_str());
  }

  // 4. Display the mesh using polyscope.
  // Initialize polyscope.
  polyscope::init();
  // Visualize mesh
  computational_geometry::MeshVisualizer mesh_visualizer(mesh_loader.getMesh());
  mesh_visualizer.showMesh();

  // 5. To demonstrate integration of Geometric Tools, compute minimum volume 3D bounding box of the mesh and reports it's volume.
  computational_geometry::MinimumVolumeBoxCalculator box_calculator(mesh_loader.getMesh());
  std::cout << "Minimum 3D bbox volume: " << box_calculator.getVolume() << std::endl;

  std::cout << "Done." << std::endl;

  return 0;
}
