#include "Utility.hpp"

#include <fstream>
#include <sstream>

std::string loadFile(std::string s) {
    std::ifstream fs(s);
    std::stringstream ss;
    ss << fs.rdbuf();
    return ss.str();
}