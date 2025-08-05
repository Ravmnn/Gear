#pragma once

#include <string>
#include <vector>
#include <filesystem>

#include <compiler/compiler.hpp>



class Statement;


struct TorqueOptions
{
    TorqueOptions() noexcept;


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



class Torque
{
private:
    static std::string s_name;
    static std::string s_description;

    static std::string s_source;

    static TorqueOptions s_options;


public:
    Torque() = delete;

    
    static const std::string& name() noexcept;
    static const std::string& description() noexcept;
    static const std::string& source() noexcept;
    static const std::vector<std::string> sourceAsLines() noexcept;

    static const TorqueOptions& options() noexcept;


    static void run(const TorqueOptions& options);

private:
    static std::string compile(const std::vector<const Statement*>& statements);
    static std::filesystem::path assemble(const std::filesystem::path& file) noexcept;
    static std::filesystem::path link(const std::filesystem::path& file) noexcept;

    static std::filesystem::path writeToOutputFile(const std::string& content);
};