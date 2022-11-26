#pragma once

#include <array>
#include <map>
#include <optional>
#include <set>
#include <vector>

namespace computational_geometry {

typedef std::array<float, 3> Vector3D;
typedef std::vector<std::vector<std::vector<float>>> Data3D;

/// Tool path point metadata class.
class ToolPathPointMetaData {
  public:
    ToolPathPointMetaData() {}

    /// @brief comment index in ToolPath::m_comments.
    void setCommentIndex(int comment_index) { m_comment_index = comment_index; }
  
  private:
    /// @brief index in ToolPath::m_comments, -1 if not set.
    int m_comment_index{-1};

    /// @brief index in ToolPath::m_data
    int m_data_index{-1};
};

/// Tool path point class.
class ToolPathPoint {
  public:
    ToolPathPoint() {}

    /// @brief set index of point this point has inherited location from.
    void setLocationIndex(int point_index);

    /// @brief get point index for location inherited from.
    int getLocationIndex() const { return m_point_location_index; }

    /// @brief set comment index in ToolPath::m_comments.
    void setCommentIndex(int comment_index);
  
  private:
    // Index of point (in ToolPath::m_points) this point has inherited location from (maybe the same point).
    int m_point_location_index{0};

    // Data associated with this path point.
    std::optional<ToolPathPointMetaData> m_data{std::nullopt};
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

    /// @brief Update comment for the given path point.
    void setComment(int point_index, const std::string& comment_str);
  
  private:
    /// @brief Actual path - sequence of points.
    std::vector<ToolPathPoint> m_points;

    /// @brief Locations map, key is index in m_points;
    std::map<int, Vector3D> m_locations;

    /// @brief comments set.
    std::set<std::string> m_comments;

    /// @brief data vector.
    std::vector<Data3D> m_data;
};
  
} // namespace computational_geometry
