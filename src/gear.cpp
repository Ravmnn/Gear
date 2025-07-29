#include <gear.hpp>

#include <iostream>

#include <compiler/token.hpp>
#include <compiler/scanner.hpp>
#include <compiler/parser.hpp>
#include <compiler/exceptions/exception_formatting.hpp>
#include <compiler/exceptions/scanner_exception.hpp>
#include <compiler/exceptions/parser_exception.hpp>



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


void Gear::compile(const std::string& source)
{
    s_source = source;

    const std::vector<Token> tokens = Scanner(source).scan();
    const std::vector<Statement*> statements = Parser(tokens).parse();
}


void Gear::error(const std::string& message) noexcept
{
    std::cout << "Error: " << message << std::endl << std::endl;
}

void Gear::error(const ScannerException& exception) noexcept
{
    error(exception.format());
}

void Gear::error(const ParserException& exception) noexcept
{
    error(exception.format());
}