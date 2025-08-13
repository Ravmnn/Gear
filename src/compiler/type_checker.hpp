#pragma once

#include <compiler/language/statement.hpp>
#include <compiler/language/expression.hpp>



enum class BitMode;

class Compiler;


class TypeChecker : StatementProcessor, ExpressionProcessor
{
private:
    const Compiler& _compiler;


public:
    TypeChecker(const Compiler& compiler) noexcept;


    void process(const Statement& statement);
    void process(const Expression& statement);


private:
    void processExpression(const ExpressionStatement& statement) override;
    void processDeclaration(const DeclarationStatement& statement) override;
    void processFunctionDeclaration(const FunctionDeclarationStatement& statement) override;
    void processReturn(const ReturnStatement& statement) override;
    void processBlock(const BlockStatement& statement) override;


    void processLiteral(const LiteralExpression& expression) override;
    void processBinary(const BinaryExpression& expression) override;
    void processGrouping(const GroupingExpression& expression) override;
    void processIdentifier(const IdentifierExpression& expression) override;
    void processCall(const CallExpression& expression) override;
    void processCast(const CastExpression& expression) override;


    void check(TypeSize typeSize) const;


    static bool areOfSameTypeSize(const Expression& left, const Expression& right) noexcept;


    static void throwIfTypeSizeDiffers(const Expression& left, const Expression& right, const TokenPosition& position);


public:
    static TypeSize stringToTypeSize(const std::string& type);
};