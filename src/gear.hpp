#pragma once

#include <string>



class Gear
{
private:
    static std::string s_source;


public:
    static const std::string& source() noexcept;


    static void compile(const std::string& source);
};