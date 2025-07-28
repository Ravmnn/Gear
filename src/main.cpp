#include <iostream>

#include <file.hpp>
#include <compiler/token.hpp>
#include <compiler/scanner.hpp>



int main(int argc, char** argv)
{
    const std::string source = readFile("examples/main.gr");

    for (const Token& token : Scanner(source).scan())
        std::cout << std::boolalpha << (int)token.type << ": " << token.isKeyword() << " - " << token.lexeme << std::endl;

    return 0;
}