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

    programOutputName = "app";

    printAst = false;
    compile = true;
    assemble = true;
    link = true;

    commentAsm = true;
}


Compiler GearOptions::initializeCompilerFromThis(const std::vector<const Statement*>& statements) const noexcept
{
    Compiler compiler(statements, (BitMode)programBitMode, programEntryPoint);

    compiler.setShouldComment(commentAsm);

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
GearOptions Gear::s_options = GearOptions();


const std::string& Gear::source() noexcept
{
    return s_source;
}

const std::vector<std::string> Gear::sourceAsLines() noexcept
{
    return split(source(), '\n');
}


const GearOptions& Gear::options() noexcept
{
    return s_options;
}




static std::vector<const Statement*> toConstStatements(const std::vector<Statement*>& statements) noexcept
{
    std::vector<const Statement*> constStatements;

    for (const Statement* const statement : statements)
        constStatements.push_back(statement);

    return constStatements;
}


void Gear::run(const GearOptions& options)
{
    s_source = readFile(options.filePath);
    s_options = options;

    const std::vector<Token> tokens = Scanner(s_source).scan();

    if (failed())
        return;

    const std::vector<const Statement*> statements = toConstStatements(Parser(tokens).parse());

    if (failed())
        return;

    if (options.printAst)
    {
        println(ASTPrinter().print(statements));
        return;
    }

    const std::string assembly = compile(statements);

    if (failed())
        return;


    std::filesystem::path file;
    std::filesystem::path objectFile;
    std::filesystem::path executableFile;

    file = writeToOutputFile(assembly);

    if (!options.assemble)
        return;
    
    objectFile = assemble(file);

    if (!options.link)
        return;
    
    executableFile = link(objectFile);
}



std::string Gear::compile(const std::vector<const Statement*>& statements)
{
    const GearOptions& options = Gear::options();

    Compiler compiler = options.initializeCompilerFromThis(statements);
    compiler.compile();

    return compiler.assembly().get();
}


std::filesystem::path Gear::assemble(const std::filesystem::path& file) noexcept
{
    std::stringstream command;
    const std::filesystem::path outputFilePath = file.parent_path() / file.stem() += ".o";

    command << "nasm " << file.string() << " -f elf64" << " -o " << outputFilePath;

    system(command.str().c_str());

    return outputFilePath;
}


std::filesystem::path Gear::link(const std::filesystem::path& file) noexcept
{
    std::stringstream command;
    const std::filesystem::path outputFilePath = file.parent_path() / options().programOutputName;

    command << "ld " << file.string() << " -o " << outputFilePath;

    system(command.str().c_str());

    return outputFilePath;
}


std::filesystem::path Gear::writeToOutputFile(const std::string& content)
{
    const GearOptions& options = Gear::options();

    const std::filesystem::path outputFilePath = options.filePath.parent_path();
    const std::string outputFileName = options.filePath.stem() += ".asm";

    const std::filesystem::path finalPath = outputFilePath / outputFileName;

    writeFile(finalPath, content);

    return finalPath;
}