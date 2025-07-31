#include <compiler/compiler.hpp>

#include <compiler/assembly/nasm.hpp>



const std::string Compiler::startLabelName = "_start";



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
    startLabel();


    finish();
}


void Compiler::startLabel()
{
    _start.nasmDirectiveGlobal(startLabelName);
    _start.label(startLabelName);
    
    _start.enableIndent();


    _start.instruction("mov", "rbp", "rsp");
    _start.newline();

    // _start.instruction("call", _entryPoint);

    _start.syscallExit(0);


    _start.disableIndent();
}


void Compiler::finish()
{
    _asm.nasmDirectiveBits(_bitMode);
    _asm.newline(2);



    _asm.nasmDirectiveSection(".data");
    _asm.newline();

    _asm.insertOther(_data);

    _asm.newline(2);



    _asm.nasmDirectiveSection(".text");
    _asm.newline();

    _asm.insertOther(_start);

    _asm.newline(2);


    
    _asm.insertOther(_funcs);
}