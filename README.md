# computational_geometry_project_template

Template project using Visual Studio Code Remote environment using development container. It can be run on both Windows and Linux. It integrates following libraries:

 - [OpenMesh](https://www.graphics.rwth-aachen.de/software/openmesh/)
 - [Geometric Tools](https://www.geometrictools.com/)
 - [Polyscope](https://polyscope.run/)

It does following:

 1. Loads STL or PLY file (both ascii and binary formats are allowed)
 2. Perform some simple calculation (check and list statistics about face areas) and prints it to stdout to terminal.
 3. Displays the mesh using polyscope.
 4. To demonstrate integration of Geometric Tools, finds out intersection between line and plane and reports it to stdout to terminal.
 5. Performs scaling of the mesh, and then output a resulting mesh file (user is able to choose STL, or PLY in ascii or binary).

# Pre-requisites

 1. Install VS Code, docker and VS Code remote development extension, instructions are [here](https://code.visualstudio.com/docs/remote/containers#_installation)
 2. [Install Docker Desktop](https://www.docker.com/products/docker-desktop/)

# Build and Run

## Build the project

### Step 1 - Launch VS code and build container

 1. Launch VS Code
 2. Run the "Remote-Containers: Open Folder in Container..." command from the Command Palette (F1)
 3. Open folder where this project was cloned. It will start building the container.
 4. When step 3 is completed, bring another terminal in VS Code (Ctrl+Shift+~). Now you are ready to build.

### Step 2 - Build computational_geometry_template application

```
./build_all.sh
```

### Step 3 - Run computational_geometry_template application

```
./install/bin/computational_geometry_template ./examples/bottle_ascii.stl
```
 


 