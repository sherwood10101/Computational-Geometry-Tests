#pragma once

#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>

#include <string>

namespace computational_geometry {

struct CGTraits : public OpenMesh::DefaultTraits {};
typedef OpenMesh::TriMesh_ArrayKernelT<CGTraits>  CGMesh;

/// Mesh object.
class Mesh {
  public:
    Mesh(const std::string& mesh_file);

    /// Utility function to report basic mesh stats.
    void reportStats() const;

    /// CGMesh reference.
    const CGMesh& getMesh() const { return m_mesh; }

  private:
    /// @brief OpenMesh object.
    CGMesh m_mesh;
};

/// Top-level class to load the mesh.
class MeshLoader {
  public:
    /// Constructor - loads mesh file into memory using OpenMesh library.
    MeshLoader(const std::string& mesh_file) : m_mesh(mesh_file) {}

    /// @return constant reference to the loaded Mesh object.
    const Mesh& getMesh() const {return m_mesh;}

  private:
    /// Mesh object.
    Mesh m_mesh;
};
  
} // namespace computational_geometry
