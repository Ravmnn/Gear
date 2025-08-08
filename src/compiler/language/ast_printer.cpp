#include <compiler/language/ast_printer.hpp>

#include <log.hpp>



ASTPrinter::ASTPrinter()
{
    _indentDegree = 0;
    _ignoreBlocks = false;
    _noNewlines = false;
}



bool ASTPrinter::ignoreBlocks() const noexcept
{
    return _ignoreBlocks;
}


bool ASTPrinter::noNewlines() const noexcept
{
    return _noNewlines;
}





void ASTPrinter::setIgnoreBlocks(const bool ignoreBlocks) noexcept
{
    _ignoreBlocks = ignoreBlocks;
}


void ASTPrinter::setNoNewlines(const bool noNewlines) noexcept
{
    _noNewlines = noNewlines;
}



std::string ASTPrinter::print(const std::vector<const Statement*>& statements) noexcept
{
    _stream = {};

    for (const Statement* const statement : statements)
        if (statement)
            process(*statement);

    return _stream.str();
}


std::string ASTPrinter::print(const Statement& statement) noexcept
{
    _stream = {};

    process(statement);

    return _stream.str();
}


std::string ASTPrinter::print(const Expression& expression) noexcept
{
    _stream = {};

    process(expression);

    return _stream.str();
}



void ASTPrinter::parenthesize(const Expression* const expression)
{
    _stream << "(";

    process(*expression);

    _stream << ")";
}


void ASTPrinter::stringify(const std::string& name, const std::vector<const Expression*>& expressions)
{
    if (expressions.size() == 1)
        unaryStringify(name, expressions[0]);

    else if (expressions.size() == 2)
        binaryStringify(name, expressions[0], expressions[1]);

    else if (expressions.size() >= 2)
        multiOperandStringify(name, expressions);
}


void ASTPrinter::unaryStringify(const std::string& name, const Expression* const operand)
{
    _stream << "(" << name << " ";

    process(*operand);

    _stream << ")";
}


void ASTPrinter::binaryStringify(const std::string& name, const Expression* const left, const Expression* const right)
{
    _stream << "(";
    process(*left);

    _stream << ' ' << name << ' ';

    process(*right);
    _stream << ")";
}


void ASTPrinter::multiOperandStringify(const std::string& name, const std::vector<const Expression*>& expressions)
{
    _stream << "(" << name;

    for (const Expression* const expression : expressions)
    {
        _stream << ' ';
        process(*expression);
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


std::string ASTPrinter::newlineChar() const noexcept
{
    return _noNewlines ? "" : "\n";
}



void ASTPrinter::beginStatement() noexcept
{
    _stream << indent();
}

void ASTPrinter::endStatement() noexcept
{
    _stream << newlineChar();
}





void ASTPrinter::process(const Statement& statement)
{
    statement.process(*this);
}



void ASTPrinter::processExpression(const ExpressionStatement& statement)
{
    beginStatement();
    
    process(*statement.expression);
    
    endStatement();
}


void ASTPrinter::processDeclaration(const DeclarationStatement& statement)
{
    beginStatement();

    _stream << statement.type.lexeme << " " << statement.name.lexeme << " = ";
    process(*statement.value);
    
    endStatement();
}


void ASTPrinter::processFunctionDeclaration(const FunctionDeclarationStatement& statement)
{
    beginStatement();

    _stream << statement.returnType.lexeme << " " << statement.name.lexeme << "(";
    
    for (const FunctionParameterDeclaration& parameter : statement.parameters)
    {
        const bool atEnd = &parameter == (statement.parameters.cend() - 1).base();

        _stream << parameter.type.lexeme << " " << parameter.name.lexeme << (!atEnd ? ", " : "");
    }

    _stream << ")" << newlineChar();

    process(*statement.body);
    
    endStatement();
}


void ASTPrinter::processReturn(const ReturnStatement& statement)
{
    beginStatement();

    _stream << "return ";
    process(*statement.expression);

    endStatement();
}


void ASTPrinter::processBlock(const BlockStatement& statement)
{
    if (_ignoreBlocks)
    {
        _stream << " block...";
        return;
    }

    beginStatement();
    _stream << "start" << newlineChar();
    
    increaseIndent();

    for (const Statement* const statement : statement.statements)
        process(*statement);

    decreaseIndent();
    
    _stream << indent() << "end";
    endStatement();
}





void ASTPrinter::process(const Expression& expression)
{
    expression.process(*this);
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
    parenthesize(expression.expression);
}


void ASTPrinter::processIdentifier(const IdentifierExpression& expression)
{
    _stream << "$" << expression.identifier.lexeme;
}


void ASTPrinter::processCall(const CallExpression& expression)
{
    process(*expression.callee);
    _stream << "(";

    for (const Expression* const argument : expression.arguments)
    {
        const bool atEnd = &argument == (expression.arguments.cend() - 1).base();

        process(*argument);
        _stream << (atEnd ? "" : ", ");
    }

    _stream << ")";
}