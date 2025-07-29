#pragma once

#include <string>
#include <vector>



class Gear
{
private:
    static std::string s_source;


public:
    Gear() = delete;


    static const std::string& source() noexcept;
    static const std::vector<std::string> sourceAsLines() noexcept;


    static void compile(const std::string& source);


    static void error(const std::string& message) noexcept;
};