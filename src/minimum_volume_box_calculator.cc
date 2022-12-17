#include <minimum_volume_box_calculator.h>

namespace computational_geometry {

MinimumVolumeBoxCalculator::MinimumVolumeBoxCalculator(const Mesh& mesh) 
{
  std::cout << "Calculating minimum volume 3D box..." << std::endl;

  uint32_t const numThreads = 4;
  size_t const lgMaxSample = 5;
  gte::MinimumVolumeBox3<float, false> mvb3(numThreads);
  gte::OrientedBox3<float> minBox;
 
  const auto& cg_mesh = mesh.getMesh();
  int n_vertices = cg_mesh.n_vertices();
  std::vector<gte::Vector3<float>> vertices(n_vertices);

  int i = 0;
  for (auto v_it = cg_mesh.vertices_begin(); v_it != cg_mesh.vertices_end(); ++v_it) {
    auto vertex_cur = cg_mesh.point(*v_it);
    vertices[i][0] = vertex_cur[0];
    vertices[i][1] = vertex_cur[1];
    vertices[i][2] = vertex_cur[2];
    i++;
  }

  mvb3(n_vertices, vertices.data(), lgMaxSample, minBox, m_volume);
}
  
} // namespace computational_geometry
