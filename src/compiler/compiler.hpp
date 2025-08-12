#pragma once

#include <array>

#include <compiler/assembly/assembly_generator.hpp>
#include <compiler/language/ast_printer.hpp>
#include <compiler/program_scope.hpp>
#include <compiler/registers.hpp>
#include <compiler/sizes.hpp>



class CompilerException;
class InternalException;


class Compiler : StatementProcessor, ExpressionProcessor
{
private:
    AssemblyGenerator _asm;

    AssemblyGenerator _data;
    AssemblyGenerator _start;
    AssemblyGenerator _code;

    AssemblyGenerator* _current;

    ASTPrinter _astPrinter;

    RegisterManager _registers;

    ProgramScope _scope;
    IdentifierManager& _scopeLocal;
    IdentifierManager& _scopeGlobal;

    unsigned int _currentExpressionDepth;
    unsigned int _currentStatementDepth;

    bool _returnValue;


    const Expression* _currentExpression;
    const Statement* _currentStatement;


    const std::vector<const Statement*> _statements;


    BitMode _bitMode;
    std::string _entryPoint;

    bool _shouldComment;


public:
    static const std::string startLabelName;

    static const TypeSize defaultTypeSize;
    static const ASMTypeSize asmDefaultTypeSize;

    static const std::string stackPointerRegister;
    static const std::string stackFrameRegister;


    static const std::stringstream* assemblyAborted;


    Compiler(const std::vector<const Statement*>& statements, BitMode bitMode, const std::string& entryPoint) noexcept;


    AssemblyGenerator& assembly() noexcept;

    BitMode bitMode() const noexcept;
    const std::string& entryPoint() const noexcept;
    bool shouldComment() const noexcept;


    void setBitmode(BitMode bitMode) noexcept;
    void setEntryPoint(const std::string& entryPoint) noexcept;
    void setShouldComment(bool shouldComment) noexcept;


private:
    void resetDepth() noexcept;


public:
    void compile();


private:
    void code();
    void startLabel();
    void finish();


    std::vector<const FunctionDeclarationStatement*> statementsToFunctions(const std::vector<const Statement*>& statements);
    void defineFunctionIdentifiers(const std::vector<const FunctionDeclarationStatement*>& function);
    void compileFunction(const FunctionDeclarationStatement* function);


    CompilerException internalException5ToCompilerException(const InternalException& exception) const;


    void comment(const std::string& comment) noexcept;
    void instantComment(const std::string& comment) noexcept;


    void moveToFreeRegister(Register& reg, const std::string& data);
    void moveZeroExtendToFreeRegister(Register& reg, const std::string& data);
    void moveToFirstFreeRegisterOfSize(ASMTypeSize size, const std::string& data);

    void allocateIdentifierOnStack(const Identifier& identifier, const std::string& value = "0");


    void call(const std::string& function);

    void cast(Register& value, const ASMTypeSize size);


    void stackFrameBegin() noexcept;
    void stackFrameEnd() noexcept;

    void scopeBegin() noexcept;
    void scopeEnd() noexcept;


    unsigned int addressDisplacementOfIdentifierOnStack(const Identifier& identifier) const;
    std::string addressOfIdentifierOnStack(const Identifier& identifier) const;


    void process(const Statement& statement);
    void process(const std::vector<const Statement*>& statements);

    void processExpression(const ExpressionStatement& statement) override;
    void processDeclaration(const DeclarationStatement& statement) override;
    void processFunctionDeclaration(const FunctionDeclarationStatement& statement) override;
    void processReturn(const ReturnStatement& statement) override;
    void processBlock(const BlockStatement& statement) override;

    void processFunctionBlock(const BlockStatement& statement);

    void defineFunctionIdentifierInGlobal(const FunctionDeclarationStatement& statement);


    void process(const Expression& expression);

    void processLiteral(const LiteralExpression& expression) override;
    void processBinary(const BinaryExpression& expression) override;
    void processGrouping(const GroupingExpression& expression) override;
    void processIdentifier(const IdentifierExpression& expression) override;
    void processCall(const CallExpression& expression) override;
    void processCast(const CastExpression& expression) override;

    void processBinaryOperands(const BinaryExpression& expression, bool& leftProcessed, bool& rightProcessed, bool& leftProcessedFirst, bool& rightProcessedFirst);

    bool processExpressionIf(const Expression& expression, bool condition);
    bool processExpressionIfNotLiteral(const Expression& expression);
    static void updateBinaryProcessingFlags(bool& processed, bool& processedFirst, bool& otherProcessedFirst) noexcept;


    Register& consumeValueOfExpression(const Expression& expression);
    Register& consumeValue();

    Register& getValueOfExpression(const Expression& expression);
    Register& getValue();


    static TypeSize stringToTypeSize(const std::string& type);


    static void throwIfAnyNullStatement(const std::vector<const Statement*>& statements);
};