// ToolPath test application.
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

void fill_tool_path(computational_geometry::ToolPath& tool_path, int step_coord_change_avg, int vector_data_size,
                    double nodes_percentage_with_string_data, double nodes_percentage_with_3d_vector) {
  // Perform random coordinate changes for approximately  every step_coord_change_avg points.

  std::default_random_engine step_generator;
  double sigma = std::clamp(step_coord_change_avg / 2., 5., 100.);
  std::normal_distribution<double> step_distribution(static_cast<double>(step_coord_change_avg), sigma);

  std::default_random_engine coord_index_generator;
  std::uniform_int_distribution<int> coord_index_distribution(0,2);

  std::default_random_engine velocity_generator;
  std::uniform_real_distribution<double> velocity_distribution(-5.0,10.0);

  int n_points = tool_path.numPoints();
  computational_geometry::Vector3D location_prev;
  int step = 1;
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
    // std::cout << "step = " << step << std::endl;
    i += step;
  }

  // Finalize locations initialization (update location for last chunk of path points).
  tool_path.finalizeLocations();

  // TODO: Randomly upgrade nodes_percentage_with_string_data % of the points to hold a 100 character string.
  std::string comment(100, 'x');

  // TODO: Randomly upgrade nodes_percentage_with_3d_vector % of the points to also hold a 3D array of floats
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

int main (const int argc, char **const argv) 
{
  if (argc != 1) {
    std::cout << "Incorrect usage, must be: tool_path_test" << std::endl;
    return -1;
  }

  int n_points_total = 100000000;
  int step_coord_change_avg = 20;
  int vector_data_size = 10;
  double nodes_percentage_with_string_data = 1.;
  double nodes_percentage_with_3d_vector = 0.1;

  // 1. Create ToolPath and fill it's data according to spec, measure performance (runtime and memory).
  report_memory();
  std::cout << "Building ToolPath object..." << std::endl;
  auto start = std::chrono::steady_clock::now();
  computational_geometry::ToolPath tool_path(n_points_total);
  fill_tool_path(tool_path, step_coord_change_avg, vector_data_size, 
                 nodes_percentage_with_string_data, nodes_percentage_with_3d_vector);
  auto end = std::chrono::steady_clock::now();
  std::chrono::duration<double> elapsed_seconds = end - start;
  std::cout << "ToolPath object created, elapsed_time = " << elapsed_seconds.count() << " sec" << std::endl;
  report_memory();

  // 2. TODO: Track performance for sequential access of all data (full toolpath).
  
  // 3. TODO: Track performance for random access of 10% of the data.

  // 4. TODO: Track performance for downgrading points to the minimum 
  //    (replace all upgraded nodes with simplest version with no metadata).

  // 5. TODO: Track performance to randomly insert 10% new nodes with random metadata as above.

  return 0;
}
