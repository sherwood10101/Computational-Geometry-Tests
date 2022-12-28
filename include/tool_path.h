#pragma once

#include <array>
#include <list>
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
    int getCommentIndex() const { return m_comment_index; }

    /// @brief data index in ToolPath::m_data.
    void setDataIndex(int data_index) { m_data_index = data_index; }
    int getDataIndex() const { return m_data_index; }
  
  private:
    /// @brief index in ToolPath::m_comments, -1 if not set.
    int m_comment_index{-1};

    /// @brief index in ToolPath::m_data, -1 if not set.
    int m_data_index{-1};
};

/// Tool path point class.
class ToolPathPoint {
  public:
    ToolPathPoint() {}

    /// @brief set location index in ToolPath::m_locations.
    void setLocationIndex(int location_index);

    /// @brief get location index in ToolPath::m_locations.
    int getLocationIndex() const { return m_location_index; }

    /// @brief set comment index in ToolPath::m_comments.
    void setCommentIndex(int comment_index);

    /// @brief set data index in ToolPath::m_data.
    void setDataIndex(int data_index);
  
  private:
    // Index of location (in ToolPath::m_locations) this point has inherited location from.
    int m_location_index{-1};

    // Data associated with this path point.
    std::optional<ToolPathPointMetaData> m_data{std::nullopt};

    friend class ToolPath;
};

/// Structure for path point data, used for queries.
struct ToolPathPointInfo {
  /// @brief Actual location of path point.
  Vector3D location;

  /// @brief Comment string (if assigned).
  std::optional<std::string> comment{std::nullopt};

  /// @brief Data.
  std::optional<Data3D> data{std::nullopt};
};

/// Top-level class for ToolPath object.
class ToolPath {
  public:
    ToolPath(int n_points);
    ToolPath(const ToolPath& other_tool_path);
    /// @brief Constructor to combine vector of paths in a single path assuming the order in vector.
    /// @param other_tool_paths tool paths to combine
    /// @note Input vector of paths is being cleaned up.
    ToolPath(std::vector<ToolPath>& other_tool_paths);

    /// @returns number of path points.
    int numPoints() const {return m_path.size();}

    /// @brief Update location for the given path point.
    void setLocation(int point_index, const Vector3D& location);

    /// @brief Finalize initialization (update location for last chunk of path points after last change + optimize m_data memory).
    void finalizeInitialization();

    /// @brief Update comment for the given path point.
    void setComment(int point_index, const std::string& comment_str);

    /// @brief Update data for the given path point.
    void setData(int point_index, const Data3D& values);

    /// @brief Get path point info.
    /// @param point_index index in m_points.
    /// @returns path point data.
    ToolPathPointInfo getToolPathPointInfo(int point_index);

    /// @brief Utility to cleanup metadata for all path points.
    void cleanUpMetaData();

    /// @brief Insertion of new path point at current path position.
    void InsertPathPointAtCurrentPosition(const Vector3D& location,
                                          std::optional<std::string> comment = std::nullopt,
                                          std::optional<Data3D> data_3d = std::nullopt); 

    /// @brief Utility to synchronize m_path and m_point_indices_map after
    /// path modifications (insertion/deletion of path points).
    void updatePointIndices();

    /// @brief utility to set m_current_position to the beginning of m_path.
    void getFirst();

    /// @brief utility to increment m_current_position to the next position in m_path.
    /// @returns false if current position is at the end of the list already.
    bool getNext();

    /// @brief utility to append other_path to the end of the given one.
    /// @note: This procedure invalidates content of the other_path.
    void append(ToolPath& other_path);

    /// @brief utility to clear all the data in ToolPath.
    void clear();

    /// @brief insertion of other tool path at given index.
    /// @note: This procedure invalidates content of the other_path.
    void insert(int point_index, ToolPath& other_path);
  
  private:
    /// @brief Actual path - sequence of points.
    std::list<ToolPathPoint> m_path;

    /// @brief current position in the path.
    std::list<ToolPathPoint>::iterator m_current_position;

    /// @brief flag indicating that m_current_position is set to something.
    bool m_current_position_set{false};

    /// @brief vector to get ToolPathPoint by index.
    std::vector<ToolPathPoint*> m_point_indices_map;

    /// @brief flag indicating that m_points_indices_map container invalidated because of insertion/deletion of new path points.
    bool m_point_indices_valid{true};

    /// @brief Locations vector.
    std::vector<Vector3D> m_locations;

    /// @brief comments set.
    std::set<std::string> m_comments;

    /// @brief data vector.
    std::vector<Data3D> m_data;
};
  
} // namespace computational_geometry
