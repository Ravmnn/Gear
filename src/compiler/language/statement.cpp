#include <compiler/language/statement.hpp>



CompilerAttributeStatement::CompilerAttributeStatement(const Token& name, const Token& value) noexcept
    : name(name), value(value)
{}


void CompilerAttributeStatement::process(StatementProcessor& processor) const
{
    processor.processCompilerAttribute(*this);
}



ExpressionStatement::ExpressionStatement(const Expression* const expression) noexcept
    : expression(expression)
{}


void ExpressionStatement::process(StatementProcessor& processor) const
{
    processor.processExpression(*this);
}



DeclarationStatement::DeclarationStatement(const Token& name, const Token& type, const Expression* const value) noexcept
    : name(name), type(type), value(value)
{}


void DeclarationStatement::process(StatementProcessor& processor) const
{
    processor.processDeclaration(*this);
}