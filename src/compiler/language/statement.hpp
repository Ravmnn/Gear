#pragma once

#include <string>

#include <compiler/language/token.hpp>


class Expression;

class CompilerAttributeStatement;
class ExpressionStatement;
class DeclarationStatement;


class StatementProcessor
{
public:
    virtual void processCompilerAttribute(const CompilerAttributeStatement& statement) = 0;
    virtual void processExpression(const ExpressionStatement& statement) = 0;
    virtual void processDeclaration(const DeclarationStatement& statement) = 0;
};



class Statement
{
public:
    virtual void process(StatementProcessor& processor) const = 0;
};



class CompilerAttributeStatement : public Statement
{
public:
    CompilerAttributeStatement(const Token& name, const Token& value) noexcept;


    Token name;
    Token value;


    void process(StatementProcessor& processor) const override;
};



class ExpressionStatement : public Statement
{
public:
    explicit ExpressionStatement(const Expression* expression) noexcept;


    const Expression* const expression;


    void process(StatementProcessor& processor) const override;
};



class DeclarationStatement : public Statement
{
public:
    DeclarationStatement(const Token& name, const Token& type, const Expression* value) noexcept;


    Token name;
    Token type;
    const Expression* const value;


    void process(StatementProcessor& processor) const override;
};