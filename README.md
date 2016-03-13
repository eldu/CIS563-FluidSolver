# CIS563-FluidSolver
(Credit : CIS565 README)


## 3D Viewer

CONTROLS
Arrows Keys to rotate around the screen.


#### How the code works
##### The Gist
Main -> Calls Viewer -> Draws window -> Creates all of the objects (JSON File is parsed to create Fluid Solver which displays all of the particles) -> Draw Loop (Draw everything possible) -> Destroy everything

##### Main
- Calls Viewer

##### Viewer
- This is where the window is created and where everything is drawn and destroyed.

##### Camera
- Camera - Sets the camera object for the viewport
- Controls - Implemented arrows keys to rotate around scene. 
TODO: Improve controls to include mouse input and panning. Also, instead of recomputing the camera every frame, simply check if there has been an event. Unfortaunte event callback functions may only make use of static objects and such. Therefore as Austin suggested, I may create a singleton class. The controls and input files don't currently do anything because I coudn't figure it out... But this is an improvement to be done later.

##### Fluid Solver
- fluidParticle - Represents 1 particle
- fluidSolver - Updates fluid position, velocity, and check collision with the caontainer

##### Geometry
- Geometry - Class that all geometries extend, has collision function
- Cube - Creates a sube
- Triangle - Creates a Triangle

##### Scene
- scene.cpp/scene.hpp - Nothing.
- jsonreader - parses files into JSON format and into a FluidSolver object

##### Shaders
- Shaders from the opengl-tutorials

#### Unimplemented Features
- Collision And Changing Color
- Camera Controls
- Gravity Simulation

#### References
- www.opengl-tutorials.org (Tutorials 1-4, 6, )
- www.cplusplus.com and nuparu (Reading from a file, JSON Parsing)
- CIS 277 la.h (linear algebra file)

Fluid Solver Submission guidelines:


- If you have modified any of the CMakeLists.txt files at all (aside from the list of CORE_SRC), you must test that your project can build. Beware of any build issues.

- Open a GitHub pull request so that we can see that you have finished. The title should be "Submission: YOUR NAME".

- In the body of the pull request, include a link to your repository.

- Submit on canvas with a direct link to your pull request on GitHub


And you're done!