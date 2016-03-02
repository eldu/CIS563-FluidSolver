#pragma once;

#include "../fluidSolver/fluidSolver.hpp"
#include <json/json.h>
#include <json/reader.h>
#include <fstream> // Stream class to read/write on files
#include <iostream>
#include <sstream>
#include "../geom/cube.hpp"

// Referenced
// www.cplusplus.com/doc/tutotial/files

class JSONReader {
public:
    std::string readFile(const char *filename);
    Json::Value loadJSON(const std::string &document);
    FluidSolver* parseScene(const Json::Value &root);
};
