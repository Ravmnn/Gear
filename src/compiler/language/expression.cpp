#include <compiler/language/expression.hpp>

#include <torque.hpp>
#include <compiler/sizes.hpp>
#include <compiler/type_checker.hpp>



LiteralExpression::LiteralExpression(const Token& value) noexcept
    : value(value)
{}


void LiteralExpression::process(ExpressionProcessor& processor) const
{
    processor.processLiteral(*this);
}



const Token& LiteralExpression::source() const noexcept
{
    return value;
}


TypeSize LiteralExpression::resultType() const noexcept
{
    return Torque::sizes()->defaultTypeSize();
}





BinaryExpression::BinaryExpression(const Expression* const left, const Token& op, const Expression* const right) noexcept
    : left(left), op(op), right(right)
{}


void BinaryExpression::process(ExpressionProcessor& processor) const
{
    processor.processBinary(*this);
}



const Token& BinaryExpression::source() const noexcept
{
    return op;
}


TypeSize BinaryExpression::resultType() const noexcept
{
    return left->resultType();
}





GroupingExpression::GroupingExpression(const Expression* const expression) noexcept
    : expression(expression)
{}


void GroupingExpression::process(ExpressionProcessor& processor) const
{
    processor.processGrouping(*this);
}



const Token& GroupingExpression::source() const noexcept
{
    return expression->source();
}


TypeSize GroupingExpression::resultType() const noexcept
{
    return expression->resultType();
}





IdentifierExpression::IdentifierExpression(const Token& identifier) noexcept
    : identifier(identifier)
{}


void IdentifierExpression::process(ExpressionProcessor& processor) const
{
    processor.processIdentifier(*this);
}



const Token& IdentifierExpression::source() const noexcept
{
    return identifier;
}


TypeSize IdentifierExpression::resultType() const noexcept
{
    return TypeSize::None;
}





CallExpression::CallExpression(const Token& parenLeft, const Expression* const callee, const std::vector<const Expression*>& arguments) noexcept
    : parenLeft(parenLeft), callee(callee), arguments(arguments)
{}


void CallExpression::process(ExpressionProcessor& processor) const
{
    processor.processCall(*this);
}



const Token& CallExpression::source() const noexcept
{
    return parenLeft;
}


TypeSize CallExpression::resultType() const noexcept
{
    return TypeSize::None;
}





CastExpression::CastExpression(const Token& keyword, const Expression* const expression, const Token& type) noexcept
    : keyword(keyword), expression(expression), type(type)
{}


void CastExpression::process(ExpressionProcessor& processor) const
{
    processor.processCast(*this);
}



const Token& CastExpression::source() const noexcept
{
    return keyword;
}


TypeSize CastExpression::resultType() const noexcept
{
    return TypeChecker::stringToTypeSize(type.lexeme);
}