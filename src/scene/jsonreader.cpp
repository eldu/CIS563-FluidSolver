#include "jsonreader.hpp"

// Referenced cplusplus.com
// Referenced the json.h and reader.h files in nuparu
// Referenced jsoncpp.sourceforge.net

JSONReader::JSONReader() {}
JSONReader::~JSONReader() {}


FLIPSolver* JSONReader::parse(const char *filename) {
    std:string doc = readFile(filename);
    Json::Value root = loadJSON(doc);
    return parseJSON(root);
}

std::string JSONReader::readFile(const char *filename) {
    std::string result;
    std::ifstream jsonfile(filename);

    std::stringstream strStream;
    strStream << jsonfile.rdbuf();
    result = strStream.str();

    strStream.clear();
    return result;
}

Json::Value JSONReader::loadJSON(const std::string &document) {
    Json::Reader reader;
    Json::Value root;

    bool result = reader.parse(document.c_str(), root, false);
    if (result) {
        return root;
    } else {
        std::cout << "Failed to load the JSONFile as a JSONValue";
    }

    return root;
}

FLIPSolver* JSONReader::parseJSON(const Json::Value &root) {
    Json::Value containerDim = root["containerDim"];
    float scaleX = containerDim["scaleX"].asFloat();
    float scaleY = containerDim["scaleY"].asFloat();
    float scaleZ = containerDim["scaleZ"].asFloat();

    Json::Value particleDim = root["particleDim"];
    float boundX = particleDim["boundX"].asFloat();
    float boundY = particleDim["boundY"].asFloat();
    float boundZ = particleDim["boundZ"].asFloat();
    
    Json::Value particleMin = root["particleMin"];
    float minX = particleMin["minX"].asFloat();
    float minY = particleMin["minY"].asFloat();
    float minZ = particleMin["minZ"].asFloat();
    
    Json::Value resolution = root["resolution"];
    int resx = resolution["resx"].asInt();
    int resy = resolution["resy"].asInt();
    int resz = resolution["resz"].asInt();

    float particleSeparation = root["particleSeparation"].asFloat();

    Cube* container = new Cube(scaleX, scaleY, scaleZ);
    Cube* fluid = new Cube(boundX, boundY, boundZ, minX, minY, minZ);

    return new FLIPSolver(container, fluid, resx, resy, resz);
}
