#pragma once

#include <string>

namespace computational_geometry {

/// Top-level class to calculate minimum volume 3D box for the mesh.
class PoissonMeshReconstructor {
  public:
    PoissonMeshReconstructor(const std::string& input_points_file, 
                             const std::string& output_ply_file) : m_input_file(input_points_file),
                                                                   m_output_ply_file(output_ply_file) {}

    /// Top-level routine to run mesh reconstruction and write out PLY file.
    void Run();
  
  private:
     std::string m_input_file;
     std::string m_output_ply_file;
};
  
} // namespace computational_geometry
