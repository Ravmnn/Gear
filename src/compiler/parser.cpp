#include <compiler/parser.hpp>



Parser::Parser(const std::vector<Token>& tokens) noexcept
    : _tokens(tokens), _current(0)
{}



std::vector<Statement*> Parser::parse()
{
    reset();

    while (!atEnd())
        _statements.push_back(declaration());

    return _statements;
}



Statement* Parser::declaration()
{
    switch (peek().type)
    {
    case TokenType::KwDeclare: return variableDeclaration();

    default:
        return statement();
    };
}


Statement* Parser::variableDeclaration()
{
    
}



Statement* Parser::statement()
{

}



void Parser::reset() noexcept
{
    _statements.clear();
    _tokens.clear();
    _current = 0;
}



bool Parser::match(const std::vector<TokenType>& tokens) noexcept
{
    for (const TokenType token : tokens)
    {
        if (!check(token))
            continue;

        advance();
        return true;
    }

    return false;
}


bool Parser::check(const TokenType token) const noexcept
{
    if (atEnd())
        return false;

    return peek().type == token;
}


const Token& Parser::advance() noexcept
{
    if (atEnd())
        return previous();

    return _tokens[_current++];
}


const Token& Parser::peek() const noexcept
{
    return _tokens[_current];
}


const Token& Parser::previous() const noexcept
{
    if (_current <= 0)
        return peek();

    return _tokens[_current - 1];
}


bool Parser::atEnd() const noexcept
{
    return _current >= _tokens.size();
}