#pragma once

#include <sstream>



enum class BitMode
{
    Bits16 = 16,
    Bits32 = 32,
    Bits64 = 64
};


class AssemblyGenerator
{
private:
    std::stringstream _stream;
    bool _indent;

    std::string _comment;


public:
    AssemblyGenerator();


    std::string get() const noexcept;

    bool indent() const noexcept;


    void enableIndent() noexcept;
    void disableIndent() noexcept;
    

    void newline(unsigned int amount = 1) noexcept;
    std::string tab() const noexcept;

    
    void insertOther(const AssemblyGenerator& other) noexcept;

    void nasmDirective(const std::string& directive, const std::string& value) noexcept;

    void nasmDirectiveBits(BitMode mode) noexcept;
    void nasmDirectiveSection(const std::string& name) noexcept;
    void nasmDirectiveGlobal(const std::string& identifier) noexcept;


    void comment(const std::string& content) noexcept;
    void instantComment(const std::string& content) noexcept;

    void label(const std::string& name) noexcept;
    void localLabel(const std::string& name) noexcept;

    void instruction(const std::string& instruction, const std::string& op1, const std::string& op2) noexcept;
    void instruction(const std::string& instruction, const std::string& op1) noexcept;
    void instruction(const std::string& instruction) noexcept;

    void clearRegister(const std::string& reg) noexcept;


    void syscall(const std::string& code, const std::string& arg1 = "", const std::string& arg2 = "", const std::string& arg3 = "") noexcept;
    void syscallExit(const std::string& exitCode) noexcept;


    static std::string addressing(const std::string& reg, int displacement = 0) noexcept;
};