#pragma once

#include <compiler/token.hpp>



class LiteralExpression;
class BinaryExpression;
class GroupingExpression;
class IdentifierExpression;


class ExpressionProcessor
{
public:
    virtual void processLiteral(const LiteralExpression& expression) = 0;
    virtual void processBinary(const BinaryExpression& expression) = 0;
    virtual void processGrouping(const GroupingExpression& expression) = 0;
    virtual void processIdentifier(const IdentifierExpression& expression) = 0;
};



class Expression
{
public:
    virtual void process(ExpressionProcessor& processor) = 0;
};



class LiteralExpression : public Expression
{
public:
    explicit LiteralExpression(const Token& value) noexcept;


    Token value;


    void process(ExpressionProcessor& processor) override;
};



class BinaryExpression : public Expression
{
public:
    BinaryExpression(const Expression* left, const Token& op, const Expression* right) noexcept;


    const Expression* const left;
    Token op;
    const Expression* const right;


    void process(ExpressionProcessor& processor) override;
};



class GroupingExpression : public Expression
{
public:
    explicit GroupingExpression(const Expression* expression) noexcept;


    const Expression* const expression;


    void process(ExpressionProcessor& processor) override;
};



class IdentifierExpression : public Expression
{
public:
    explicit IdentifierExpression(const Token& identifier) noexcept;


    Token identifier;


    void process(ExpressionProcessor& processor) override;
};