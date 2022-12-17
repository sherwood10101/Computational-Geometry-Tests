#include <mesh_loader.h>

#include <OpenMesh/Core/IO/MeshIO.hh>


namespace computational_geometry {

Mesh::Mesh(const std::string& mesh_file) 
{
  std::cout << "Loading mesh file..." << std::endl;
  OpenMesh::IO::Options options;
  options += OpenMesh::IO::Options::Flag::FaceNormal;
  options += OpenMesh::IO::Options::Flag::VertexNormal;
  if (!OpenMesh::IO::read_mesh(m_mesh, mesh_file, options))  {
    throw std::runtime_error("Cannot load mesh file");
  }
  m_mesh.request_face_normals();
  m_mesh.request_vertex_normals();
}

void Mesh::reportStats() const 
{
  std::cout << "Mesh stats:" << std::endl;

  // Report number of vertices and faces.
  std::cout << "Number of vertices: " << m_mesh.n_vertices() << std::endl;
  std::cout << "Number of faces: " << m_mesh.n_faces() << std::endl;

  // Report total area of faces.
  double area = 0.;
  for (auto f_it = m_mesh.faces_begin(); f_it != m_mesh.faces_end(); ++f_it) {
    area += m_mesh.calc_face_area(*f_it);
  }
 std::cout << "Total area of faces: " << area << std::endl;

}
  
} // namespace computational_geometry
