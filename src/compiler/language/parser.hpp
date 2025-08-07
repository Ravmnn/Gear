#pragma once

#include <vector>

#include <compiler/language/token.hpp>



class Statement;
class Expression;
class ParserException;
struct FunctionParameterDeclaration;


class Parser
{
private:
    std::vector<Statement*> _statements;
    std::vector<Token> _tokens;
    unsigned int _current;


public:
    explicit Parser(const std::vector<Token>& tokens) noexcept;


    std::vector<Statement*> parse();


private:
    Statement* declaration();
    
    Statement* variableDeclaration(const Token& type, const Token& identifier);

    Statement* functionDeclaration(const Token& returnType, const Token& identifier);
    std::vector<FunctionParameterDeclaration> functionParameters();


    Statement* statement();
    Statement* expressionStatement();
    Statement* returnStatement();

    Statement* block();


    Expression* expression();
    Expression* term();
    Expression* factor();
    Expression* primary();

    Expression* parseGroupExpression();


    void synchronize() noexcept;



    void reset() noexcept;


    const Token& expect(TokenType token, const ParserException& exception);
    const Token& expectEndOfStatement();

    const Token& expectIdentifier();
    const Token& expectTypename();


    bool match(const std::vector<TokenType>& tokens) noexcept;
    bool check(TokenType token) const noexcept;

    const Token& advance() noexcept;
    const Token& peek() const noexcept;
    const Token& previous() const noexcept;
    bool atEnd() const noexcept;
};