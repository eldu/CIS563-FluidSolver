#ifndef FS_JSONREADER_H
#define FS_JSONREADER_H

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
    JSONReader();
    ~JSONReader();

    // Reads, loads, and parses a Fluid Solver
    FluidSolver* parse(const char *filename);

    // Helperfunctions of parse
    std::string readFile(const char *filename);
    Json::Value loadJSON(const std::string &document);
    FluidSolver* parseJSON(const Json::Value &root);
};

#endif
