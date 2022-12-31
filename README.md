# computational_geometry_template

Template project using Visual Studio Code Remote environment using development container. It can be run on both Windows and Linux. It integrates following libraries:

 - [OpenMesh](https://www.graphics.rwth-aachen.de/software/openmesh/)
 - [Geometric Tools](https://www.geometrictools.com/)
 - [Polyscope](https://polyscope.run/)

It does following:

 1. Loads STL or PLY file (both ascii and binary formats are allowed)
 2. Perform some simple calculation (check and list statistics about total number of vertices and faces and total area of faces) and prints it to stdout to terminal.
 3. Using vertices and vertex normals from loaded mesh, writes out temporary point cloud file and calls PoissonRecon algorithm to reconstruct triangular mesh and write it to output PLY file.
 4. Displays the mesh using polyscope.
 5. To demonstrate integration of Geometric Tools, computes minimum volume 3D bounding box of the mesh and reports it's volume.

# Pre-requisites

 1. Install VS Code, docker and VS Code remote development extension, instructions are [here](https://code.visualstudio.com/docs/remote/containers#_installation)
 2. [Install Docker Desktop](https://www.docker.com/products/docker-desktop/)


# tool_path_test

Project to demo/test ToolPath library.  It does following:

- creates a tool path data structure
- adds optional upgraded node storage for certain nodes to include x, y, or z or floating point metadata plus An optional string field plus an optional large 3D array (10 x 10 x 10 vector of vectors of vectors of float) that  can be attached occasionally to any point.
- populates Toolpath instance with 100 million points (nodes), starting with only x,y,z data where each x or y or z coordinate changes randomly approximately every 20 points. Then randomly upgrade 1% of the points to hold a 100 character string.  Then upgrade 0.1% of the points to also hold a 3D array of floats
- tracks performance (time and storage space) for creation
- tracks performance for sequential access of all data (full toolpath)
- tracks performance for random access of 10% of the data
- tracks performance for downgrading points to the minimum (i.e., replace all upgraded nodes with simplest version with no metadata)
- tracks performance to randomly insert 10% new nodes with random metadata as above
- tracks performance of creation of 2 paths of the 1/3 size and add one path to the end of another.
- tracks performance of insertion of copy of one of the above paths into the middle of combined path.
- tracks performance of creation of 1000 paths with 100000 points each and concatenating them into combined path sequentially, one after another.
- tracks performance of creation of 1000 paths with 100000 points each and concatenating them into combined path all together in one shot, assuming the order in the input vector of paths is the same as order of creation.

# Build and Run

## Build the project

### Step 1 - Launch VS code and build container

 1. To be able to run GUI using OpenGL from docker container, need to install VcXsrv Windows X Server. The process is described [here](https://dev.to/darksmile92/run-gui-app-in-linux-docker-container-on-windows-host-4kde), please follow instructions in "How to share the display from a windows host?" section. The only note - no need to install it through Chocolatey, just run downloaded installer executable, then start Xlaunch and follow instructions in this section (save config to %userprofile% folder as extracted, at least, it worked for me). No need to create dockerfile etc after that, it is all will be done within VS Code later.
 2. Launch VS Code
 3. Run the "Remote-Containers: Open Folder in Container..." command from the Command Palette (F1)
 4. Open folder where this project was cloned. It will start building the container.
 5. When step 4 is completed, bring another terminal in VS Code (Ctrl+Shift+~). Now you are ready to build.

### Step 2 - Build computational_geometry_template and tool_path_test applications

```
./build_all.sh
```
If you get "command not found", you can manually create the build directory and manually run the cmake command from the script

### Step 3 - Run computational_geometry_template and/or tool_path_test applications

To run computational_geometry_template:

```
./install/bin/computational_geometry_template ./examples/airplane_ascii.ply ./airplane_ascii_poisson_reconstructed.ply
```

To run ToolPath unit tests:

```
./run_tests_all.sh
```



 
