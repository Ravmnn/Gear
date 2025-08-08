#include <compiler/compiler.hpp>

#include <algorithm>

#include <compiler/language/statement.hpp>
#include <compiler/language/expression.hpp>
#include <compiler/exceptions/exceptions.hpp>
#include <compiler/assembly/nasm.hpp>
#include <compiler/sizes.hpp>
#include <log.hpp>



// TODO: add support to function calling (and returning values)
// TODO: add support to function arguments
// TODO: add support to external functions
// TODO: add basic stdlib

// TODO: add support to negative numbers
// TODO: add support to float numbers
// TODO: detect overflows
// TODO: add support to type convertions
// TODO: add support to multiply and division operation
// TODO: add support to strings
// TODO: add support to arrays



const std::string Compiler::startLabelName = "_start";

const TypeSize Compiler::defaultTypeSize = TypeSize::Int32;
const ASMTypeSize Compiler::asmDefaultTypeSize = ASMTypeSize::DWord;

const std::string Compiler::stackPointerRegister = "rsp";
const std::string Compiler::stackFrameRegister = "rbp";
//const std::string Compiler::returnRegister = "rax";



Compiler::Compiler(const std::vector<const Statement*>& statements, const BitMode bitMode, const std::string& entryPoint) noexcept
    : _scopeLocal(_scope.local()), _scopeGlobal(_scope.global()), _statements(statements)
{
    _astPrinter.setIgnoreBlocks(true);
    _astPrinter.setNoNewlines(true);


    _currentExpressionDepth = 0;
    _currentStatementDepth = 0;

    _currentExpression = nullptr;
    _currentStatement = nullptr;


    _bitMode = bitMode;
    _entryPoint = entryPoint;

    _shouldComment = false;
}



AssemblyGenerator& Compiler::assembly() noexcept
{
    return _asm;
}


BitMode Compiler::bitMode() const noexcept
{
    return _bitMode;
}


const std::string& Compiler::entryPoint() const noexcept
{
    return _entryPoint;
}


bool Compiler::shouldComment() const noexcept
{
    return _shouldComment;
}




void Compiler::setBitmode(const BitMode bitMode) noexcept
{
    _bitMode = bitMode;
}


void Compiler::setEntryPoint(const std::string& entryPoint) noexcept
{
    _entryPoint = entryPoint;
}


void Compiler::setShouldComment(const bool shouldComment) noexcept
{
    _shouldComment = shouldComment;
}



void Compiler::resetDepth() noexcept
{
    _currentExpressionDepth = _currentStatementDepth = 0;
}




void Compiler::compile()
{
    throwIfAnyNullStatement(_statements);
    
    code();
    startLabel();
    finish();
}


void Compiler::code()
{
    std::vector<const FunctionDeclarationStatement*> functions = statementsToFunctions(_statements);

    // function identifiers should be defined before they are processed
    defineFunctionIdentifiers(functions);

    for (const FunctionDeclarationStatement* function : functions)
    {
        compileFunction(function);

        // security reset, in case of an exception
        resetDepth();
        _registers.freeAllBusyRegisters();
    }
}


void Compiler::startLabel()
{
    _start.nasmDirectiveGlobal(startLabelName);
    _start.label(startLabelName);
    
    _start.enableIndent();


    comment(_start, "init stack frame");
    _start.instruction("mov", "rbp", "rsp");
    _start.newline();

    _start.instruction("call", _entryPoint);
    _start.newline();

    instantComment(_start, "syscall exit");
    _start.syscallExit(0);


    _start.disableIndent();
}


void Compiler::finish()
{
    _asm.nasmDirectiveBits(_bitMode);
    _asm.newline(2);



    _asm.nasmDirectiveSection(".data");
    _asm.newline();

    _asm.insertOther(_data);

    _asm.newline(2);



    _asm.nasmDirectiveSection(".text");
    _asm.newline();

    _asm.insertOther(_start);

    _asm.newline(4);


    
    _asm.insertOther(_code);
}





std::vector<const FunctionDeclarationStatement*> Compiler::statementsToFunctions(const std::vector<const Statement*>& statements)
{
    std::vector<const FunctionDeclarationStatement*> functions;

    for (const Statement* const statement : _statements)
    {
        const FunctionDeclarationStatement* function = dynamic_cast<const FunctionDeclarationStatement*>(statement);

        if (function)
            functions.push_back(function);
    }

    return functions;
}



void Compiler::defineFunctionIdentifiers(const std::vector<const FunctionDeclarationStatement*>& functions)
{
    for (const FunctionDeclarationStatement* const function : functions)
        defineFunctionIdentifierInGlobal(*function);
}



void Compiler::compileFunction(const FunctionDeclarationStatement* function)
{
    try
    {
        if (function)
            process(*function);
        else
            throw torque_e3000(function->source().position);
    }

    catch (const CompilerException& exception)
    {
        // when a compiler error occurs, the function being compiled is ignored

        error(&exception);
    }

    catch (const InternalException& exception)
    {
        const CompilerException newException = internalException5ToCompilerException(exception);
        error(&newException);
    }
}





CompilerException Compiler::internalException5ToCompilerException(const InternalException& exception) const
{
    if (!_currentExpression && !_currentStatement)
        throw internal_e0000_nullptr();

    const TokenPosition& position =
                _currentExpression ? _currentExpression->source().position : _currentStatement->source().position;

    if (exception.id() == internal_e0003().id())
        return torque_e3004(position);

    throw internal_e0000_argument();
}





void Compiler::comment(AssemblyGenerator& generator, const std::string& comment) noexcept
{
    if (_shouldComment)
        generator.comment(comment);
}

void Compiler::instantComment(AssemblyGenerator& generator, const std::string& comment) noexcept
{
    if (_shouldComment)
        generator.instantComment(comment);
}



void Compiler::moveToFreeRegister(const Register& reg, const std::string& data)
{
    _code.instruction("mov", reg.name, data);

    _registers.pushRegisterToBusy(reg);
}


void Compiler::moveToFirstFreeRegisterOfSize(const ASMTypeSize size, const std::string& data)
{
    moveToFreeRegister(_registers.getFirstFreeRegisterOfSize(size), data);
}



void Compiler::allocateIdentifierOnStack(const Identifier& identifier, const std::string& value)
{
    _scopeLocal.defineIdentifier(identifier);

    const std::string sizeInBytesString = std::to_string((unsigned int)identifier.size);
    const std::string address = addressOfIdentifierOnStack(identifier);

    _code.instruction("sub", stackPointerRegister, sizeInBytesString);
    _code.instruction("mov", address, value);
}



void Compiler::stackFrameBegin() noexcept
{
    // TODO: make _identifiers' scope handling work with stack frame

    instantComment(_code, "stack frame begin");
    _code.instruction("push", stackFrameRegister);
    _code.instruction("mov", stackFrameRegister, stackPointerRegister);
    _code.instruction("add", stackFrameRegister, "16");
    _code.newline(2);
}


void Compiler::stackFrameEnd() noexcept
{
    instantComment(_code, "stack frame end");
    _code.instruction("sub", stackFrameRegister, "16");
    _code.instruction("mov", stackPointerRegister, stackFrameRegister);
    _code.instruction("pop", stackFrameRegister);
    _code.newline();
}



void Compiler::scopeBegin() noexcept
{
    _scopeLocal.scopeBegin();
}


void Compiler::scopeEnd() noexcept
{
    const std::string sizeInBytesString = std::to_string(_scopeLocal.sizeOfCurrentScopeInBytes());

    _scopeLocal.scopeEnd();

    _code.instruction("add", stackPointerRegister, sizeInBytesString);
}





unsigned int Compiler::addressDisplacementOfIdentifierOnStack(const Identifier& identifier) const
{
    _scope.throwIfNotDefined(identifier.name, identifier.position);

    unsigned int offsetInBytes = 0;

    for (int i = _scopeLocal.identifiers().size() - 1; i >= 0; i--)
    {
        const Identifier& currentIdentifier = _scopeLocal.identifiers()[i];

        if (currentIdentifier.name == identifier.name)
            break;

        offsetInBytes += (unsigned int)currentIdentifier.size;
    }

    return offsetInBytes;
}


std::string Compiler::addressOfIdentifierOnStack(const Identifier& identifier) const
{
    const unsigned int addressDisplacement = addressDisplacementOfIdentifierOnStack(identifier);

    return AssemblyGenerator::addressing(stackPointerRegister, addressDisplacement);
}





void Compiler::process(const Statement& statement)
{
    instantComment(_code, _astPrinter.print(statement));

    // pop return register if it's busy
    if (_registers.isRegisterOfFamilyBusy(9))
        _registers.popRegisterOfFamily(9);


    const Statement* oldStatement = _currentStatement;
    _currentStatement = &statement;

    _currentStatementDepth++;
    statement.process(*this);
    _currentStatementDepth--;

    _currentStatement = oldStatement;

    _code.newline();


    // after a statement ends, all registers are supposed to be free
    _registers.freeAllBusyRegisters();
}


void Compiler::process(const std::vector<const Statement*>& statements)
{
    for (const Statement* const statement : statements)
        process(*statement);
}



void Compiler::processExpression(const ExpressionStatement& statement)
{
    process(*statement.expression);
}


void Compiler::processDeclaration(const DeclarationStatement& statement)
{
    const ASMTypeSize size = (ASMTypeSize)stringToTypeSize(statement.type.lexeme);
    const std::string value = getValueOfExpression(*statement.value);

    allocateIdentifierOnStack(Identifier{ statement.name.lexeme, size, statement.name.position }, value);
}


void Compiler::processFunctionDeclaration(const FunctionDeclarationStatement& statement)
{
    // depth 1 is file scope

    if (_currentStatementDepth > 1)
        throw torque_e3003(statement.source().position);

    const IdentifierManager oldScope = _scopeLocal;
    
    
    // pop return register if it's busy
    if (_registers.isRegisterOfFamilyBusy(9))
        _registers.popRegisterOfFamily(9);

    _scopeLocal.clear(); // function scope cannot access another function's scope

    _code.label(statement.name.lexeme);
    _code.enableIndent();

    stackFrameBegin();
    processFunctionBlock(*statement.body); // don't use Compiler::processBlock because it creates a scope. Function scope is handled by stack frames.
    stackFrameEnd();
;
    _code.instruction("ret");

    _code.disableIndent();


    _scopeLocal = oldScope;
}


void Compiler::processReturn(const ReturnStatement& statement)
{
    _registers.prioritizeReturnRegister();

    process(*statement.expression);
}


void Compiler::processBlock(const BlockStatement& statement)
{
    instantComment(_code, "start");
    _code.newline();


    scopeBegin();
    process(statement.statements);
    scopeEnd();


    _code.newline();
    instantComment(_code, "end");
}



void Compiler::processFunctionBlock(const BlockStatement& statement)
{
    process(statement.statements);
}



void Compiler::defineFunctionIdentifierInGlobal(const FunctionDeclarationStatement& statement)
{
    const ASMTypeSize returnType = (ASMTypeSize)stringToTypeSize(statement.returnType.lexeme);
    const Identifier identifier = Identifier{ statement.name.lexeme, returnType, statement.source().position, true };

    _scopeGlobal.defineIdentifier(identifier);
}





void Compiler::process(const Expression& expression)
{
    const Expression* oldExpression = _currentExpression;
    _currentExpression = &expression;

    _currentExpressionDepth++;
    expression.process(*this);
    _currentExpressionDepth--;

    _currentExpression = oldExpression;
}



void Compiler::processLiteral(const LiteralExpression& expression)
{
    std::string value = expression.value.lexeme;

    if (expression.value.isBoolean())
        value = std::to_string(Token::stringToBoolean(value));

    moveToFirstFreeRegisterOfSize(asmDefaultTypeSize, value);
}


void Compiler::processBinary(const BinaryExpression& expression)
{
    bool leftProcessed = false;
    bool rightProcessed = false;

    bool leftProcessedFirst = false;
    bool rightProcessedFirst = false;

    processBinaryOperands(expression, leftProcessed, rightProcessed, leftProcessedFirst, rightProcessedFirst);

    Register right = _registers.popLastRegisterFromBusy();
    Register left = _registers.popLastRegisterFromBusy();

    if (rightProcessedFirst)
        std::swap(left, right);

    // the result of a binary operation is stored in the left register,
    // so it's needed to re-occupy it after consuming (freeing) it to avoid
    // the loss of the result

    _registers.pushRegisterToBusy(left);

    switch (expression.op.type)
    {
    case TokenType::Plus:
        _code.instruction("add", left.name, right.name);
        break;

    case TokenType::Minus:
        _code.instruction("sub", left.name, right.name);
        break;

    default:
        throw internal_e0000_argument();
    }
}


void Compiler::processGrouping(const GroupingExpression& expression)
{
    process(*expression.expression);
}


void Compiler::processIdentifier(const IdentifierExpression& expression)
{
    _scope.throwIfNotDefined(expression.identifier.lexeme, expression.identifier.position);

    const Identifier* identifier = _scope.getIdentifier(expression.identifier.lexeme);
    const bool isInstructionAddress = identifier->isInstructionAddress;

    const std::string value = isInstructionAddress ? identifier->name : addressOfIdentifierOnStack(*identifier);
    const ASMTypeSize registerSize = isInstructionAddress ? (ASMTypeSize)TypeSize::FPtr : identifier->size;

    moveToFirstFreeRegisterOfSize(registerSize, value);
}


void Compiler::processCall(const CallExpression& expression)
{
    // TODO: detect whether the callee is a function or not

    const IdentifierExpression* const identifier = dynamic_cast<const IdentifierExpression*>(expression.callee);

    if (!identifier)
        throw internal_e0000();

    const std::string function = identifier->identifier.lexeme;

    // return type size of the function
    const ASMTypeSize registerSize = _scope.getIdentifier(function)->size;
    const Register& returnRegister = _registers.getReturnRegisterOfSize(registerSize);

    // when calling a function, if it returns any value, it's necessary to push the return register (rax)
    // to busy, so we can get its value later.

    _registers.pushRegisterToBusy(returnRegister);


    const std::string value = getValueOfExpression(*expression.callee);

    _code.instruction("call", value);
}





void Compiler::processBinaryOperands(const BinaryExpression& expression, bool& leftProcessed, bool& rightProcessed, bool& leftProcessedFirst, bool& rightProcessedFirst)
{
    if (processExpressionIfNotLiteral(*expression.left))
        updateBinaryProcessingFlags(leftProcessed, leftProcessedFirst, rightProcessedFirst);
    
    if (processExpressionIfNotLiteral(*expression.right))
        updateBinaryProcessingFlags(rightProcessed, rightProcessedFirst, leftProcessedFirst);


    if (processExpressionIf(*expression.left, !leftProcessed))
        updateBinaryProcessingFlags(leftProcessed, leftProcessedFirst, rightProcessedFirst);

    if (processExpressionIf(*expression.right, !rightProcessed))
        updateBinaryProcessingFlags(rightProcessed, rightProcessedFirst, leftProcessedFirst);
}





bool Compiler::processExpressionIf(const Expression& expression, const bool condition)
{
    if (condition)
        process(expression);

    return condition;
}

bool Compiler::processExpressionIfNotLiteral(const Expression& expression)
{
    return processExpressionIf(expression, !dynamic_cast<const LiteralExpression*>(&expression));
}


void Compiler::updateBinaryProcessingFlags(bool& processed, bool& processedFirst, bool& otherProcessedFirst) noexcept
{
    processed = true;

    if (!otherProcessedFirst)
        processedFirst = true;
}





std::string Compiler::getValueOfExpression(const Expression& expression)
{
    process(expression);

    return _registers.popLastRegisterFromBusy().name;
}





TypeSize Compiler::stringToTypeSize(const std::string& type)
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





void Compiler::throwIfAnyNullStatement(const std::vector<const Statement*>& statements)
{
    for (const Statement* const statement : statements)
        if (!statement)
            throw internal_e0000_nullptr();
}