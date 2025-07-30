#include <compiler/compiler.hpp>

#include <compiler/assembly/nasm.hpp>



const std::string Compiler::startLabel = "_start";



Compiler::Compiler(const BitMode bitMode, const std::string& entryPoint) noexcept
{
    _bitMode = bitMode;
    _entryPoint = entryPoint;
}



AssemblyGenerator& Compiler::assembly() noexcept
{
    return _asm;
}


BitMode Compiler::bitMode() const noexcept
{
    return _bitMode;
}


const std::string& Compiler::entryPoint() const noexcept
{
    return _entryPoint;
}




void Compiler::setBitmode(const BitMode bitMode) noexcept
{
    _bitMode = bitMode;
}


void Compiler::setEntryPoint(const std::string& entryPoint) noexcept
{
    _entryPoint = entryPoint;
}




void Compiler::compile()
{
    setup();
}


void Compiler::setup()
{
    _asm.nasmDirectiveBits(_bitMode);
    _asm.newline(2);



    _asm.nasmDirectiveSection(".data");
    _asm.newline();

    _asm.newline(2);



    _asm.nasmDirectiveSection(".text");
    _asm.newline();

    _asm.nasmDirectiveGlobal(startLabel);
    _asm.label(startLabel);
    _asm.enableIndent();



    _asm.disableIndent();
}