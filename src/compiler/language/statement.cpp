#include <compiler/language/statement.hpp>



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





FunctionDeclarationStatement::FunctionDeclarationStatement(const Token& name, const std::vector<FunctionParameterDeclaration>& parameters,
    const Token& returnType, const BlockStatement* const body) noexcept
        : name(name), parameters(parameters), returnType(returnType), body(body)
{}


void FunctionDeclarationStatement::process(StatementProcessor& processor) const
{
    processor.processFunctionDeclaration(*this);
}





ReturnStatement::ReturnStatement(const Expression* const expression) noexcept
    : expression(expression)
{}


void ReturnStatement::process(StatementProcessor& processor) const
{
    processor.processReturn(*this);
}





BlockStatement::BlockStatement(const std::vector<Statement*>& statements) noexcept
    : statements(statements)
{}


void BlockStatement::process(StatementProcessor& processor) const
{
    processor.processBlock(*this);
}