#pragma once

#include <vector>
#include <string>

namespace computational_geometry {


struct PoissonParams
{
    std::string programName; // added - becomes argv[0]
    std::string inputFilename; //
    std::string outputFilename; //
    std::string tempDir; //
    std::string voxelGridFilename; //
    std::string treeFilename; // not in poisson_recon.cc?
    std::string xformFilename; //
    bool performance = false; // not in poisson_recon.cc?
    bool showResidual = false; //
    bool noComments = false; //
    bool polygonMesh = false; //
    bool normalWeights = false; // added. is this bool?
    bool nonManifold = false; //
    bool ascii = false; //
    bool density = false; //
    bool linearFit = false; //
    bool primalVoxel = false; //
    bool useDouble = false; // added. is this bool?
    bool exactInterpolation = false;
    bool normals = false;
    bool colors = false;
    bool inCore = false;
    bool verbose = false; //
    int degree = 1; //
    int depth = 8; //
    int cgDepth = 0; // added
    int kernelDepth = 0; //
    int adaptiveExponent = 1; // added
    int iters = 8; //
    int voxelDepth = -1; // added
    int fullDepth = 5; //
    int baseDepth = 0; // not in poisson_recon.cc?
    int baseVCycles = 1; // not in poisson_recon.cc?
    int maxMemoryGB = 0; // not in poisson_recon.cc?
    int threadChunkSize = 128; // not in poisson_recon.cc?
    int maxSolveDepth = 16; // added.  Is this right?
    int threads = 0; //
    float dataX = 32.0f;
    float samplesPerNode = 1.5f;
    float scale = 1.1f; //
    float width = 0.0f;
    float confidence = 0.0f; //
    float confidenceBias = 0.0f;
    float cgSolverAccuracy = 1.0e-3f; //
    float lowResIterMultiplier = 1.f; // added
    float pointWeight = 4.0f; //
    float color = 16.f; // added
    std::string bType = "BOUNDARY_NEUMANN+1"; // added
    std::string threadPoolScheduleType = "Dynamic"; // added
    std::string threadPoolParallelType = "THREAD_Pool"; // added
};

//void testArgvCreator(int argctest, char** argvtest, std::vector<std::string>& arguments);

/*! Create command line argc and argv from input parameters
 *  So that you can use Poisson mesh reconstruction functions that require command line input
 */
//void makeCommandLineFromParams(int& argc, char* argv[], const PoissonParams& params);


/// Top-level class to perform Poisson mesh reconstruction of point cloud with vertex normals
// this wone doesn't work yet - need to fix to use PoissonParams struct
/*
class PoissonMeshReconstructor {
  public:
    PoissonMeshReconstructor(const PoissonParams& params) : 
                             m_params(params) {}
    
    /// Top-level routine to run mesh reconstruction and write out PLY file.
    void Run();

    private:
      PoissonParams m_params;

};
*/

class PoissonMeshReconstructor {
  public:
    PoissonMeshReconstructor(const PoissonParams& paramsTest,
                             const std::string& input_points_file, 
                             const std::string& output_ply_file,
                             const int depth,
                             const int argc,
                             char** argv) : m_paramsTest(paramsTest),
                                                m_input_file(input_points_file),
                                                m_output_ply_file(output_ply_file),
                                                m_depth(depth),
                                                m_argc(argc), 
                                                m_argv(argv) {}

    /// Top-level routine to run mesh reconstruction and write out PLY file.
    void Run();
  
  private:
     PoissonParams m_paramsTest;
     std::string m_input_file;
     std::string m_output_ply_file;
     int m_depth;
     int m_argc;
     char** m_argv;
};

//  Old version for reference (works but doesn't let you specify all the parameters)
/*
class PoissonMeshReconstructor {
  public:
    PoissonMeshReconstructor(const PoissonParams& paramsTest,
                             const std::string& input_points_file, 
                             const std::string& output_ply_file,
                             const int depth) : m_paramsTest(paramsTest),
                                                m_input_file(input_points_file),
                                                m_output_ply_file(output_ply_file),
                                                m_depth(depth) {}

    /// Top-level routine to run mesh reconstruction and write out PLY file.
    void Run();
  
  private:
     PoissonParams m_paramsTest;
     std::string m_input_file;
     std::string m_output_ply_file;
     int m_depth;
};
*/
  
} // namespace computational_geometry
