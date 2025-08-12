#include <compiler/language/parser.hpp>

#include <compiler/language/statement.hpp>
#include <compiler/language/expression.hpp>
#include <compiler/exceptions/exceptions.hpp>
#include <log.hpp>
#include <torque.hpp>



Parser::Parser(const std::vector<Token>& tokens) noexcept
    : _tokens(tokens), _current(0)
{}


std::vector<const Statement*> Parser::parse()
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



const Statement* Parser::declaration()
{
    switch (peek().type)
    {
    case TokenType::Type: {
        const Token type = expectTypename();
        const Token name = expectIdentifier();

        if (check(TokenType::ParenLeft))
            return functionDeclaration(type, name);
        else
            return variableDeclaration(type, name);

        break;
    }

    default:
        return statement();
    };
}





const Statement* Parser::variableDeclaration(const Token& type, const Token& name)
{
    expect(TokenType::Equal, torque_e2004(peek().position));

    const Expression* const value = expression();

    expectEndOfStatement();

    return new DeclarationStatement(name, type, value);
}





const Statement* Parser::functionDeclaration(const Token& returnType, const Token& name)
{
    std::vector<FunctionParameterDeclaration> parameters;

    expect(TokenType::ParenLeft, torque_e2007(peek().position));
    parameters = functionParameters();
    expect(TokenType::ParenRight, torque_e2008(peek().position));

    const BlockStatement* body = dynamic_cast<const BlockStatement*>(block());

    return new FunctionDeclarationStatement(name, returnType, parameters, body);
}


std::vector<FunctionParameterDeclaration> Parser::functionParameters()
{
    std::vector<FunctionParameterDeclaration> parameters;

    if (check(TokenType::Identifier))
        do
        {
            const Token name = expectIdentifier();
            expect(TokenType::Colon, torque_e2002(peek().position));
            const Token type = expectTypename();

            parameters.push_back(FunctionParameterDeclaration{ name, type });
        }
        while (match({ TokenType::Comma }));

    return parameters;
}





const Statement* Parser::statement()
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

        throw torque_e2012(peek().position);


    default:
        return expressionStatement();
    }
}


const Statement* Parser::expressionStatement()
{
    const Expression* const expression = this->expression();
    expectEndOfStatement();

    return new ExpressionStatement(expression);
}


const Statement* Parser::returnStatement()
{
    const Token& keyword = advance();

    const Expression* const expression = this->expression();
    expectEndOfStatement();

    return new ReturnStatement(keyword, expression);
}



const Statement* Parser::block()
{
    std::vector<const Statement*> block;

    const Token& start = expect(TokenType::KwStart, torque_e2010(peek().position));

    while (!atEnd() && !check(TokenType::KwEnd))
        block.push_back(declaration());

    const Token& end = expect(TokenType::KwEnd, torque_e2011(peek().position));

    return new BlockStatement(start, end, block);
}



const Expression* Parser::expression()
{
    return term();
}


const Expression* Parser::term()
{
    const Expression* expression = factor();

    while (match({ TokenType::Plus, TokenType::Minus }))
    {
        const Token op = previous();
        const Expression* const right = factor();
        expression = new BinaryExpression(expression, op, right);
    }

    return expression;
}


const Expression* Parser::factor()
{
    const Expression* expression = cast();

    while (match({ TokenType::Star, TokenType::Slash }))
    {
        const Token op = previous();
        const Expression* const right = cast();
        expression = new BinaryExpression(expression, op, right);
    }

    return expression;
}


const Expression* Parser::cast()
{
    const Expression* expression = call();

    while (match({ TokenType::KwAs }))
    {
        const Token keyword = previous();
        const Token type = expectTypename();
        expression = new CastExpression(keyword, expression, type);
    }

    return expression;
}


const Expression* Parser::call()
{
    const Expression* expression = primary();

    while (match({ TokenType::ParenLeft }))
    {  
        const Token parenLeft = previous();
        std::vector<const Expression*> arguments;
        
        if (!check(TokenType::ParenRight))
            arguments = this->arguments();

        expect(TokenType::ParenRight, torque_e2013(peek().position));

        expression = new CallExpression(parenLeft, expression, arguments);
    }

    return expression;
}


std::vector<const Expression*> Parser::arguments()
{
    std::vector<const Expression*> expressions;

    do
        expressions.push_back(expression());
    while (match({ TokenType::Comma }));
    

    return expressions;
}


const Expression* Parser::primary()
{
    if (match({ TokenType::Value }))
        return new LiteralExpression(previous());

    if (match({ TokenType::Identifier }))
        return new IdentifierExpression(previous());

    if (match({ TokenType::ParenLeft }))
        return parseGroupExpression();

    throw torque_e2006(peek().position);
}


const Expression* Parser::parseGroupExpression()
{
    const Token leftParen = previous();
    const Expression* const expression = this->expression();

    expect(TokenType::ParenRight, torque_e2006(leftParen.position));

    return new GroupingExpression(expression);
}



void Parser::synchronize() noexcept
{
    advance();

    while (!atEnd())
    {
        switch (peek().type)
        {
        case TokenType::Type:
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
    return expect(TokenType::SemiColon, torque_e2000(peek().position));
}



const Token& Parser::expectIdentifier()
{
    return expect(TokenType::Identifier, torque_e2001(peek().position));
}

const Token& Parser::expectTypename()
{
    return expect(TokenType::Type, torque_e2003(peek().position));
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