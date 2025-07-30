#pragma once

#include <sstream>

#include <compiler/language/expression.hpp>
#include <compiler/language/statement.hpp>



class ASTPrinter : public ExpressionProcessor, public StatementProcessor
{
private:
    std::stringstream _stream;


public:
    ASTPrinter() = default;


    std::string print(const std::vector<Statement*>& statements) noexcept;


private:
    void processExpression(const ExpressionStatement& statement) override;
    void processDeclaration(const DeclarationStatement& statement) override;

    void endOfStatement() noexcept;


    void processLiteral(const LiteralExpression& expression) override;
    void processBinary(const BinaryExpression& expression) override;
    void processGrouping(const GroupingExpression& expression) override;
    void processIdentifier(const IdentifierExpression& expression) override;


    void stringify(const std::string& name, const std::vector<const Expression*>& expressions);
};