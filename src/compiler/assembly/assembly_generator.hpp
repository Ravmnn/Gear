#pragma once

#include <sstream>



enum class BitMode;


class AssemblyGenerator
{
private:
    std::stringstream _stream;
    bool _indent;


public:
    AssemblyGenerator() = default;


    std::string get() const noexcept;

    bool indent() const noexcept;


    void enableIndent() noexcept;
    void disableIndent() noexcept;
    

    void newline(unsigned int amount = 1) noexcept;
    std::string tab() const noexcept;


    void nasmDirective(const std::string& directive, const std::string& value) noexcept;

    void nasmDirectiveBits(BitMode mode) noexcept;
    void nasmDirectiveSection(const std::string& name) noexcept;
    void nasmDirectiveGlobal(const std::string& identifier) noexcept;


    void label(const std::string& name) noexcept;
    void localLabel(const std::string& name) noexcept;
};