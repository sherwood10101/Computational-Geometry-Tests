#include <tool_path.h>

#include <assert.h>

namespace computational_geometry {

void ToolPathPoint::setLocationIndex(int location_index) {
  assert(location_index >= 0);
  assert(location_index >= m_location_index);
  m_location_index = location_index;
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

ToolPath::ToolPath(int n_points) : m_path(n_points), m_point_indices_map(n_points, nullptr) {
  int point_counter = 0;
  for (auto& path_point : m_path){
    m_point_indices_map[point_counter] = &path_point;
    point_counter++;
  }
}

void ToolPath::updatePointIndices() {
  if (!m_point_indices_valid) {
    m_point_indices_map = std::vector<ToolPathPoint*>(m_path.size(), nullptr);
    int point_counter = 0;
    for (auto& path_point : m_path){
      m_point_indices_map[point_counter] = &path_point;
      point_counter++;
    }

    m_point_indices_valid = true;
    m_current_position_set = false;
  }
}

void ToolPath::getFirst() {
  m_current_position = m_path.begin();
  m_current_position_set = true;
}

bool ToolPath::getNext() {
  assert(m_current_position_set);
  if (m_current_position == m_path.end()) {
    return false;
  }

  m_current_position++;
  return (m_current_position != m_path.end());
}

void ToolPath::setLocation(int point_index, const Vector3D& location) {
  if (!m_point_indices_valid) {
    updatePointIndices();
  }

  if (m_locations.empty()) {
    // This is initial point of trajectory, it must always be at index = 0
    assert(point_index == 0);
  } else {
    int n_points = m_path.size();
    auto* path_point = m_point_indices_map[point_index];
    if (point_index > 0) {
      // Annotate location indices of the previous path points.
      int location_index_init = path_point->getLocationIndex();
      if (location_index_init < 0) {
         int annotated_point_index = point_index;
         for(int i = point_index; i >= 0; i--) {
          const auto* path_point_cur = m_point_indices_map[i];
          int location_index_cur = path_point_cur->getLocationIndex();
          if (location_index_cur >= 0) {
            annotated_point_index = i;
            break;
          }
         }

         assert(annotated_point_index != point_index);
         int prev_location_index = m_point_indices_map[annotated_point_index]->getLocationIndex();
         assert(prev_location_index >= 0);
         for(int i = annotated_point_index + 1; i < point_index; i++) {
          m_point_indices_map[i]->setLocationIndex(prev_location_index);
         }
      }
    }

    if (point_index < n_points - 1) {
      int location_index_init = path_point->getLocationIndex();
      if (location_index_init >= 0) {
        // Annotate location indices of the next path points.
        int location_index_new = m_locations.size();
        for(int i = point_index + 1; i < n_points; i++) {
          auto* path_point_cur = m_point_indices_map[i];
          int location_index_cur = path_point_cur->getLocationIndex();
          if (location_index_cur != location_index_init) {
            break;
          }

          path_point_cur->setLocationIndex(location_index_new);
        }
      }
    }
  }

  // Set location index for the point.
  m_point_indices_map[point_index]->setLocationIndex(m_locations.size());

  // Insert location into the locations container.
  m_locations.push_back(location);
}

void ToolPath::finalizeInitialization() {
  if (!m_point_indices_valid) {
    updatePointIndices();
  }

  int n_points = m_path.size();

  // Get the last existing location.
  auto* last_point = m_point_indices_map[n_points - 1];
  if (last_point->getLocationIndex() < 0) {
    // 1. Find last point index with annotated location.
    int last_annotated_point_index = -1;
    for(int i = n_points - 2; i >=0; i--) {
      auto* path_point_cur = m_point_indices_map[i];
      if(path_point_cur->getLocationIndex() >= 0) {
        last_annotated_point_index = i;
        break;
      }
    }

    assert(last_annotated_point_index >=0);
    assert(last_annotated_point_index < n_points);

    int location_index = m_point_indices_map[last_annotated_point_index]->getLocationIndex();
    for(int i = last_annotated_point_index + 1; i < n_points; i++) {
      m_point_indices_map[i]->setLocationIndex(location_index);
    }
  }

  // Resize m_data to actual capacity to optimize memory.
  m_data.shrink_to_fit();
  m_locations.shrink_to_fit();
}

void ToolPath::setComment(int point_index, const std::string& comment_str) {
  if (!m_point_indices_valid) {
    updatePointIndices();
  }

  // Check if comments exists in m_comments.
  auto iter = m_comments.find(comment_str);
  if (iter == m_comments.end()) {
    m_comments.insert(comment_str);
  }
  iter = m_comments.find(comment_str);
  assert(iter != m_comments.end());
  int index = std::distance(m_comments.begin(), iter);

  m_point_indices_map[point_index]->setCommentIndex(index);
}

void ToolPath::setData(int point_index, const Data3D& values) {
  if (!m_point_indices_valid) {
    updatePointIndices();
  }

  int index = m_data.size();
  m_data.push_back(values);
  m_point_indices_map[point_index]->setDataIndex(index);
}

ToolPathPointInfo ToolPath::getToolPathPointInfo(int point_index) {
  if (!m_point_indices_valid) {
    updatePointIndices();
  }

  ToolPathPointInfo result;

  const auto* path_point = m_point_indices_map[point_index];

  // Get location.
  const auto location_index = path_point->getLocationIndex();
  assert(location_index >= 0);
  assert(location_index < static_cast<int>(m_locations.size()));
  result.location = m_locations[location_index];

  if (path_point->m_data) {
    // Get comment string.
    int comment_index = path_point->m_data->getCommentIndex();
    if (comment_index >= 0) {
      assert(comment_index < static_cast<int>(m_comments.size()));
      result.comment = *std::next(m_comments.begin(), comment_index);
    }

    // Get data.
    int data_index = path_point->m_data->getDataIndex();
    if (data_index >= 0) {
      assert(data_index < static_cast<int>(m_data.size()));
      result.data = m_data[data_index];
    }
  }

  return result;
}

void ToolPath::cleanUpMetaData() {
  if (!m_point_indices_valid) {
    updatePointIndices();
  }

  std::vector<Data3D>().swap(m_data);
  m_comments.clear();

  int n_points = m_point_indices_map.size();
  for(int i = 0; i < n_points; i++) {
    m_point_indices_map[i]->m_data =  std::nullopt;
  }
}

void ToolPath::InsertPathPointAtCurrentPosition(const Vector3D& location,
                                                std::optional<std::string> comment,
                                                std::optional<Data3D> data_3d) {
  // Insert new path point.
  assert(m_current_position_set);
  m_current_position = m_path.insert(m_current_position, ToolPathPoint());
  auto* path_point = &(*m_current_position);
  m_point_indices_valid = false;

  // Insert new location.
  int location_index_new = m_locations.size();
  m_locations.push_back(location);
  path_point->setLocationIndex(location_index_new);

  if (m_current_position != m_path.begin()) {
    // Update location index for next path points.
    const auto& path_point_prev = *(std::prev(m_current_position));
    int location_index_prev = path_point_prev.getLocationIndex();
    assert(location_index_prev >= 0);
    for (auto iter = std::next(m_current_position); iter != m_path.end(); iter++) {
      auto& path_point_cur = *iter;
      if (path_point_cur.getLocationIndex() != location_index_prev) {
        break;
      }

      path_point_cur.setLocationIndex(location_index_new);
    }
  }

  if (comment) {
    // Set comment.
    // Check if comments exists in m_comments.
    auto iter = m_comments.find(*comment);
    if (iter == m_comments.end()) {
      m_comments.insert(*comment);
    }
    iter = m_comments.find(*comment);
    assert(iter != m_comments.end());
    int index = std::distance(m_comments.begin(), iter);

    path_point->setCommentIndex(index);
  }

  if (data_3d) {
    int index = m_data.size();
    m_data.push_back(*data_3d);
    path_point->setDataIndex(index);
  }
}

void ToolPath::append(const ToolPath& other_path) {
  // TODO : implement this function.
}
  
} // namespace computational_geometry
