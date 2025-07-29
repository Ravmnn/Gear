#include <compiler/assembly/assembly_generator.hpp>

#include <compiler/assembly/nasm.hpp>



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



void AssemblyGenerator::newline(unsigned int amount) noexcept
{
    for (size_t i = 0; i < amount; i++)
        _stream << std::endl;
}

std::string AssemblyGenerator::tab() const noexcept
{
    return _indent ? "\t" : "";
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



void AssemblyGenerator::label(const std::string& name) noexcept
{
    _stream << name << ':';
    newline();
}

void AssemblyGenerator::localLabel(const std::string& name) noexcept
{
    label("." + name);
}