#include <compiler/language/statement.hpp>

#include <compiler/language/expression.hpp>



ExpressionStatement::ExpressionStatement(const Expression* const expression) noexcept
    : expression(expression)
{}


void ExpressionStatement::process(StatementProcessor& processor) const
{
    processor.processExpression(*this);
}


const Token& ExpressionStatement::source() const noexcept
{
    return expression->source();
}





DeclarationStatement::DeclarationStatement(const Token& name, const Token& type, const Expression* const value) noexcept
    : name(name), type(type), value(value)
{}


void DeclarationStatement::process(StatementProcessor& processor) const
{
    processor.processDeclaration(*this);
}


const Token& DeclarationStatement::source() const noexcept
{
    return type;
}





FunctionDeclarationStatement::FunctionDeclarationStatement(const Token& name, const Token& returnType, const std::vector<FunctionParameterDeclaration>& parameters,
    const BlockStatement* const body) noexcept
        : name(name), returnType(returnType), parameters(parameters), body(body)
{}


void FunctionDeclarationStatement::process(StatementProcessor& processor) const
{
    processor.processFunctionDeclaration(*this);
}


const Token& FunctionDeclarationStatement::source() const noexcept
{
    return returnType;
}





ReturnStatement::ReturnStatement(const Token& keyword, const Expression* const expression) noexcept
    : keyword(keyword), expression(expression)
{}


void ReturnStatement::process(StatementProcessor& processor) const
{
    processor.processReturn(*this);
}


const Token& ReturnStatement::source() const noexcept
{
    return keyword;
}






BlockStatement::BlockStatement(const Token& start, const Token& end, const std::vector<const Statement*>& statements) noexcept
    : start(start), end(end), statements(statements)
{}


void BlockStatement::process(StatementProcessor& processor) const
{
    processor.processBlock(*this);
}


const Token& BlockStatement::source() const noexcept
{
    return start;
}