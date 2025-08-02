#pragma once

#include <compiler/language/token.hpp>



enum class ASMTypeSize;


struct Identifier
{
    const std::string name;
    const ASMTypeSize size;

    const TokenPosition position;
};