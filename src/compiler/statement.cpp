#include <compiler/statement.hpp>



ExpressionStatement::ExpressionStatement(const Expression* const expression) noexcept
    : expression(expression)
{}


void ExpressionStatement::process(StatementProcessor& processor)
{
    processor.processExpression(*this);
}



DeclarationStatement::DeclarationStatement(const Token& name, const Token& type, const Expression* const value) noexcept
    : name(name), type(type), value(value)
{}


void DeclarationStatement::process(StatementProcessor& processor)
{
    processor.processDeclaration(*this);
}