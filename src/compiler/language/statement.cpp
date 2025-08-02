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





DeclarationStatement::DeclarationStatement(const Token& keyword, const Token& name, const Token& type, const Expression* const value) noexcept
    : keyword(keyword), name(name), type(type), value(value)
{}


void DeclarationStatement::process(StatementProcessor& processor) const
{
    processor.processDeclaration(*this);
}


const Token& DeclarationStatement::source() const noexcept
{
    return keyword;
}





FunctionDeclarationStatement::FunctionDeclarationStatement(const Token& keyword, const Token& name, const std::vector<FunctionParameterDeclaration>& parameters,
    const Token& returnType, const BlockStatement* const body) noexcept
        : keyword(keyword), name(name), parameters(parameters), returnType(returnType), body(body)
{}


void FunctionDeclarationStatement::process(StatementProcessor& processor) const
{
    processor.processFunctionDeclaration(*this);
}


const Token& FunctionDeclarationStatement::source() const noexcept
{
    return keyword;
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






BlockStatement::BlockStatement(const Token& start, const Token& end, const std::vector<Statement*>& statements) noexcept
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