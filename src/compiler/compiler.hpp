#pragma once

#include <array>
#include <stack>

#include <compiler/assembly/assembly_generator.hpp>
#include <compiler/language/statement.hpp>
#include <compiler/language/expression.hpp>
#include <compiler/language/ast_printer.hpp>
#include <compiler/registers.hpp>
#include <compiler/sizes.hpp>




class Compiler : StatementProcessor, ExpressionProcessor
{
private:
    AssemblyGenerator _asm;

    AssemblyGenerator _data;
    AssemblyGenerator _start;
    AssemblyGenerator _code;

    ASTPrinter _astPrinter;

    std::array<Register, 32> _generalRegisters;
    std::vector<Register> _busyRegisters;

    unsigned int _currentExpressionDepth;


    const std::vector<const Statement*> _statements;


    BitMode _bitMode;
    std::string _entryPoint;

    bool _shouldComment;


public:
    static const std::string startLabelName;

    static const TypeSize defaultTypeSize;
    static const ASMTypeSize asmDefaultTypeSize;


    Compiler(const std::vector<const Statement*>& statements, BitMode bitMode, const std::string& entryPoint) noexcept;


    AssemblyGenerator& assembly() noexcept;

    BitMode bitMode() const noexcept;
    const std::string& entryPoint() const noexcept;
    bool shouldComment() const noexcept;


    void setBitmode(BitMode bitMode) noexcept;
    void setEntryPoint(const std::string& entryPoint) noexcept;
    void setShouldComment(bool shouldComment) noexcept;


    void compile();


private:
    void functionDeclarations();

    void startLabel();

    void finish();


    void throwIfAnyNullStatement(const std::vector<const Statement*>& statements) const;


    void comment(AssemblyGenerator& generator, const std::string& comment) noexcept;
    void instantComment(AssemblyGenerator& generator, const std::string& comment) noexcept;


    void moveToFreeRegister(const Register& reg, const std::string& data);
    void moveToFirstFreeRegisterOfSize(ASMTypeSize size, const std::string& data);


    void process(const Statement& statement);

    void processExpression(const ExpressionStatement& statement) override;
    void processDeclaration(const DeclarationStatement& statement) override;
    void processFunctionDeclaration(const FunctionDeclarationStatement& statement) override;
    void processReturn(const ReturnStatement& statement) override;
    void processBlock(const BlockStatement& statement) override;


    void process(const Expression& expression);

    void processLiteral(const LiteralExpression& expression) override;
    void processBinary(const BinaryExpression& expression) override;
    void processGrouping(const GroupingExpression& expression) override;
    void processIdentifier(const IdentifierExpression& expression) override;


    bool isRegisterBusy(const Register& reg) const noexcept;

    void pushRegisterToBusy(const Register& reg);
    Register popLastRegisterFromBusy();
    Register popFirstRegisterFromBusy();

    const Register& getFirstFreeRegisterOfSize(ASMTypeSize size);


    void freeAllBusyRegisters() noexcept;


    static TypeSize stringToTypeSize(const std::string& type);
};