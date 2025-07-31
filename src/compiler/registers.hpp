#pragma once

#include <string>



enum class ASMTypeSize;


struct Register
{
    Register(const std::string& name, ASMTypeSize size) noexcept;


    const std::string name;

    const ASMTypeSize size;
    // bool busy;


    // void occupy() noexcept;
    // void free() noexcept;
};