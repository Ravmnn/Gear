#include <compiler/language/parser.hpp>

#include <compiler/language/statement.hpp>
#include <compiler/language/expression.hpp>
#include <compiler/exceptions/exceptions.hpp>
#include <log.hpp>
#include <gear.hpp>


// TODO: add exception: functions must be declared in file scope.


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
            error(&exception);
            synchronize();
        }
    }

    return _statements;
}



Statement* Parser::declaration()
{
    switch (peek().type)
    {
    case TokenType::KwFunction: return functionDeclaration();
    case TokenType::KwDeclare: return variableDeclaration();

    default:
        return statement();
    };
}





Statement* Parser::variableDeclaration()
{
    const Token& keyword = advance();

    const Token name = expectIdentifier();
    expect(TokenType::Colon, gear_e2002(peek().position));
    const Token type = expectTypename();
    expect(TokenType::Equal, gear_e2004(peek().position));
    
    const Expression* const value = expression();

    expectEndOfStatement();

    return new DeclarationStatement(keyword, name, type, value);
}





Statement* Parser::functionDeclaration()
{
    const Token& keyword = advance();

    const Token name = expectIdentifier();
    std::vector<FunctionParameterDeclaration> parameters;

    expect(TokenType::ParenLeft, gear_e2007(peek().position));
    parameters = functionParameters();
    expect(TokenType::ParenRight, gear_e2008(peek().position));

    expect(TokenType::Arrow, gear_e2009(peek().position));
    const Token returnType = expect(TokenType::Type, gear_e2003(peek().position));

    const BlockStatement* body = dynamic_cast<BlockStatement*>(block());

    return new FunctionDeclarationStatement(keyword, name, parameters, returnType, body);
}


std::vector<FunctionParameterDeclaration> Parser::functionParameters()
{
    std::vector<FunctionParameterDeclaration> parameters;

    if (check(TokenType::Identifier))
        do
        {
            const Token name = expectIdentifier();
            expect(TokenType::Colon, gear_e2002(peek().position));
            const Token type = expectTypename();

            parameters.push_back(FunctionParameterDeclaration{ name, type });
        }
        while (match({ TokenType::Comma }));

    return parameters;
}





Statement* Parser::statement()
{
    switch (peek().type)
    {
    case TokenType::KwStart: return block();
    case TokenType::KwReturn: return returnStatement();

    // some tokens only make sense when together with another,
    // but parser exceptions may break that "together", leaving those
    // tokens without any processing. To avoid unnecessary error messages,
    // some tokens should be ignored:

    case TokenType::KwEnd:
        advance();

        if (failed()) // something already went wrong, ignore
            return nullptr;

        throw gear_e2012(peek().position);


    default:
        return expressionStatement();
    }
}


Statement* Parser::expressionStatement()
{
    const Expression* const expression = this->expression();
    expectEndOfStatement();

    return new ExpressionStatement(expression);
}


Statement* Parser::returnStatement()
{
    const Token& keyword = advance();

    const Expression* const expression = this->expression();
    expectEndOfStatement();

    return new ReturnStatement(keyword, expression);
}



Statement* Parser::block()
{
    std::vector<Statement*> block;

    const Token& start = expect(TokenType::KwStart, gear_e2010(peek().position));

    while (!atEnd() && !check(TokenType::KwEnd))
        block.push_back(declaration());

    const Token& end = expect(TokenType::KwEnd, gear_e2011(peek().position));

    return new BlockStatement(start, end, block);
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

    throw gear_e2006(peek().position);
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
            return;
        }

        advance();
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
    return expect(TokenType::SemiColon, gear_e2000(peek().position));
}



const Token& Parser::expectIdentifier()
{
    return expect(TokenType::Identifier, gear_e2001(peek().position));
}

const Token& Parser::expectTypename()
{
    return expect(TokenType::Type, gear_e2003(peek().position));
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