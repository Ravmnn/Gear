#pragma once

#include <string>

#include <compiler/language/token.hpp>


class Expression;

class ExpressionStatement;
class DeclarationStatement;
class FunctionDeclarationStatement;
class ReturnStatement;
class BlockStatement;


class StatementProcessor
{
public:
    virtual void processExpression(const ExpressionStatement& statement) = 0;
    virtual void processDeclaration(const DeclarationStatement& statement) = 0;
    virtual void processFunctionDeclaration(const FunctionDeclarationStatement& statement) = 0;
    virtual void processReturn(const ReturnStatement& statement) = 0;
    virtual void processBlock(const BlockStatement& statement) = 0;
};



class Statement
{
public:
    virtual void process(StatementProcessor& processor) const = 0;
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


    const Token name;
    const Token type;
    const Expression* const value;


    void process(StatementProcessor& processor) const override;
};



struct FunctionParameterDeclaration
{
    const Token name;
    const Token type;
};


class FunctionDeclarationStatement : public Statement
{
public:
    FunctionDeclarationStatement(const Token& name, const std::vector<FunctionParameterDeclaration>& parameters,
        const Token& returnType, const BlockStatement* body) noexcept;

    
    const Token name;
    const std::vector<FunctionParameterDeclaration> parameters;
    const Token returnType;
    const BlockStatement* const body;


    void process(StatementProcessor& processor) const override;
};



class ReturnStatement : public Statement
{
public:
    ReturnStatement(const Expression* expression) noexcept;


    const Expression* const expression;


    void process(StatementProcessor& processor) const override;
};



class BlockStatement : public Statement
{
public:
    BlockStatement(const std::vector<Statement*>& statements) noexcept;


    const std::vector<Statement*> statements;


    void process(StatementProcessor& processor) const override;
};