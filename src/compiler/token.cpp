#include <compiler/token.hpp>



bool Token::isKeyword(const std::string& keyword) noexcept
{
    return keywords.count(keyword) > 0;
}


bool Token::isKeyword() const noexcept
{
    return Token::isKeyword(lexeme);
}