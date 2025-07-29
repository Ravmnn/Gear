#include <compiler/expression.hpp>



LiteralExpression::LiteralExpression(const Token& value) noexcept
    : value(value)
{}


void LiteralExpression::process(ExpressionProcessor& processor) const
{
    processor.processLiteral(*this);
}



BinaryExpression::BinaryExpression(const Expression* const left, const Token& op, const Expression* const right) noexcept
    : left(left), op(op), right(right)
{}


void BinaryExpression::process(ExpressionProcessor& processor) const
{
    processor.processBinary(*this);
}



GroupingExpression::GroupingExpression(const Expression* const expression) noexcept
    : expression(expression)
{}


void GroupingExpression::process(ExpressionProcessor& processor) const
{
    processor.processGrouping(*this);
}



IdentifierExpression::IdentifierExpression(const Token& identifier) noexcept
    : identifier(identifier)
{}


void IdentifierExpression::process(ExpressionProcessor& processor) const
{
    processor.processIdentifier(*this);
}