#pragma once

#include <vector>
#include <string>
#include <iostream>

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
    PoissonMeshReconstructor(const std::string& input_points_file, 
                             const std::string& output_ply_file,
                             //const int depth,
                             const int argc,
                             char** argv) :     m_input_file(input_points_file),
                                                m_output_ply_file(output_ply_file),
                                                //m_depth(depth),
                                                m_argc(argc), 
                                                m_argv(argv) 
      {
        m_useSyntheticParams = false; // so later you can tell if it was command line or synthetic params
      }

    // constructor that uses only the PoisonParams struct
    PoissonMeshReconstructor(const PoissonParams& p_params) : m_inputparams(p_params) 
    {
        m_useSyntheticParams = true; // so later you can tell if it was command line or synthetic params
        
        m_input_file = m_inputparams.inputFilename;
        m_output_ply_file = m_inputparams.outputFilename;
        
        // turn paramsTest into argv, argc here




        std::vector<std::string> arguments = { "PoissonRecon", "--in", const_cast<char*>(m_inputparams.inputFilename.c_str()),
	                 "--out", const_cast<char*>( m_inputparams.outputFilename.c_str())};
	                 //"--depth", const_cast<char*>(depth_char)};


/*
  paramsTest.programName = "computational_geometry_template_main.cc";
  paramsTest.inputFilename = points_file;
  paramsTest.outputFilename = argv[2];
  paramsTest.tempDir = "";
  //paramsTest.tempDir = "tempdir";
  paramsTest.voxelGridFilename = "voxelfile";
  paramsTest.xformFilename = "xformfile";
  paramsTest.showResidual = false;
  paramsTest.noComments = false;
  paramsTest.polygonMesh = false;
  paramsTest.normalWeights = false;
  paramsTest.nonManifold = false;
  paramsTest.ascii = false;
  paramsTest.density = false;
  paramsTest.linearFit = false;
  paramsTest.primalVoxel = false;
  paramsTest.useDouble = false;
  paramsTest.verbose = true;
  paramsTest.degree = 1;
  paramsTest.depth = 9;
  paramsTest.cgDepth = 0;
  paramsTest.kernelDepth = 0;
  paramsTest.adaptiveExponent = 1;
  paramsTest.iters = 8;
  paramsTest.voxelDepth = -1;
  paramsTest.fullDepth = 5;
  paramsTest.maxSolveDepth = 16;
  paramsTest.threads = 0;
  paramsTest.samplesPerNode = 1.5f;
  paramsTest.scale = 1.1f;
  paramsTest.confidence = 0.0f;
  paramsTest.cgSolverAccuracy = 1.0e-3f;
  paramsTest.pointWeight = 4.0f;
  paramsTest.color = 16.f;
  paramsTest.bType = 1; // I think options are 0, 1, 2 but not sure
*/
        // this is tedious, but we have to convert params to strings and add them to the arguments vector
        // then convert the arguments to a synthetic argv char**
        
        if(m_inputparams.tempDir != "") {
            arguments.push_back("--tempDir");
            arguments.push_back(m_inputparams.tempDir);
        }
  

        if(m_inputparams.useDouble) 
            arguments.push_back("--double");
        arguments.push_back("--bType");
        arguments.push_back("1");
        arguments.push_back("--iters");
        arguments.push_back("10");

        if(m_inputparams.verbose)
            arguments.push_back("--verbose");


        std::string depthstr = std::to_string(m_inputparams.depth);
	      char const *depth_char = depthstr.c_str();
        arguments.push_back("--depth");
        arguments.push_back(const_cast<char*>(depth_char));

        // now convert to vector of char*
        for (const auto& arg : arguments)
            m_argVec.push_back((char*)arg.data());
        m_argVec.push_back(nullptr);

        m_argc = m_argVec.size() - 1;

        // assign address of start of data to m_argv (final synthetic version of argv)
        m_argv = m_argVec.data();

        std::cout << "number of arguments: " << m_argc << std::endl;
        std::cout << "printing m_argv inside constructor: " << std::endl;
        for (int i = 0; i < m_argc; i++)
            std::cout << m_argv[i] << std::endl;
        std::cout << "press enter to continue" << std::endl;
        std::cin.get();

    }                                                

    /// Top-level routine to run mesh reconstruction and write out PLY file.
    void Run();
  
  private:
     PoissonParams m_inputparams;
     std::string m_input_file;
     std::string m_output_ply_file;
     //int m_depth;
     int m_argc;
     char** m_argv;
     std::vector<char*> m_argVec;
     bool m_useSyntheticParams;
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
