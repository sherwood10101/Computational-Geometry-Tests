#pragma once

#include <vector>
#include <string>
#include <iostream>

namespace computational_geometry {

/*  // usage from poisson_recon.cc - params should correspond to these //
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
    bool showResidual = false; // option is "showResidual"
    bool noComments = false; // option is "noComments"
    bool polygonMesh = false; // option is "polygonMesh"
    bool normalWeights = false; // option is "nWeights"; is this bool?
    bool nonManifold = false; //option is "nonManifold"
    bool ascii = false; // option is "ascii"
    bool density = false; // option is "density" - is this bool?
    bool linearFit = false; // option is "linearFit"
    bool primalVoxel = false; // option is "primalVoxel"
    bool useDouble = false; // option is "double"
    bool verbose = false; // option is "verbose"
    int degree = 1; // option is "degree"
    int depth = 8; // option is "depth"
    int cgDepth = 0; // option is "cgDepth"
    int kernelDepth = 0; // option is "kernelDepth"
    int adaptiveExponent = 1; // option is "adaptiveExp"
    int iters = 8; // option is "iters"
    int voxelDepth = -1; // option is "voxelDepth"
    int fullDepth = 5; // option is "fullDepth"
    int maxSolveDepth = 16; // option is "maxSolveDepth" - not sure default is right
    int threads = 0; // option is "threads"
    float samplesPerNode = 1.5f; // option is "scamplesPerNode"
    float scale = 1.1f; // option is "scale"
    float confidence = 0.0f; // option is "confidence"
    float cgSolverAccuracy = 1.0e-3f; // option is "cgAccuracy"
    float pointWeight = 4.0f; // option is "pointWeight"
    float color = -1.f; // option is "color"; typ value is 16.f ??  -1. means disabled
    int bType = 1; // option is "bType";  I think this can be 0, 1, 2 for BOUNDARY_FREE, BOUNDARY_NEUMANN, BOUNDARY_DIRICHLET but not sure
};



// use this constructor to send only these specific items
class PoissonMeshReconstructor {
  public:
    PoissonMeshReconstructor(const std::string& input_points_file, 
                             const std::string& output_ply_file,
                             const int depth,
                             const int argc,
                             char** argv) :     m_input_file(input_points_file),
                                                m_output_ply_file(output_ply_file),
                                                m_depth(depth),
                                                m_argc(argc), 
                                                m_argv(argv) 
    {
        m_useSyntheticParams = false; // so later you can tell if it was command line or synthetic params
    }

    // use this constructor with regular command line arguments
    PoissonMeshReconstructor(const int argc,
                             char** argv) : 
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
        

        /* // Example PoissonParams struct data //
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

        // this is tedious, but we have to convert params to strings and add them to the m_arguments vector
        // then convert the m_arguments to a synthetic argv char**

        m_arguments = { "PoissonRecon", "--in", const_cast<char*>(m_inputparams.inputFilename.c_str()),
                        "--out", const_cast<char*>( m_inputparams.outputFilename.c_str())};
        
        if(m_inputparams.tempDir != "") {
            m_arguments.push_back("--tempDir");
            m_arguments.push_back(m_inputparams.tempDir);
        }
        if(m_inputparams.voxelGridFilename != "") {
            m_arguments.push_back("--voxel");
            m_arguments.push_back(m_inputparams.voxelGridFilename);
        }
        if(m_inputparams.xformFilename != "") {
            m_arguments.push_back("--xForm");
            m_arguments.push_back(m_inputparams.xformFilename);
        }

        if(m_inputparams.showResidual)
            m_arguments.push_back("--showResidual");

        if(m_inputparams.noComments)
            m_arguments.push_back("--noComments");
        
        if(m_inputparams.polygonMesh)
            m_arguments.push_back("--polygonMesh");
        
        if(m_inputparams.normalWeights)
            m_arguments.push_back("--nWeights");

        if(m_inputparams.nonManifold)
            m_arguments.push_back("--nonManifold");

        if(m_inputparams.ascii)
            m_arguments.push_back("--ascii");

        if(m_inputparams.density)
            m_arguments.push_back("--density");

        if(m_inputparams.linearFit)
            m_arguments.push_back("--inearFit");

        if(m_inputparams.primalVoxel)
            m_arguments.push_back("--primalVoxel");
        
        if(m_inputparams.useDouble)
            m_arguments.push_back("--double");

        if(m_inputparams.verbose)
            m_arguments.push_back("--verbose");

        if(m_inputparams.degree > 0) {
            std::string degreestr = std::to_string(m_inputparams.degree);
            char const *degree_char = degreestr.c_str();
            m_arguments.push_back("--degree");
            m_arguments.push_back(const_cast<char*>(degree_char));
        }

        if(m_inputparams.depth > 0) {
            std::string depthstr = std::to_string(m_inputparams.depth);
            char const *depth_char = depthstr.c_str();
            m_arguments.push_back("--depth");
            m_arguments.push_back(const_cast<char*>(depth_char));
        }

        if(m_inputparams.cgDepth >= 0) {
            std::string cgDepthstr = std::to_string(m_inputparams.cgDepth);
            char const *cgDepth_char = cgDepthstr.c_str();
            m_arguments.push_back("--cgDepth");
            m_arguments.push_back(const_cast<char*>(cgDepth_char));
        }

        if(m_inputparams.kernelDepth >= 0) {
            std::string kernelDepthstr = std::to_string(m_inputparams.kernelDepth);
            char const *kernelDepth_char = kernelDepthstr.c_str();
            m_arguments.push_back("--kernelDepth");
            m_arguments.push_back(const_cast<char*>(kernelDepth_char));
        }

        if(m_inputparams.adaptiveExponent > 0) {
            std::string adaptiveExponentstr = std::to_string(m_inputparams.adaptiveExponent);
            char const *adaptiveExponent_char = adaptiveExponentstr.c_str();
            m_arguments.push_back("--adaptiveExp");
            m_arguments.push_back(const_cast<char*>(adaptiveExponent_char));
        }

        if(m_inputparams.iters > 0) {
            std::string itersstr = std::to_string(m_inputparams.iters);
            char const *iters_char = itersstr.c_str();
            m_arguments.push_back("--iters");
            m_arguments.push_back(const_cast<char*>(iters_char));
        }

        if(m_inputparams.voxelDepth > 0) {
            std::string voxelDepthstr = std::to_string(m_inputparams.voxelDepth);
            char const *voxelDepth_char = voxelDepthstr.c_str();
            m_arguments.push_back("--voxelDepth");
            m_arguments.push_back(const_cast<char*>(voxelDepth_char));
        }

        if(m_inputparams.fullDepth > 0) {
            std::string fullDepthstr = std::to_string(m_inputparams.fullDepth);
            char const *fullDepth_char = fullDepthstr.c_str();
            m_arguments.push_back("--fullDepth");
            m_arguments.push_back(const_cast<char*>(fullDepth_char));
        }


        if(m_inputparams.maxSolveDepth > 0) {
            std::string maxSolveDepthstr = std::to_string(m_inputparams.maxSolveDepth);
            char const *maxSolveDepth_char = maxSolveDepthstr.c_str();
            m_arguments.push_back("--maxSolveDepth");
            m_arguments.push_back(const_cast<char*>(maxSolveDepth_char));
        }

        if(m_inputparams.threads >= 0) {
            std::string threadsstr = std::to_string(m_inputparams.threads);
            char const *threads_char = threadsstr.c_str();
            m_arguments.push_back("--threads");
            m_arguments.push_back(const_cast<char*>(threads_char));
        }

        if(m_inputparams.samplesPerNode > 0.0f) {
            std::string samplesPerNodestr = std::to_string(m_inputparams.samplesPerNode);
            char const *samplesPerNode_char = samplesPerNodestr.c_str();
            m_arguments.push_back("--samplesPerNode");
            m_arguments.push_back(const_cast<char*>(samplesPerNode_char));
        }

        if(m_inputparams.scale > 0.0f) {
            std::string scalestr = std::to_string(m_inputparams.scale);
            char const *scale_char = scalestr.c_str();
            m_arguments.push_back("--scale");
            m_arguments.push_back(const_cast<char*>(scale_char));
        }

        if(m_inputparams.confidence >= 0.0f) {
            std::string confidencestr = std::to_string(m_inputparams.confidence);
            char const *confidence_char = confidencestr.c_str();
            m_arguments.push_back("--confidence");
            m_arguments.push_back(const_cast<char*>(confidence_char));
        }

        if(m_inputparams.cgSolverAccuracy > 0.0f) {
            std::string cgAccuracystr = std::to_string(m_inputparams.cgSolverAccuracy);
            char const *cgAccuracy_char = cgAccuracystr.c_str();
            m_arguments.push_back("--cgAccuracy");
            m_arguments.push_back(const_cast<char*>(cgAccuracy_char));
        }

        if(m_inputparams.pointWeight >= 0.0f) {
            std::string pointWeightstr = std::to_string(m_inputparams.pointWeight);
            char const *pointWeight_char = pointWeightstr.c_str();
            m_arguments.push_back("--pointWeight");
            m_arguments.push_back(const_cast<char*>(pointWeight_char));
        }

        if(m_inputparams.color > 0.0f) {
            std::string colorstr = std::to_string(m_inputparams.color);
            char const *color_char = colorstr.c_str();
            m_arguments.push_back("--color");
            m_arguments.push_back(const_cast<char*>(color_char));
        }

        if(m_inputparams.bType >= 0) {
            std::string bTypestr = std::to_string(m_inputparams.bType);
            char const *bType_char = bTypestr.c_str();
            m_arguments.push_back("--bType");
            m_arguments.push_back(const_cast<char*>(bType_char));
        }



        // now convert to vector of char*
        for (const auto& arg : m_arguments)
            m_argVec.push_back((char*)arg.data());
        m_argVec.push_back(nullptr);

        m_argc = m_argVec.size() - 1;

    }                                                

    /// Top-level routine to run mesh reconstruction and write out PLY file.
    void Run();
  
  private:
     PoissonParams m_inputparams;
     std::vector<std::string> m_arguments;
     std::string m_input_file;
     std::string m_output_ply_file;
     int m_depth;
     int m_argc;
     char** m_argv;
     std::vector<char*> m_argVec;
     bool m_useSyntheticParams;
};

  
} // namespace computational_geometry
