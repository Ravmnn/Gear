#pragma once

#include <string>



enum class ASMTypeSize;


struct Register
{
    Register(const std::string& name, ASMTypeSize size) noexcept;


    std::string name;

    ASMTypeSize size;
};