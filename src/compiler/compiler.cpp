#include <compiler/compiler.hpp>

#include <compiler/assembly/nasm.hpp>



const std::string Compiler::startLabel = "_start";



Compiler::Compiler() noexcept
{
    _bitmode = BitMode::Bits64;
}



AssemblyGenerator& Compiler::assembly() noexcept
{
    return _asm;
}


BitMode Compiler::bitmode() const noexcept
{
    return _bitmode;
}


const std::string& Compiler::entrypoint() const noexcept
{
    return _entrypoint;
}




void Compiler::setBitmode(const BitMode bitmode) noexcept
{
    _bitmode = bitmode;
}


void Compiler::setEntrypoint(const std::string& entrypoint) noexcept
{
    _entrypoint = entrypoint;
}




void Compiler::compile()
{
    setup();

    // TODO: resolve attributes first (maybe create a pre-compiling stage)
}


void Compiler::setup()
{
    _asm.nasmDirectiveBits(_bitmode);
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