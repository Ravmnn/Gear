#include <compiler/token.hpp>



bool Token::isKeyword(const std::string& keyword) noexcept
{
    return keywords.count(keyword) > 0;
}


bool Token::isBoolean(const std::string& boolean) noexcept
{
    return boolean == "true" || boolean == "false";
}


bool Token::stringToBoolean(const std::string& boolean) noexcept
{
    return boolean == "true" ? true : false;
}




bool Token::isKeyword() const noexcept
{
    return Token::isKeyword(lexeme);
}


bool Token::isBoolean() const noexcept
{
    return Token::isBoolean(lexeme);
}