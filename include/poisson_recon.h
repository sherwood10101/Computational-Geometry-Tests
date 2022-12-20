#pragma once

#include <vector>
#include <string>

namespace computational_geometry {

/*
possible options are:
  degree
  double
  bType
  in
  depth
  out
  xForm
  scale
  verbose
  cgAccuracy
  noComments
  iterMultiplier
  kernelDepth
  samplesPerNode
  confidence
  nWeights
  nonManifold
  polygonMesh
  ascii
  showResidual
  voxelDepth
  pointWeight
  voxel
  threads
  maxSolveDepth
  adaptiveExp
  density
  fullDepth
  cgDepth
  iters
  color
  linearFit
  primalVoxel
  tempDir
*/

// missing: iterMultiplier
struct PoissonParams
{
    std::string programName; // added - becomes argv[0]
    std::string inputFilename; // option is "in"
    std::string outputFilename; // option is "out"
    std::string tempDir; // option is "tempDir"
    std::string voxelGridFilename; // option is "voxel" ??
    std::string xformFilename; // option is "xForm"
    //bool performance = false; // not in poisson_recon.cc?
    bool showResidual = false; // option is "show Residual"
    bool noComments = false; // option is "noComments"
    bool polygonMesh = false; // option is "polygonMesh"
    bool normalWeights = false; // option is "nWeights"; is this bool?
    bool nonManifold = false; //option is "nonManifold"
    bool ascii = false; // option is "ascii"
    bool density = false; // option is "density" - is this bool?
    bool linearFit = false; // option is "linearFit"
    bool primalVoxel = false; // option is "primavoxel"
    bool useDouble = false; // option is "double"
    //bool exactInterpolation = false;
    //bool normals = false;
    //bool colors = false;  // ?
    //bool inCore = false;
    bool verbose = false; // option is "verbose"
    int degree = 1; // option is "degree"
    int depth = 8; // option is "depth"
    int cgDepth = 0; // option is "cgDepth"
    int kernelDepth = 0; // option is "kernelDepth"
    int adaptiveExponent = 1; // option is "adaptiveExp"
    int iters = 8; // option is "iters"
    int voxelDepth = -1; // option is "voxelDepth"
    int fullDepth = 5; // option is "fullDepth"
    //int baseDepth = 0; // not in poisson_recon.cc?
    //int baseVCycles = 1; // not in poisson_recon.cc?
    //int maxMemoryGB = 0; // not in poisson_recon.cc?
    //int threadChunkSize = 128; // not in poisson_recon.cc?
    int maxSolveDepth = 16; // option is "maxSolveDepth" - not sure default is right
    int threads = 0; // option is "threads"
    //float dataX = 32.0f;
    float samplesPerNode = 1.5f; // option is "scamplesPerNode"
    float scale = 1.1f; // option is "scale"
    //float width = 0.0f;
    float confidence = 0.0f; // option is "confidence"
    //float confidenceBias = 0.0f;
    float cgSolverAccuracy = 1.0e-3f; // option is "cgAccuracy"
    //float lowResIterMultiplier = 1.f; // 
    float pointWeight = 4.0f; // option is "pointWeight"
    float color = 16.f; // option is "color"
    int bType = 1; // option is "bType";  I think this can be 0, 1, 2 for BOUNDARY_FREE, BOUNDARY_NEUMANN, BOUNDARY_DIRICHLET but not sure
    //std::string threadPoolScheduleType = "Dynamic"; // 
    //std::string threadPoolParallelType = "THREAD_Pool"; // 
};




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

    // constructor that uses only the PoisonParams struct
    PoissonMeshReconstructor(const PoissonParams& paramsTest) : m_paramsTest(paramsTest) 
    {
        m_input_file = m_paramsTest.inputFilename;
        m_output_ply_file = m_paramsTest.outputFilename;
        
        // need to turn paramsTest into argv, argc here


        std::string tmp = std::to_string(m_paramsTest.depth);
	      char const *depth_char = tmp.c_str();

        std::vector<std::string> arguments = { "PoissonRecon", "--in", const_cast<char*>(m_paramsTest.inputFilename.c_str()),
	                 "--out", const_cast<char*>( m_paramsTest.outputFilename.c_str()),
	                 "--depth", const_cast<char*>(depth_char)};
    }                                                

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
