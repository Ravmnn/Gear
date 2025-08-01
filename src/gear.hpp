#pragma once

#include <string>
#include <vector>
#include <filesystem>

#include <compiler/compiler.hpp>



class Statement;


struct GearOptions
{
    GearOptions() noexcept;


    std::filesystem::path filePath;

    std::string programEntryPoint;
    unsigned int programBitMode;

    std::string programOutputName;

    bool printAst;
    bool compile;
    bool assemble;
    bool link;

    bool commentAsm;


    Compiler initializeCompilerFromThis(const std::vector<const Statement*>& statements) const noexcept;
};



class Gear
{
private:
    static std::string s_source;

    static GearOptions s_options; 


public:
    Gear() = delete;


    static const std::string& source() noexcept;
    static const std::vector<std::string> sourceAsLines() noexcept;

    static const GearOptions& options() noexcept;


    static void run(const GearOptions& options);

private:
    static std::string compile(const std::vector<const Statement*>& statements);
    static std::filesystem::path assemble(const std::filesystem::path& file) noexcept;
    static std::filesystem::path link(const std::filesystem::path& file) noexcept;

    static std::filesystem::path writeToOutputFile(const std::string& content);
};