#pragma once

#include <string>



enum class ASMTypeSize;


struct Register
{
    std::string name;
    ASMTypeSize size;
};