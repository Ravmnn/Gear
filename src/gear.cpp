#include <gear.hpp>

#include <iostream>

#include <compiler/token.hpp>
#include <compiler/scanner.hpp>
#include <compiler/exceptions/exception_formatting.hpp>
#include <compiler/exceptions/gear_exception.hpp>
#include <compiler/exceptions/scanner_exceptions.hpp>



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

    try
    {
        for (const Token& token : Scanner(source).scan())
            std::cout << std::boolalpha << (int)token.type << ": " << token.isKeyword() << " - " << token.lexeme << std::endl;
    }

    catch (const ScannerException& exception)
    {
        error(generateFinalExceptionMessage(exception.what(), exception.position()));
    }
    
    catch (const GearException& exception)
    {
        error(exception.what());
    }
}


void Gear::error(const std::string& message) noexcept
{
    std::cout << "Error: " << message << std::endl;
}