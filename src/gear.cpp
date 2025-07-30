#include <gear.hpp>

#include <iostream>
#include <filesystem>

#include <log.hpp>
#include <file.hpp>
#include <compiler/assembly/nasm.hpp>
#include <compiler/language/token.hpp>
#include <compiler/language/scanner.hpp>
#include <compiler/language/parser.hpp>
#include <compiler/language/ast_printer.hpp>



GearOptions::GearOptions() noexcept
{
    // default values

    programEntryPoint = "main";
    programBitMode = (unsigned int)BitMode::Bits64;

    printAst = false;
}


Compiler GearOptions::initializeCompilerFromThis() const noexcept
{
    Compiler compiler((BitMode)programBitMode, programEntryPoint);


    return compiler;
}



static std::vector<std::string> split(const std::string& source, const char delimiter) noexcept
{
    std::vector<std::string> splited;
    std::string buffer;

    for (const char ch : source)
    {
        if (ch == delimiter)
        {
            splited.push_back(buffer);
            buffer = "";
            continue;
        }

        buffer += ch;
    }

    if (!buffer.empty())
        splited.push_back(buffer);

    return splited;
}



std::string Gear::s_source = std::string();


const std::string& Gear::source() noexcept
{
    return s_source;
}


const std::vector<std::string> Gear::sourceAsLines() noexcept
{
    return split(source(), '\n');
}




void Gear::run(const GearOptions& options)
{
    s_source = readFile(options.filePath);

    const std::vector<Token> tokens = Scanner(s_source).scan();
    const std::vector<Statement*> statements = Parser(tokens).parse();

    if (failed())
        return;

    if (options.printAst)
    {
        println(ASTPrinter().print(statements));
        return;
    }


    Compiler compiler = options.initializeCompilerFromThis();
}