#ifndef FS_JSONREADER_H
#define FS_JSONREADER_H

#include "../fluidSolver/fluidSolver.hpp"
#include "../fluidSolver/flipSolver.hpp"
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
    FLIPSolver* parse(const char *filename);

    // Helperfunctions of parse
    std::string readFile(const char *filename);
    Json::Value loadJSON(const std::string &document);
    FLIPSolver* parseJSON(const Json::Value &root);
};

#endif
