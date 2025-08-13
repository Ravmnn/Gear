#include <compiler/type_checker.hpp>

#include <torque.hpp>
#include <compiler/sizes.hpp>
#include <compiler/exceptions/exceptions.hpp>



TypeChecker::TypeChecker(const Compiler& compiler) noexcept
    : _compiler(compiler)
{}



void TypeChecker::process(const Statement& statement)
{
    statement.process(*this);
}


void TypeChecker::process(const Expression& expression)
{
    expression.process(*this);
}





void TypeChecker::processExpression(const ExpressionStatement& statement)
{
    process(*statement.expression);
}


void TypeChecker::processDeclaration(const DeclarationStatement& statement)
{
    // TODO: throw if value type differs from the variable type
    
    check(stringToTypeSize(statement.type.lexeme));

    process(*statement.value);
}


void TypeChecker::processFunctionDeclaration(const FunctionDeclarationStatement& statement)
{
    // TODO: throw if the return value differs from the return type

    check(stringToTypeSize(statement.returnType.lexeme));

    for (const FunctionParameterDeclaration& parameter : statement.parameters)
        check(stringToTypeSize(parameter.type.lexeme));

    process(*statement.body);
}


void TypeChecker::processReturn(const ReturnStatement& statement)
{
    process(*statement.expression);
}


void TypeChecker::processBlock(const BlockStatement& statement)
{
    for (const Statement* const statementLine : statement.statements)
        process(*statementLine);
}





void TypeChecker::processLiteral(const LiteralExpression& expression)
{
    check(expression.resultType());
}


void TypeChecker::processBinary(const BinaryExpression& expression)
{
    process(*expression.left);
    process(*expression.right);

    throwIfTypeSizeDiffers(*expression.left, *expression.right, expression.source().position);
}


void TypeChecker::processGrouping(const GroupingExpression& expression)
{
    process(*expression.expression);
}


void TypeChecker::processIdentifier(const IdentifierExpression& expression)
{
    const Identifier* const identifier = _compiler._scope.getIdentifier(expression.identifier.lexeme);

    // TODO: add the ability to the parser to know the identifiers declared.
    // TODO: after that, fix IdentifierExpression::resultType()
}


void TypeChecker::processCall(const CallExpression& expression)
{
    // TODO: finish TODOs above
}


void TypeChecker::processCast(const CastExpression& expression)
{
    check(expression.resultType());
}





void TypeChecker::check(const TypeSize typeSize) const
{
    Torque::sizes()->check(typeSize);
}





bool TypeChecker::areOfSameTypeSize(const Expression& left, const Expression& right) noexcept
{
    return left.resultType() == right.resultType();
}





void TypeChecker::throwIfTypeSizeDiffers(const Expression& left, const Expression& right, const TokenPosition& position)
{
    if (!areOfSameTypeSize(left, right))
        throw torque_e3006(position);
}





TypeSize TypeChecker::stringToTypeSize(const std::string& type)
{
    if (type == "byte") return TypeSize::Byte;
    if (type == "char") return TypeSize::Char;
    if (type == "bool") return TypeSize::Bool;
    if (type == "uint8") return TypeSize::UInt8;
    if (type == "uint16") return TypeSize::UInt16;
    if (type == "uint32") return TypeSize::UInt32;
    if (type == "uint64") return TypeSize::UInt64;
    if (type == "int8") return TypeSize::Int8;
    if (type == "int16") return TypeSize::Int16;
    if (type == "int32") return TypeSize::Int32;
    if (type == "int64") return TypeSize::Int64;

    throw internal_e0000_argument();
}