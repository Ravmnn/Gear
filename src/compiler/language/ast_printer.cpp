#include <compiler/language/ast_printer.hpp>



std::string ASTPrinter::print(const std::vector<Statement*>& statements) noexcept
{
    _stream.clear();

    for (const Statement* const statement : statements)
        if (statement)
            statement->process(*this);

    return _stream.str();
}



void ASTPrinter::processExpression(const ExpressionStatement& statement)
{
    statement.expression->process(*this);
    _stream << ';' << std::endl;
    
    endOfStatement();
}


void ASTPrinter::processDeclaration(const DeclarationStatement& statement)
{
    _stream << statement.name.lexeme << ": " << statement.type.lexeme << " = ";
    statement.value->process(*this);
    
    endOfStatement();
}



void ASTPrinter::endOfStatement() noexcept
{
    _stream << ';' << std::endl;
}



void ASTPrinter::processLiteral(const LiteralExpression& expression)
{
    _stream << expression.value.lexeme;
}


void ASTPrinter::processBinary(const BinaryExpression& expression)
{
    stringify(expression.op.lexeme, { expression.left, expression.right });
}


void ASTPrinter::processGrouping(const GroupingExpression& expression)
{
    stringify("group", { expression.expression });
}


void ASTPrinter::processIdentifier(const IdentifierExpression& expression)
{
    _stream << "$" << expression.identifier.lexeme;
}




void ASTPrinter::stringify(const std::string& name, const std::vector<const Expression*>& expressions)
{
    _stream << "(" << name;

    for (const Expression* const expression : expressions)
    {
        _stream << ' ';
        expression->process(*this);
    }

    _stream << ")";
}