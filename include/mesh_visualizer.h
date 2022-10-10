#pragma once

#include <mesh_loader.h>

namespace computational_geometry {

/// Top-level class to visualize 3D mesh with polyscope.
class MeshVisualizer {
  public:
    MeshVisualizer(const Mesh& mesh) : m_mesh(&mesh) {}

    /// @brief Main routine to display the mesh.
    void showMesh() const;
  
  private:
     const Mesh* m_mesh;
};
  
} // namespace computational_geometry
