#pragma once

#include <sstream>

#include <compiler/language/expression.hpp>
#include <compiler/language/statement.hpp>



class ASTPrinter : ExpressionProcessor, StatementProcessor
{
private:
    std::stringstream _stream;
    unsigned int _indentDegree;


public:
    ASTPrinter() = default;


    std::string print(const std::vector<Statement*>& statements) noexcept;


private:
    void increaseIndent() noexcept;
    void decreaseIndent() noexcept;

    std::string indent() const noexcept;

    void beginStatement() noexcept;
    void endStatement() noexcept;

    void stringify(const std::string& name, const std::vector<const Expression*>& expressions);



    void processExpression(const ExpressionStatement& statement) override;
    void processDeclaration(const DeclarationStatement& statement) override;
    void processFunctionDeclaration(const FunctionDeclarationStatement& statement) override;
    void processReturn(const ReturnStatement& statement) override;
    void processBlock(const BlockStatement& statement) override;


    void processLiteral(const LiteralExpression& expression) override;
    void processBinary(const BinaryExpression& expression) override;
    void processGrouping(const GroupingExpression& expression) override;
    void processIdentifier(const IdentifierExpression& expression) override;


};