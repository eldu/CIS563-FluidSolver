//
//  viewer.cpp
//  Thanda

#include "viewer.hpp"
#include "../camera/camera.hpp"
#include "../shaders/shader.hpp"
#include "../camera/controls.hpp"
#include "../geom/cube.hpp"
#include "../geom/triangle.hpp"

Viewer::Viewer(int width, int height) {
    x = width;
    y = height;
}

Viewer::Viewer() {
    x = 1024;
    y = 768;
}

Viewer::~Viewer() {
}

int Viewer::initialize() {
    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow(x, y, "3D Viewer", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
//    glfwSetKeyCallback(window, key_callback);

    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    // Depth Test
    glEnable(GL_DEPTH_TEST);

    // Accerpt fragment if it is closer to the camera thatn the former one
    glDepthFunc(GL_LESS);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

//    // Create and compile our GLSL program from the shaders
//    GLuint programID = LoadShaders( "../CIS563-FluidSolver/src/shaders/TransformVertexShader.glsl",
//                                    "../CIS563-FluidSolver/src/shaders/ColorFragmentShader.glsl" );

    // Create and compile our GLSL program from the shaders
    GLuint programID = LoadShaders( "../CIS563-FluidSolver/src/shaders/SimpleVertexShader.glsl",
                                    "../CIS563-FluidSolver/src/shaders/SimpleFragmentShader.glsl" );


    // Get a handle for our "cameraMat" uniform
    GLuint matrixID = glGetUniformLocation(programID, "cameraMat");

    // Initalize and create Objects
    //    Cube* unitCube = new Cube();
    //    unitCube->create();

    Triangle* unitTriangle = new Triangle();
    unitTriangle->create();

    // Initalize Camera
    Camera* camera = new Camera(x, y);

    do{

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use our shader
        glUseProgram(programID);

        // Send our transformation to the currently bound shader,
        // in the "cameraMat" uniform
        glm::mat4 x = camera->getCameraMat();
        glUniformMatrix4fv(matrixID, 1, GL_FALSE, &x[0][0]);

        // Draw Objects
//        unitCube->draw();
        unitTriangle->draw();

        glDisableVertexAttribArray(0);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );

    // Cleanup VBO
//    unitCube->destroy();
    unitTriangle->destroy();
    glDeleteVertexArrays(1, &VertexArrayID);
    glDeleteProgram(programID);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();
}


//void Viewer::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
//{
//    float DEG2RAD = M_PI / 180.0;
//    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
//        glfwSetWindowShouldClose(window, GL_TRUE);
//    }
//    else if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
////        adjustPhi(camera, 5.0f * DEG2RAD);
//    }
////    else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
////        adjustPhi(camera, -5.0f * DEG2RAD);
////    } else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
////        adjustTheta(camera, -5.0f * DEG2RAD);
////    } else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
////        adjustTheta(camera, 5.0f * DEG2RAD);
////    }
//}

//void Viewer::adjustPhi(Camera* c, float p) {
//    camera->phi += p;
//}

//void Viewer::adjustTheta(float t) {
//    camera->theta += t;
//}

