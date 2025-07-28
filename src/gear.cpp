#include <gear.hpp>

#include <iostream>

#include <compiler/token.hpp>
#include <compiler/scanner.hpp>



const std::string& Gear::source() noexcept
{
    return s_source;
}


void Gear::compile(const std::string& source)
{
    s_source = source;

    for (const Token& token : Scanner(source).scan())
        std::cout << std::boolalpha << (int)token.type << ": " << token.isKeyword() << " - " << token.lexeme << std::endl;
}