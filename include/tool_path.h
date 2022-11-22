#pragma once

#include <optional>
#include <vector>

namespace computational_geometry {

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
    // Actual path - sequence of points.
    std::optional<ToolPathPointData> m_data{std::nullopt};
};

/// Top-level class for ToolPath object.
class ToolPath {
  public:
    ToolPath(int n_points);
  
  private:
    // Actual path - sequence of points.
    std::vector<ToolPathPoint> m_points;
};
  
} // namespace computational_geometry
