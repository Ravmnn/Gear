#include <compiler/statement.hpp>



DeclarationStatement::DeclarationStatement(const Token& name, const Token& type, const Token& value) noexcept
    : name(name), type(type), value(value)
{}


void DeclarationStatement::process(StatementProcessor& processor)
{
    processor.processDeclaration(*this);
}