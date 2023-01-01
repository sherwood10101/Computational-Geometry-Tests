// ToolPath unit tests.
#ifdef STAND_ALONE
#define BOOST_TEST_MODULE tool_path_tests
#endif

#include <boost/test/unit_test.hpp>

#include <tool_path.h>

#include <unistd.h>
#include <algorithm>
#include <chrono>
#include <ios>
#include <iostream>
#include <fstream>
#include <random>
#include <string>

// Basic procedure to populate test tool path data according to spec:
// - create a tool path data structure as described above as a linked list
// - add optional upgraded node storage for certain nodes to include x, y, or z or floating point metadata 
//   plus an optional string field plus an optional large 3D array (10 x 10 x 10 vector of vectors of vectors of float) 
//   that  can be attached occasionally to any point.
//   NOTE: original spec from James stated 3D vector must have 1000x1000x1000 size, but that brings us to ~ 4Gb per one node
//   and, as my laptop has only 16 Gb of memory, it makes it impossible to annotate 0.1% of nodes by 3D vector data.
//   So, I had to reduce it to 10x10x10 3D vector.
// - populate Toolpath instance with 100 million points (nodes), starting with only x,y,z data 
//   where each x or y or z coordinate changes randomly approximately every 20 points. 
// - Then randomly upgrade 1% of the points to hold a 100 character string.  
// - Then upgrade 0.1% of the points to also hold a 3D array of floats.

void fillToolPath(computational_geometry::ToolPath& tool_path, int step_coord_change_avg, int vector_data_size,
                    double nodes_percentage_with_string_data, double nodes_percentage_with_3d_vector) {
  // Perform random coordinate changes for approximately  every step_coord_change_avg points.

  std::default_random_engine step_generator;
  double sigma = std::clamp(step_coord_change_avg / 2., 5., 100.);
  std::normal_distribution<double> step_distribution(static_cast<double>(step_coord_change_avg), sigma);

  std::default_random_engine coord_index_generator;
  std::uniform_int_distribution<int> coord_index_distribution(0, 2);

  std::default_random_engine velocity_generator;
  std::uniform_real_distribution<double> velocity_distribution(-5.0, 10.0);

  int n_points = tool_path.numPoints();
  computational_geometry::Vector3D location_prev;
  int step = 1;
  //std::cout << "n_points = " << n_points << std::endl;
  for(int i = 0; i < n_points;) {
    if (i == 0) {
      // Set initial trajectory location.
      computational_geometry::Vector3D initial_location{0., 0., 0.};
      location_prev = initial_location;
      tool_path.setLocation(i, initial_location);
    } else {
      int coord_index = coord_index_distribution(coord_index_generator);
      double velocity = velocity_distribution(velocity_generator);
      auto current_location = location_prev;
      current_location[coord_index] += velocity * static_cast<double>(step);
      tool_path.setLocation(i, current_location);
      location_prev = current_location;
    }

    step = std::clamp(static_cast<int>(step_distribution(step_generator)), 1, n_points / 3);
    //std::cout << "i = " << i << ", step = " << step << std::endl;
    i += step;
  }

  if (nodes_percentage_with_string_data > std::numeric_limits<double>::epsilon()) {
    // Randomly upgrade nodes_percentage_with_string_data % of the points to hold a 100 character string.
    std::string comment_str(100, 'x');
    int step_comment_avg = std::clamp(static_cast<int>(100. / nodes_percentage_with_string_data), 1, n_points / 2);
    //std::cout << "step_comment_avg = " << step_comment_avg << std::endl;
    std::default_random_engine comment_step_generator;
    double comment_sigma = std::clamp(step_comment_avg / 2., 5., n_points / 10.);
    std::normal_distribution<double> step_comment_distribution(static_cast<double>(step_comment_avg), comment_sigma);
    for(int i = 0; i < n_points;) {
      tool_path.setComment(i, comment_str);
      step = std::clamp(static_cast<int>(step_comment_distribution(comment_step_generator)), 1, n_points / 3);
      //std::cout << "step = " << step << std::endl;
      i += step;
    }
  }

  if (nodes_percentage_with_3d_vector > std::numeric_limits<double>::epsilon()) {
    // Randomly upgrade nodes_percentage_with_3d_vector % of the points to also hold a 3D array of floats.
    std::vector<float> data_1d(vector_data_size, 1.); // All 1s.
    std::vector<std::vector<float>> data_2d(vector_data_size, data_1d);
    computational_geometry::Data3D data_3d(vector_data_size, data_2d);
    int step_data_avg = std::clamp(static_cast<int>(100. / nodes_percentage_with_3d_vector), 1, n_points / 2);
    //std::cout << "step_data_avg = " << step_data_avg << std::endl;
    std::default_random_engine data_step_generator;
    double data_sigma = std::clamp(step_data_avg / 2., 5., n_points / 10.);
    std::normal_distribution<double> step_data_distribution(static_cast<double>(step_data_avg), data_sigma);
    for(int i = 0; i < n_points;) {
      tool_path.setData(i, data_3d);
      step = std::clamp(static_cast<int>(step_data_distribution(data_step_generator)), 1, n_points / 3);
      //std::cout << "step = " << step << std::endl;
      i += step;
    }
  }

  // Finalize locations initialization (update location for last chunk of path points) + optimize Data3D memory.
  tool_path.finalizeInitialization();
}

//////////////////////////////////////////////////////////////////////////////
//
// process_mem_usage(double &, double &) - takes two doubles by reference,
// attempts to read the system-dependent data for a process' virtual memory
// size and resident set size, and return the results in KB.
//
// On failure, returns 0.0, 0.0

void process_mem_usage(double& vm_usage, double& resident_set)
{
   using std::ios_base;
   using std::ifstream;
   using std::string;

   vm_usage     = 0.0;
   resident_set = 0.0;

   // 'file' stat seems to give the most reliable results
   //
   ifstream stat_stream("/proc/self/stat", ios_base::in);

   // dummy vars for leading entries in stat that we don't care about
   //
   string pid, comm, state, ppid, pgrp, session, tty_nr;
   string tpgid, flags, minflt, cminflt, majflt, cmajflt;
   string utime, stime, cutime, cstime, priority, nice;
   string O, itrealvalue, starttime;

   // the two fields we want
   //
   unsigned long vsize;
   long rss;

   stat_stream >> pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr
               >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt
               >> utime >> stime >> cutime >> cstime >> priority >> nice
               >> O >> itrealvalue >> starttime >> vsize >> rss; // don't care about the rest

   stat_stream.close();

   long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
   vm_usage     = vsize / 1024.0;
   resident_set = rss * page_size_kb;
}

/// @brief Utility to report current memory usage.
void report_memory () {
  double vm, rss;
  process_mem_usage(vm, rss);
  std::cout << "Current memory allocated by program:" << std::endl;
  std::cout << "VM: " << vm << " kB; RSS: " << rss << " kB" << std::endl;
}

/// @brief Test for sequential access of all path points.
void testSequentialAccess(computational_geometry::ToolPath& tool_path, bool debug_output) {
  int n_points = tool_path.numPoints();
  std::cout << "Sequentially access all path points..." << std::endl;
  auto start = std::chrono::steady_clock::now();
  //std::cout << "n_points = " << n_points << std::endl;
  for(int i = 0; i < n_points; i++) {
    const auto path_point_data = tool_path.getToolPathPointInfo(i);
    if (debug_output) {
      std::cout << "Index: " << i << " , location = (" << path_point_data.location[0] << ", "
                << path_point_data.location[1] << ", " << path_point_data.location[2] << ")" << std::endl;
      if (path_point_data.comment) {
        std::cout << "  Comment: " << *path_point_data.comment << std::endl;
      }
      if (path_point_data.data) {
        std::cout << "  3D data is there, first element: " << (*path_point_data.data)[0][0][0] << std::endl;
      }
    }
  }
  auto end = std::chrono::steady_clock::now();
  std::chrono::duration<double> elapsed_seconds = end - start;
  std::cout << "Sequential access of all points finished, elapsed_time = " << elapsed_seconds.count() << " sec" << std::endl;
  report_memory();
}

/// @brief Test for performance for random access of percentage_of_points_to_access% of the data.
void testRandomAccess(computational_geometry::ToolPath& tool_path, double percentage_of_points_to_access, bool debug_output) {
  int n_points = tool_path.numPoints();
  int n_points_to_query = std::clamp(static_cast<int>((percentage_of_points_to_access / 100.) * n_points), 1, n_points);
  //std::cout << "n_points_to_query = " << n_points_to_query << std::endl;
  std::cout << "Performing random access of " << percentage_of_points_to_access << "% of the path points"<< std::endl;
  auto start = std::chrono::steady_clock::now();
  std::default_random_engine point_index_generator;
  std::uniform_int_distribution<int> point_index_distribution(0, n_points - 1);
  for(int i = 0; i < n_points_to_query; i++) {
    int point_index = point_index_distribution(point_index_generator);
    //std::cout << "point index = " << point_index << std::endl;
    const auto path_point_data = tool_path.getToolPathPointInfo(point_index);
    if (debug_output) {
      std::cout << "Index: " << i << " , location = (" << path_point_data.location[0] << ", "
                << path_point_data.location[1] << ", " << path_point_data.location[2] << ")" << std::endl;
      if (path_point_data.comment) {
        std::cout << "  Comment: " << *path_point_data.comment << std::endl;
      }
      if (path_point_data.data) {
        std::cout << "  3D data is there, first element: " << (*path_point_data.data)[0][0][0] << std::endl;
      }
    }
  }
  auto end = std::chrono::steady_clock::now();
  std::chrono::duration<double> elapsed_seconds = end - start;
  std::cout << "Random access of path points finished, elapsed_time = " << elapsed_seconds.count() << " sec" << std::endl;
  report_memory();
}

/// @brief Test of performance for downgrading points to the minimum (no metadata).
void testMetadataCleanup(computational_geometry::ToolPath& tool_path) {
  std::cout << "Performing cleanup of metadata for all path points..." << std::endl;
  auto start = std::chrono::steady_clock::now();
  tool_path.cleanUpMetaData();
  auto end = std::chrono::steady_clock::now();
  std::chrono::duration<double> elapsed_seconds = end - start;
  std::cout << "Metadata cleanup finished, elapsed_time = " << elapsed_seconds.count() << " sec" << std::endl;
  report_memory();
}

/// @brief Test for performance to randomly insert percentage_of_points_to_insert% new nodes with random metadata.
void testRandomSinglePointInsertion(computational_geometry::ToolPath& tool_path, double percentage_of_points_to_insert,
                                    double nodes_percentage_with_string_data, double nodes_percentage_with_3d_vector, 
                                    int vector_data_size) {
  int n_points = tool_path.numPoints();
  std::cout << "Performing random insertion of " << percentage_of_points_to_insert << "% of the new path points"<< std::endl;
  int n_new_points = std::clamp(static_cast<int>((percentage_of_points_to_insert / 100.) * n_points), 1, n_points);
  //std::cout << "n_new_points = " << n_new_points << std::endl;
  std::default_random_engine location_generator;
  std::uniform_real_distribution<float> location_distribution(-1e+06, 1e+06);
  std::default_random_engine metadata_probability_generator;
  std::uniform_real_distribution<float> metadata_probability_distribution(0., 1.0);
  std::string comment_str(100, 'y');
  std::vector<float> data_1d(vector_data_size, 0.); // All 0s.
  std::vector<std::vector<float>> data_2d(vector_data_size, data_1d);
  computational_geometry::Data3D data_3d(vector_data_size, data_2d);
  float probability_comment = nodes_percentage_with_string_data / 100.;
  float probability_data = nodes_percentage_with_3d_vector / 100.;
  float probability_insertion = percentage_of_points_to_insert / 100.;
  auto start = std::chrono::steady_clock::now();
  int points_added = 0;
  for(tool_path.getFirst(); tool_path.getNext();) {
    if (points_added > n_new_points) {
      break;
    }

    float probability = metadata_probability_distribution(metadata_probability_generator);
    if (probability > probability_insertion) {
      continue;
    }
    points_added++;

    float x = location_distribution(location_generator);
    float y = location_distribution(location_generator);
    float z = location_distribution(location_generator);
    computational_geometry::Vector3D location{x, y, z};

    // As in the original tool_path object, 1% of new path points must have comment string.
    std::optional<std::string> comment_cur{std::nullopt};
    probability = metadata_probability_distribution(metadata_probability_generator);
    if (probability < probability_comment) {
      comment_cur = comment_str;
    }

    // As in the original tool_path object, 0.1% of new path points must have Data3D annotated.
    std::optional<computational_geometry::Data3D> data3d_cur{std::nullopt};
    probability = metadata_probability_distribution(metadata_probability_generator);
    if (probability < probability_data) {
      data3d_cur = data_3d;
    }
    tool_path.InsertPathPointAtCurrentPosition(location, comment_cur, data3d_cur);
  }
  tool_path.updatePointIndices();
  auto end = std::chrono::steady_clock::now();
  std::chrono::duration<double> elapsed_seconds = end - start;
  std::cout << "Random insertion of path points finished, elapsed_time = " << elapsed_seconds.count() << " sec" << std::endl;
  report_memory();
}

/// @brief Test for performance of single append and insert operations.
void testAppendAndInsert(int n_points_short, int step_coord_change_avg, double percentage_of_points_to_insert,
                         double nodes_percentage_with_string_data, double nodes_percentage_with_3d_vector, 
                         int vector_data_size) {
  // 1. Create 2 paths of the 1/3 size.
  std::cout << "Re-creating 2 ToolPath objects of 1/3 size..." << std::endl;
  auto start = std::chrono::steady_clock::now();
  computational_geometry::ToolPath tool_path_main(n_points_short);
  fillToolPath(tool_path_main, step_coord_change_avg, vector_data_size, 
               nodes_percentage_with_string_data, nodes_percentage_with_3d_vector);
  computational_geometry::ToolPath tool_path2(n_points_short);
  fillToolPath(tool_path2, step_coord_change_avg, vector_data_size, 
               nodes_percentage_with_string_data, nodes_percentage_with_3d_vector);
  auto end = std::chrono::steady_clock::now();
  std::chrono::duration<double> elapsed_seconds = end - start;
  std::cout << "2 ToolPath objects created, elapsed_time = " << elapsed_seconds.count() << " sec" << std::endl;
  report_memory();

  // 2. Making a copy of tool_path2 - we'll need it later for insertion test.
  std::cout << "Making a copy of the second ToolPath..." << std::endl;
  start = std::chrono::steady_clock::now();
  computational_geometry::ToolPath tool_path3(tool_path2);
  end = std::chrono::steady_clock::now();
  elapsed_seconds = end - start;
  std::cout << "Copy of ToolPath created, elapsed_time = " << elapsed_seconds.count() << " sec" << std::endl;
  report_memory();

  // 3. Appending tool_path2 to the end of tool_path.
  std::cout << "Appending second ToolPath objects to the end of the first one..." << std::endl;
  start = std::chrono::steady_clock::now();
  tool_path_main.append(tool_path2);
  end = std::chrono::steady_clock::now();
  elapsed_seconds = end - start;
  BOOST_TEST(tool_path_main.numPoints() == 2 * n_points_short);
  std::cout << "Addition done, elapsed_time = " << elapsed_seconds.count() << " sec" << std::endl;
  report_memory();
  
  // 4. Insert tool_path3 into the middle of combined path.
  std::cout << "Insertion of copy of second ToolPath object into the middle of combined ToolPath..." << std::endl;
  start = std::chrono::steady_clock::now();
  tool_path_main.insert(n_points_short, tool_path3);
  end = std::chrono::steady_clock::now();
  elapsed_seconds = end - start;
  BOOST_TEST(tool_path_main.numPoints() == 3 * n_points_short);
  std::cout << "Insertion done, combined ToolPath size = " << tool_path_main.numPoints() 
            << ", elapsed_time = " << elapsed_seconds.count() << " sec" << std::endl;
  report_memory();
}

/// @brief Test for performance of concatenation of n_sub_paths ToolPaths (amount should be large, i.e., 1000)
/// of n_points_sub_path size each (which is about 100000) into one single ToolPath sequentially, one after another,
/// assuming the order of them is the same as in input vector.
void testSequentialAppend(int n_sub_paths, int n_points_sub_path, int step_coord_change_avg, 
                          double percentage_of_points_to_insert, double nodes_percentage_with_string_data, 
                          double nodes_percentage_with_3d_vector, int vector_data_size) {
  // 1. Create n_sub_paths paths with n_points_sub_path points each and store them in vector.
  std::cout << "Creation of " << n_sub_paths << " ToolPaths of " << n_points_sub_path 
            << " size each and add them into the vector of pre-calculated ToolPaths..." << std::endl;
  auto start = std::chrono::steady_clock::now();
  std::vector<computational_geometry::ToolPath> tool_paths;
  tool_paths.reserve(n_sub_paths);
  for (int i = 0; i < n_sub_paths; i++) {
    computational_geometry::ToolPath sub_path(n_points_sub_path);
    fillToolPath(sub_path, step_coord_change_avg, vector_data_size, 
                 nodes_percentage_with_string_data, nodes_percentage_with_3d_vector);
    tool_paths.push_back(sub_path);
  }
  auto end = std::chrono::steady_clock::now();
  std::chrono::duration<double> elapsed_seconds = end - start;
  std::cout << "Generation of vector of tool paths is done, elapsed_time = " << elapsed_seconds.count() << " sec" << std::endl;
  report_memory();
  
  // 2. Concatenate pre-generated (on step 9) n_sub_paths paths into combined path sequentially, one after another.
  std::cout << "Concatenation of created " << n_sub_paths << " ToolPaths "
            << " into combined ToolPath sequentially..." << std::endl;
  start = std::chrono::steady_clock::now();
  computational_geometry::ToolPath tool_path_combined(tool_paths[0]);
  fillToolPath(tool_path_combined, step_coord_change_avg, vector_data_size, 
               nodes_percentage_with_string_data, nodes_percentage_with_3d_vector);
  for (int i = 1; i < n_sub_paths; i++) {
    tool_path_combined.append(tool_paths[i]);
  }
  tool_path_combined.updatePointIndices();
  end = std::chrono::steady_clock::now();
  elapsed_seconds = end - start;
  BOOST_TEST(tool_path_combined.numPoints() == n_sub_paths * n_points_sub_path);
  std::cout << "Sequential concatenation done, elapsed_time = " << elapsed_seconds.count() << " sec" << std::endl;
  report_memory();
}

/// @brief Test for performance of concatenation of n_sub_paths ToolPaths (amount should be large, i.e., 1000)
/// of n_points_sub_path size each (which is about 100000) into combined path all together in one shot, 
/// assuming the order in the input vector of paths is the same as order in input vector.
void testAppendInOneShot(int n_sub_paths, int n_points_sub_path, int step_coord_change_avg, 
                         double percentage_of_points_to_insert, double nodes_percentage_with_string_data, 
                        double nodes_percentage_with_3d_vector, int vector_data_size) {
  // 1. Create 1000 paths with 100000 points each and store them in vector.
  std::cout << "Creation of " << n_sub_paths << " ToolPaths of " << n_points_sub_path 
            << " size each and add them into the vector of pre-calculated ToolPaths..." << std::endl;
  auto start = std::chrono::steady_clock::now();
  std::vector<computational_geometry::ToolPath> tool_paths;
  tool_paths.reserve(n_sub_paths);
  for (int i = 0; i < n_sub_paths; i++) {
    computational_geometry::ToolPath sub_path(n_points_sub_path);
    fillToolPath(sub_path, step_coord_change_avg, vector_data_size, 
                 nodes_percentage_with_string_data, nodes_percentage_with_3d_vector);
    tool_paths.push_back(sub_path);
  }
  auto end = std::chrono::steady_clock::now();
  std::chrono::duration<double> elapsed_seconds = end - start;
  std::cout << "Generation of vector of tool paths is done, elapsed_time = " << elapsed_seconds.count() << " sec" << std::endl;
  report_memory();

  // 2. Combine vector of ToolPaths into one path in one shot, assuming the order of them
  // is the same as order of creation (i.e., order in the vector of paths).
  std::cout << "Concatenation of created " << n_sub_paths << " ToolPaths "
            << " into combined ToolPath in one shot..." << std::endl;
  start = std::chrono::steady_clock::now();
  computational_geometry::ToolPath tool_path_combined(tool_paths);
  end = std::chrono::steady_clock::now();
  elapsed_seconds = end - start;
  BOOST_TEST(tool_path_combined.numPoints() == n_sub_paths * n_points_sub_path);
  std::cout << "Multiple concatenation done, elapsed_time = " << elapsed_seconds.count() << " sec" << std::endl;
  report_memory();
}

struct ToolPathTestParams {
  int step_coord_change_avg = 20;
  int vector_data_size = 10;
  double nodes_percentage_with_string_data = 1.;
  double nodes_percentage_with_3d_vector = 0.1;
};

BOOST_AUTO_TEST_SUITE(tool_path_test_suite)

BOOST_AUTO_TEST_CASE(ConstructionTest)
{
  ToolPathTestParams dut;
  int n_points_total = 100000000;
  const bool debug_output = false;

  // 1. Create ToolPath and fill it's data according to spec, measure performance (runtime and memory).
  report_memory();
  std::cout << "Building ToolPath object..." << std::endl;
  auto start = std::chrono::steady_clock::now();
  computational_geometry::ToolPath tool_path(n_points_total);
  fillToolPath(tool_path, dut.step_coord_change_avg, dut.vector_data_size, 
               dut.nodes_percentage_with_string_data, dut.nodes_percentage_with_3d_vector);
  auto end = std::chrono::steady_clock::now();
  std::chrono::duration<double> elapsed_seconds = end - start;
  std::cout << "ToolPath object created, elapsed_time = " << elapsed_seconds.count() << " sec" << std::endl;
  report_memory();

  // 2. Track performance for sequential access of all data (full toolpath).
  testSequentialAccess(tool_path, debug_output);
  
  // 3. Track performance for random access of 10% of the data.
  double percentage_of_points_to_access = 10.;
  testRandomAccess(tool_path, percentage_of_points_to_access, debug_output);

  // 4. Track performance for downgrading points to the minimum 
  //    (replace all upgraded nodes with simplest version with no metadata).
  testMetadataCleanup(tool_path);

  // 5. Track performance to randomly insert 10% new nodes with random metadata as above.
  double percentage_of_points_to_insert = 10.;
  testRandomSinglePointInsertion(tool_path, percentage_of_points_to_insert, 
                                 dut.nodes_percentage_with_string_data, dut.nodes_percentage_with_3d_vector, 
                                 dut.vector_data_size);
}

BOOST_AUTO_TEST_CASE(ConcatenationTest)
{
  ToolPathTestParams dut;
  int n_points_total = 100000000;
  double percentage_of_points_to_insert = 10.;

  // 6. Track performance of insert() and append() operations for paths of 1/3 of original size.
  int n_points_short = n_points_total / 3;
  testAppendAndInsert(n_points_short, dut.step_coord_change_avg, percentage_of_points_to_insert,
                      dut.nodes_percentage_with_string_data, dut.nodes_percentage_with_3d_vector, dut.vector_data_size);

  // 7. Track performance of creation of 1000 paths with 100000 points each and 
  // concatenating them into combined path sequentially, one after another.
  int n_sub_paths = 1000;
  int n_points_sub_path = 100000;
  testSequentialAppend(n_sub_paths, n_points_sub_path, dut.step_coord_change_avg, percentage_of_points_to_insert,
                       dut.nodes_percentage_with_string_data, dut.nodes_percentage_with_3d_vector, dut.vector_data_size);

  // 8. Track performance of creation of 1000 paths with 100000 points each and concatenating them into combined path 
  // all together in one shot, assuming the order in the input vector of paths is the same as order of creation.
  testAppendInOneShot(n_sub_paths, n_points_sub_path, dut.step_coord_change_avg, percentage_of_points_to_insert,
                      dut.nodes_percentage_with_string_data, dut.nodes_percentage_with_3d_vector, dut.vector_data_size);
}

BOOST_AUTO_TEST_SUITE_END()

