#pragma once

#include <compiler/assembly/assembly_generator.hpp>



class Compiler
{
private:
    AssemblyGenerator _asm;

    AssemblyGenerator _data;
    AssemblyGenerator _start;
    AssemblyGenerator _funcs;


    BitMode _bitMode;
    std::string _entryPoint;


public:
    static const std::string startLabelName;


    Compiler(BitMode bitMode, const std::string& entryPoint) noexcept;


    AssemblyGenerator& assembly() noexcept;

    BitMode bitMode() const noexcept;
    const std::string& entryPoint() const noexcept;


    void setBitmode(BitMode bitMode) noexcept;
    void setEntryPoint(const std::string& entryPoint) noexcept;


    void compile();


private:
    void startLabel();

    void finish();


    void syscall(unsigned int code, const std::string& arg1, const std::string& arg2, const std::string& arg3);
};