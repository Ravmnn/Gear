#pragma once

#include <vector>

#include <compiler/token.hpp>



class Statement;
class ParserException;


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
    Statement* variableDeclaration();

    Statement* statement();




    void reset() noexcept;


    const Token& expect(TokenType token, const ParserException& exception);
    const Token& expectEndOfStatement();

    bool match(const std::vector<TokenType>& tokens) noexcept;
    bool check(TokenType token) const noexcept;

    const Token& advance() noexcept;
    const Token& peek() const noexcept;
    const Token& previous() const noexcept;
    bool atEnd() const noexcept;
};