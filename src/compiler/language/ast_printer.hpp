#pragma once

#include <sstream>

#include <compiler/language/expression.hpp>
#include <compiler/language/statement.hpp>



class ASTPrinter : ExpressionProcessor, StatementProcessor
{
private:
    std::stringstream _stream;
    unsigned int _indentDegree;

    bool _ignoreBlocks;
    bool _noNewlines;


public:
    ASTPrinter();


    bool ignoreBlocks() const noexcept;
    bool noNewlines() const noexcept;


    void setIgnoreBlocks(bool ignoreBlocks) noexcept;
    void setNoNewlines(bool noNewlines) noexcept;

    
    std::string print(const std::vector<const Statement*>& statements) noexcept;

    std::string print(const Expression& expression) noexcept;


private:
    void increaseIndent() noexcept;
    void decreaseIndent() noexcept;

    std::string indent() const noexcept;
    std::string newlineChar() const noexcept;

    void beginStatement() noexcept;
    void endStatement() noexcept;

    void parenthesize(const Expression* expression);

    void stringify(const std::string& name, const std::vector<const Expression*>& expressions);

    void unaryStringify(const std::string& name, const Expression* operand);
    void binaryStringify(const std::string& name, const Expression* left, const Expression* right);
    
    void multiOperandStringify(const std::string& name, const std::vector<const Expression*>& expressions);



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