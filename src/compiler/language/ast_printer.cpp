#include <compiler/language/ast_printer.hpp>

#include <log.hpp>



std::string ASTPrinter::print(const std::vector<Statement*>& statements) noexcept
{
    _stream.clear();

    for (const Statement* const statement : statements)
        if (statement)
            statement->process(*this);

    return _stream.str();
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



void ASTPrinter::increaseIndent() noexcept
{
    _indentDegree++;
}

void ASTPrinter::decreaseIndent() noexcept
{
    _indentDegree--;
}


std::string ASTPrinter::indent() const noexcept
{
    std::string indent;

    for (size_t i = 0; i < _indentDegree; i++)
        indent += "    ";

    return indent;
}



void ASTPrinter::beginStatement() noexcept
{
    _stream << indent();
}

void ASTPrinter::endStatement() noexcept
{
    _stream << std::endl;
}





void ASTPrinter::processExpression(const ExpressionStatement& statement)
{
    beginStatement();
    
    statement.expression->process(*this);
    
    endStatement();
}


void ASTPrinter::processDeclaration(const DeclarationStatement& statement)
{
    beginStatement();

    _stream << statement.name.lexeme << ": " << statement.type.lexeme << " = ";
    statement.value->process(*this);
    
    endStatement();
}


void ASTPrinter::processFunctionDeclaration(const FunctionDeclarationStatement& statement)
{
    beginStatement();

    _stream << "function " << statement.name.lexeme << "(";
    
    for (const FunctionParameterDeclaration& parameter : statement.parameters)
    {
        const bool atEnd = &parameter == (statement.parameters.cend() - 1).base();

        _stream << parameter.name.lexeme << ": " << parameter.type.lexeme << (!atEnd ? ", " : "");
    }

    _stream << ")" << " -> " << statement.returnType.lexeme << std::endl;

    processBlock(*statement.body);
    
    endStatement();
}


void ASTPrinter::processReturn(const ReturnStatement& statement)
{
    beginStatement();

    _stream << "return ";
    statement.expression->process(*this);

    endStatement();
}


void ASTPrinter::processBlock(const BlockStatement& statement)
{
    beginStatement();
    _stream << "start" << std::endl;
    
    increaseIndent();

    for (Statement* const statement : statement.statements)
        statement->process(*this);

    decreaseIndent();
    
    _stream << indent() << "end";
    endStatement();
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