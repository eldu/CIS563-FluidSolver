//#ifndef FS_INPUT_H
//#define FS_INPUT_H

///// http://stackoverflow.com/questions/7676971/

//// callback class for glfwSetMouseButtonCallback
//class Input
//{
//public:
//    static Input& getInstance()
//    {
//        static Input instance;
//        return instance;
//    }

//    // Callback for glfw
//    static void GLFWcallback(int key, int action)
//    {
//        //here we access the instance via the singleton pattern and forward the callback to the instance method
//        getInstance().mouseButtonCallbackImpl(key, action);
//    }

//    // Actual Callback that can access non-static things
//    void callback(int key, int action)
//    {

//    }


//private:
//    Input(void) {}
//    Input(Input const&); // prevent copies
//    void operator=(Input const&); // prevent assignments


//};

//#endif
