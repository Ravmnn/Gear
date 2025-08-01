#include <compiler/assembly/assembly_generator.hpp>

#include <compiler/assembly/nasm.hpp>



AssemblyGenerator::AssemblyGenerator()
{
    _indent = false;
}



std::string AssemblyGenerator::get() const noexcept
{
    return _stream.str();
}


bool AssemblyGenerator::indent() const noexcept
{
    return _indent;
}





void AssemblyGenerator::enableIndent() noexcept
{
    _indent = true;
}

void AssemblyGenerator::disableIndent() noexcept
{
    _indent = false;
}





void AssemblyGenerator::newline(const unsigned int amount) noexcept
{
    if (!_comment.empty())
    {
        _stream << " ; " << _comment;
        _comment.clear();
    }

    for (size_t i = 0; i < amount; i++)
        _stream << std::endl;
}

std::string AssemblyGenerator::tab() const noexcept
{
    return _indent ? "    " : "";
}





void AssemblyGenerator::insertOther(const AssemblyGenerator& other) noexcept
{
    _stream << other.get();
}



void AssemblyGenerator::nasmDirective(const std::string& directive, const std::string& value) noexcept
{
    _stream << directive << " " << value;
    newline();
}


void AssemblyGenerator::nasmDirectiveBits(const BitMode mode) noexcept
{
    nasmDirective("bits", std::to_string((int)mode));
}


void AssemblyGenerator::nasmDirectiveSection(const std::string& name) noexcept
{
    nasmDirective("section", name);
}


void AssemblyGenerator::nasmDirectiveGlobal(const std::string& identifier) noexcept
{
    nasmDirective("global", identifier);
}





void AssemblyGenerator::comment(const std::string& content) noexcept
{
    _comment = content;
}

void AssemblyGenerator::instantComment(const std::string& content) noexcept
{
    _stream << tab() << "; " << content;
    newline();
}



void AssemblyGenerator::label(const std::string& name) noexcept
{
    _stream << name << ':';
    newline();
}

void AssemblyGenerator::localLabel(const std::string& name) noexcept
{
    label("." + name);
}



void AssemblyGenerator::instruction(const std::string& instruction, const std::string& op1, const std::string& op2) noexcept
{
    _stream << tab() << instruction << " " << op1 << ", " << op2;
    newline();
}


void AssemblyGenerator::instruction(const std::string& instruction, const std::string& op1) noexcept
{
    _stream << tab() << instruction << " " << op1;
    newline();
}


void AssemblyGenerator::instruction(const std::string& instruction) noexcept
{
    _stream << tab() << instruction;
    newline();
}





void AssemblyGenerator::syscall(const std::string& code, const std::string& arg1, const std::string& arg2, const std::string& arg3) noexcept
{
    instruction("mov", "rax", code);
    
    if (!arg1.empty())
        instruction("mov", "rdi", arg1);

    if (!arg2.empty())
        instruction("mov", "rsi", arg2);

    if (!arg3.empty())
        instruction("mov", "rdx", arg3);
    
    instruction("syscall");
}


void AssemblyGenerator::syscallExit(const unsigned int exitCode) noexcept
{
    syscall("60", std::to_string(exitCode));
}