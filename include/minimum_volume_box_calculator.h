#pragma once

#include <mesh_loader.h>

#include <Mathematics/MinimumVolumeBox3.h>

namespace computational_geometry {

/// Top-level class to calculate minimum volume 3D box for the mesh.
class MinimumVolumeBoxCalculator {
  public:
    MinimumVolumeBoxCalculator(const Mesh& mesh);

    /// @return volume of 3D box.
    float getVolume() const { return m_volume; }
  
  private:
     float m_volume;
};
  
} // namespace computational_geometry
