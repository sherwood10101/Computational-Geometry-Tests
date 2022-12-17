#include <mesh_visualizer.h>

#include "polyscope/polyscope.h"
#include "polyscope/point_cloud.h"

namespace computational_geometry {

void MeshVisualizer::showMesh() const {
  // Get point cloud from the mesh.
  assert(m_mesh);
  std::vector<std::array<double, 3>> points;
  const auto& cg_mesh = m_mesh->getMesh();
  for (auto v_it = cg_mesh.vertices_begin(); v_it != cg_mesh.vertices_end(); ++v_it) {
    auto vertex_cur = cg_mesh.point(*v_it);
    std::array<double, 3> point_cur;
    point_cur[0] = vertex_cur[0];
    point_cur[1] = vertex_cur[1];
    point_cur[2] = vertex_cur[2];
    points.push_back(point_cur);
  }

  // Register a point cloud.
  polyscope::registerPointCloud("mesh points", points);

  // View the point cloud we just registered in the 3D UI.
  polyscope::show();
}
  
} // namespace computational_geometry
