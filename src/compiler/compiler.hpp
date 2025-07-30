#pragma once

#include <compiler/assembly/assembly_generator.hpp>



class Compiler
{
private:
    AssemblyGenerator _asm;

    BitMode _bitMode;
    std::string _entryPoint;


public:
    static const std::string startLabel;


    Compiler(BitMode bitMode, const std::string& entryPoint) noexcept;


    AssemblyGenerator& assembly() noexcept;

    BitMode bitMode() const noexcept;
    const std::string& entryPoint() const noexcept;


    void setBitmode(BitMode bitMode) noexcept;
    void setEntryPoint(const std::string& entryPoint) noexcept;


    void compile();

    void setup();
};