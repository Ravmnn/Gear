#include <compiler/parser.hpp>

#include <compiler/statement.hpp>
#include <compiler/expression.hpp>
#include <compiler/exceptions/exceptions.hpp>
#include <gear.hpp>



Parser::Parser(const std::vector<Token>& tokens) noexcept
    : _tokens(tokens), _current(0)
{}



std::vector<Statement*> Parser::parse()
{
    reset();

    while (!atEnd())
    {
        try
        {
            _statements.push_back(declaration());
        }
        catch (const ParserException& exception)
        {
            Gear::error(exception);
            synchronize();
        }
    }

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
    advance();

    const Token name = expect(TokenType::Identifier, gear_e2001(previous().position));
    expect(TokenType::Colon, gear_e2002(previous().position));
    const Token type = expect(TokenType::Type, gear_e2003(previous().position));
    expect(TokenType::Equal, gear_e2004(previous().position));
    
    const Expression* const value = expression();

    expectEndOfStatement();

    return new DeclarationStatement(name, type, value);
}



Statement* Parser::statement()
{
    switch (peek().type)
    {
    default:
        return expressionStatement();
    }
}


Statement* Parser::expressionStatement()
{
    Expression* const expression = this->expression();
    expectEndOfStatement();

    return new ExpressionStatement(expression);
}



Expression* Parser::expression()
{
    return term();
}


Expression* Parser::term()
{
    Expression* expression = factor();

    while (match({ TokenType::Plus, TokenType::Minus }))
    {
        const Token op = previous();
        Expression* const right = factor();
        expression = new BinaryExpression(expression, op, right);
    }

    return expression;
}


Expression* Parser::factor()
{
    Expression* expression = primary();

    while (match({ TokenType::Star, TokenType::Slash }))
    {
        const Token op = previous();
        Expression* const right = primary();
        expression = new BinaryExpression(expression, op, right);
    }

    return expression;
}


Expression* Parser::primary()
{
    if (match({ TokenType::Value }))
        return new LiteralExpression(previous());

    if (match({ TokenType::Identifier }))
        return new IdentifierExpression(previous());

    if (match({ TokenType::ParenLeft }))
        return parseGroupExpression();

    throw gear_e2006(atEnd() ? previous().position : peek().position);
}


Expression* Parser::parseGroupExpression()
{
    const Token leftParen = previous();
    const Expression* const expression = this->expression();

    expect(TokenType::ParenRight, gear_e2006(leftParen.position));

    return new GroupingExpression(expression);
}



void Parser::synchronize() noexcept
{
    advance();

    while (!atEnd())
    {
        switch (peek().type)
        {
        case TokenType::KwDeclare:
        case TokenType::KwFunction:
        case TokenType::KwReturn:
        case TokenType::KwStart:
        case TokenType::KwEnd:
            return;
        }
    }
}



void Parser::reset() noexcept
{
    _statements.clear();
    _current = 0;
}



const Token& Parser::expect(const TokenType token, const ParserException& exception)
{
    if (check(token))
        return advance();

    throw exception;
}


const Token& Parser::expectEndOfStatement()
{
    return expect(TokenType::SemiColon, gear_e2000(previous().position));
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
    return !atEnd() ? _tokens[_current] : previous();
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