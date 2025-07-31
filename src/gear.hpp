#pragma once

#include <string>
#include <vector>
#include <filesystem>

#include <compiler/compiler.hpp>



struct GearOptions
{
    GearOptions() noexcept;


    std::filesystem::path filePath;

    std::string programEntryPoint;
    unsigned int programBitMode;

    bool printAst;


    Compiler initializeCompilerFromThis() const noexcept;
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
};