#pragma once

#include <compiler/language/token.hpp>



enum class TypeSize;


class LiteralExpression;
class BinaryExpression;
class GroupingExpression;
class IdentifierExpression;
class CallExpression;
class CastExpression;


class ExpressionProcessor
{
public:
    virtual void processLiteral(const LiteralExpression& expression) = 0;
    virtual void processBinary(const BinaryExpression& expression) = 0;
    virtual void processGrouping(const GroupingExpression& expression) = 0;
    virtual void processIdentifier(const IdentifierExpression& expression) = 0;
    virtual void processCall(const CallExpression& expression) = 0;
    virtual void processCast(const CastExpression& expression) = 0;
};



class Expression
{
public:
    virtual void process(ExpressionProcessor& processor) const = 0;
    
    
    virtual const Token& source() const noexcept = 0;

    virtual TypeSize resultType() const noexcept = 0;
};



class LiteralExpression : public Expression
{
public:
    explicit LiteralExpression(const Token& value) noexcept;


    const Token value;


    void process(ExpressionProcessor& processor) const override;
    

    const Token& source() const noexcept override;

    TypeSize resultType() const noexcept override;
};



class BinaryExpression : public Expression
{
public:
    BinaryExpression(const Expression* left, const Token& op, const Expression* right) noexcept;


    const Expression* const left;
    const Token op;
    const Expression* const right;


    void process(ExpressionProcessor& processor) const override;


    const Token& source() const noexcept override;

    TypeSize resultType() const noexcept override;
};



class GroupingExpression : public Expression
{
public:
    explicit GroupingExpression(const Expression* expression) noexcept;


    const Expression* const expression;


    void process(ExpressionProcessor& processor) const override;


    const Token& source() const noexcept override;

    TypeSize resultType() const noexcept override;
};



class IdentifierExpression : public Expression
{
public:
    explicit IdentifierExpression(const Token& identifier) noexcept;


    const Token identifier;


    void process(ExpressionProcessor& processor) const override;


    const Token& source() const noexcept override;

    TypeSize resultType() const noexcept override;
};



class CallExpression : public Expression
{
public:
    CallExpression(const Token& parenLeft, const Expression* callee, const std::vector<const Expression*>& arguments) noexcept;


    const Token parenLeft;

    const Expression* const callee;
    const std::vector<const Expression*> arguments;


    void process(ExpressionProcessor& processor) const override;


    const Token& source() const noexcept override;

    TypeSize resultType() const noexcept override;
};



class CastExpression : public Expression
{
public:
    CastExpression(const Token& keyword, const Expression* expression, const Token& type) noexcept;


    const Token keyword;

    const Expression* const expression;
    const Token type;


    void process(ExpressionProcessor& processor) const override;


    const Token& source() const noexcept override;

    TypeSize resultType() const noexcept override;
};