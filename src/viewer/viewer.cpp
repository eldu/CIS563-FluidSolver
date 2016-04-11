//
//  viewer.cpp
//  Thanda

#include "viewer.hpp"
#include "../camera/camera.hpp"


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

    // Grey Background
    glClearColor(0.8f, 0.8f, 0.8f, 0.0f);

    // Depth Test
    glEnable(GL_DEPTH_TEST);

    // Accerpt fragment if it is closer to the camera thatn the former one
    glDepthFunc(GL_LESS);


    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // Create and compile our GLSL program from the shaders
    programID = LoadShaders( "src/shaders/TransformVertexShader.glsl",
                             "src/shaders/ColorFragmentShader.glsl");

//        // Create and compile our GLSL program from the shaders
//        GLuint programID = LoadShaders( "../src/shaders/SimpleVertexShader.glsl",
//                                        "../src/shaders/SimpleFragmentShader.glsl" );

    // Get a handle for our "cameraMat" uniform
    matrixID = glGetUniformLocation(programID, "cameraMat");

    // Fluid
    // TODO Make it so I don't have to hard code the filepath
    // Change with user input in the viewer or something;
    JSONReader* reader;
    fs = reader->parse("src/scene/scene.json");

    fs->container->create();
//    fs->fluid->create();
//    fs->mGrid.create();
    fs->create();

    // Initalize Camera
    camera = new Camera(x, y);

}

int Viewer::run() {
    double fps = 24.0f;
    double lastTime = glfwGetTime();

    do{
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;

        // Run at a certain fps
        if (deltaTime >= 1.0f / fps) {
            lastTime = currentTime; // update time

            // Clear the screen
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Use our shader
            glUseProgram(programID);

            // Send our transformation to the currently bound shader,
            // in the "cameraMat" uniform
            glm::mat4 x = camera->getCameraMat();
            glUniformMatrix4fv(matrixID, 1, GL_FALSE, &x[0][0]);

            // Draw Objects
            fs->container->draw();
//            fs->fluid->draw();
//            fs->mGrid.draw();
            fs->draw();

            // Adjust Camera
//            camera->recomputeCameraFromInputs(window);

            // Update Fluid
            fs->update(deltaTime);

            glDisableVertexAttribArray(0);

            // Swap buffers
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );

//    // Cleanup VBO
    fs->container->destroy();
//    fs->fluid->destroy();
//    fs->mGrid.destroy();
    fs->destroy();

    glDeleteVertexArrays(1, &VertexArrayID);
    glDeleteProgram(programID);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();
}


/*
 * Call back function works with print statements, however, because camera is a not static
 * and this function has to be static, it really hates me at the moment
 * Austin suggested creating a static object to handle all fo the adjustments
*/
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

