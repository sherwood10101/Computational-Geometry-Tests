#include <tool_path.h>

#include <assert.h>

namespace computational_geometry {

void ToolPathPoint::setLocationIndex(int point_index) {
  assert(point_index >= 0);
  assert(point_index >= m_point_location_index);
  m_point_location_index = point_index;
}

void ToolPathPoint::setCommentIndex(int comment_index) {
  if (!m_data) {
    m_data = ToolPathPointMetaData();
  }

  m_data->setCommentIndex(comment_index);
}

void ToolPathPoint::setDataIndex(int data_index) {
  if (!m_data) {
    m_data = ToolPathPointMetaData();
  }

  m_data->setDataIndex(data_index);
}

ToolPath::ToolPath(int n_points) : m_points(n_points) {
}

void ToolPath::setLocation(int point_index, const Vector3D& location) {
  auto iter_range = m_locations.equal_range(point_index);
  if (m_locations.empty() || iter_range.second == m_locations.begin()) {
    // This is initial point of trajectory, it must always be at index = 0
    assert(point_index == 0);
  } else {
    if (iter_range.first == m_locations.end()) {
      // Get the last existing location.
      int last_location_index = m_locations.rbegin()->first;
      assert(last_location_index < point_index);

      // Annotate location indices for [last_location_index, point_index) interval of points
      for(int i = last_location_index + 1; i < point_index; i++) {
        m_points[i].setLocationIndex(last_location_index);
      }
    } else {
      auto iter_from = iter_range.first;
      int location_index_from = iter_from->first;
      auto iter_to = iter_range.second;
      int location_index_to = iter_to->first;
      assert(point_index >= location_index_from);
      assert(point_index <= location_index_to);

      // Annotate location indices for [point_index, location_index_to) interval of points
      for(int i = point_index + 1; i < location_index_to; i++) {
        m_points[i].setLocationIndex(point_index);
      }
    }
  }

  // Insert location into the map.
  m_locations[point_index] = location;

  // Set location index for the point.
  m_points[point_index].setLocationIndex(point_index);
}

void ToolPath::finalizeInitialization() {
  // Get the last existing location.
  int last_location_index = m_locations.rbegin()->first;
  int n_points = m_points.size();
  assert(last_location_index < n_points);
  assert(m_points[last_location_index].getLocationIndex() == last_location_index);

  // Annotate location indices for [last_location_index, point_index interval of points)
  for(int i = last_location_index + 1; i < n_points; i++) {
    m_points[i].setLocationIndex(last_location_index);
  }

  // Resize m_data to actual capacity to optimize memory.
  m_data.shrink_to_fit();
}

void ToolPath::setComment(int point_index, const std::string& comment_str) {
  // Check if comments exists in m_comments.
  auto iter = m_comments.find(comment_str);
  if (iter == m_comments.end()) {
    m_comments.insert(comment_str);
  }
  iter = m_comments.find(comment_str);
  assert(iter != m_comments.end());
  int index = std::distance(m_comments.begin(), iter);

  m_points[point_index].setCommentIndex(index);
}

void ToolPath::setData(int point_index, const Data3D& values) {
  int index = m_data.size();
  m_data.push_back(values);
  m_points[point_index].setDataIndex(index);
}
  
} // namespace computational_geometry
