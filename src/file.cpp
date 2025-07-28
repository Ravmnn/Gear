#include <file.hpp>

#include <iostream>
#include <sstream>
#include <fstream>



std::string readFile(const std::string& path)
{
    std::ifstream file(path);
    std::stringstream buffer;

    buffer << file.rdbuf();

    return buffer.str();
}