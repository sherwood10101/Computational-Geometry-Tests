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
