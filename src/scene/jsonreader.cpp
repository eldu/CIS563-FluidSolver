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

    float particleSeparation = root["particleSeparation"].asFloat();

    Cube* container = new Cube(scaleX, scaleY, scaleZ);
    Cube* fluid = new Cube(boundX, boundY, boundZ);
//    fluid->min += glm::vec3(0.f, 1.f, 0.f);
//    fluid->max += glm::vec3(0.f, 1.f, 0.f);

    return new FLIPSolver(container, fluid);
}
