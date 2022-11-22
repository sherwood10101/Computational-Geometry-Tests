#pragma once

#include <array>
#include <map>
#include <optional>
#include <vector>

namespace computational_geometry {

typedef std::array<float, 3> Vector3D;

/// Tool path point data class.
class ToolPathPointData {
  public:
    ToolPathPointData() {}
    // TODO : define this class.
};

/// Tool path point class.
class ToolPathPoint {
  public:
    ToolPathPoint() {}
  
  private:
    // Actual 3D point
    Vector3D* location{nullptr};

    // Data associated with this path point.
    std::optional<ToolPathPointData> m_data{std::nullopt};
};

/// Top-level class for ToolPath object.
class ToolPath {
  public:
    ToolPath(int n_points);

    /// @returns number of path points.
    int numPoints() const {return m_points.size();}

    /// @brief Update location for the given path point.
    void setLocation(int point_index, const Vector3D& location);

    /// @brief Finalize locations initialization (update location for last chunk of path points after last change).
    void finalizeLocations();
  
  private:
    // Actual path - sequence of points.
    std::vector<ToolPathPoint> m_points;

    // Locations map, key is index in m_points;
    std::map<int, Vector3D> m_locations;
};
  
} // namespace computational_geometry
