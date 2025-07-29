#pragma once

#include <compiler/assembly/assembly_generator.hpp>



class Compiler
{
private:
    AssemblyGenerator _asm;

    BitMode _bitmode;
    std::string _entrypoint;


public:
    static const std::string startLabel;


    Compiler() noexcept;


    AssemblyGenerator& assembly() noexcept;

    BitMode bitmode() const noexcept;
    const std::string& entrypoint() const noexcept;


    void setBitmode(BitMode bitmode) noexcept;
    void setEntrypoint(const std::string& entrypoint) noexcept;


    void compile();

    void setup();
};