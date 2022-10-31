# computational_geometry_project_template

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

# Build and Run

## Build the project

### Step 1 - Launch VS code and build container

 1. To be able to run GUI using OpenGL from docker container, need to install VcXsrv Windows X Server. The process is described [here](https://dev.to/darksmile92/run-gui-app-in-linux-docker-container-on-windows-host-4kde), please follow instructions in "How to share the display from a windows host?" section. The only note - no need to install it through Chocolatey, just run downloaded installer executable, then start Xlaunch and follow instructions in this section (save config to %userprofile% folder as extracted, at least, it worked for me). No need to create dockerfile etc after that, it is all will be done within VS Code later.
 2. Launch VS Code
 3. Run the "Remote-Containers: Open Folder in Container..." command from the Command Palette (F1)
 4. Open folder where this project was cloned. It will start building the container.
 5. When step 4 is completed, bring another terminal in VS Code (Ctrl+Shift+~). Now you are ready to build.

### Step 2 - Build computational_geometry_template application

```
./build_all.sh
```

### Step 3 - Run computational_geometry_template application

```
./install/bin/computational_geometry_template ./examples/airplane_ascii.ply ./airplane_ascii_poisson_reconstructed.ply
```
 


 