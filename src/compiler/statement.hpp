#pragma once

#include <string>

#include <compiler/token.hpp>



class StatementProcessor
{
public:
    virtual void processDeclaration(DeclarationStatement& statement) = 0;
};



class Statement
{
public:
    virtual void process(StatementProcessor& processor) = 0;
};



class DeclarationStatement : public Statement
{
public:
    DeclarationStatement(const Token& name, const Token& type, const Token& value) noexcept;


    Token name;
    Token type;
    Token value;


    void process(StatementProcessor& processor) override;
};