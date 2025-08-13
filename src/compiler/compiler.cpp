#include <compiler/compiler.hpp>

#include <algorithm>

#include <compiler/language/statement.hpp>
#include <compiler/language/expression.hpp>
#include <compiler/exceptions/exceptions.hpp>
#include <compiler/sizes.hpp>
#include <compiler/type_checker.hpp>
#include <torque.hpp>
#include <log.hpp>



// TODO: support 16 and 32 bit mode

// TODO: add support to function arguments
// TODO: add support to external functions
// TODO: add basic stdlib

// TODO: add support to negative numbers
// TODO: add support to float numbers
// TODO: detect overflows
// TODO: add support to multiply and division operation
// TODO: add support to strings
// TODO: add support to arrays



const std::string Compiler::startLabelName = "_start";

const std::string Compiler::stackPointerRegister = "rsp";
const std::string Compiler::stackFrameRegister = "rbp";


const std::stringstream* Compiler::assemblyAborted = nullptr;



Compiler::Compiler(const std::vector<const Statement*>& statements, const BitMode bitMode, const std::string& entryPoint) noexcept
    : _typeChecker(*this), _scopeLocal(_scope.local()), _scopeGlobal(_scope.global()), _statements(statements)
{
    _astPrinter.setIgnoreBlocks(true);
    _astPrinter.setNoNewlines(true);


    _current = nullptr;

    _currentExpressionDepth = 0;
    _currentStatementDepth = 0;

    _currentExpression = nullptr;
    _currentStatement = nullptr;

    _returnValue = false;


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
    
    try
    {
        code();
        startLabel();

        finish();
    }
    catch (const InternalException& exception)
    {
        assemblyAborted = new std::stringstream(_code.get());
        throw exception;
    }
}


void Compiler::code()
{
    _current = &_code;

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
    _current = &_start;

    if (!_scope.isIdentifierDefined(_entryPoint))
        throw internal_e0004(_entryPoint);

    _start.nasmDirectiveGlobal(startLabelName);
    _start.label(startLabelName);
    
    _start.enableIndent();


    comment("init stack frame");
    _start.instruction("mov", "rbp", "rsp");
    _start.newline();

    // TODO: when Torque support void functions, detect here
    call(_entryPoint);
    _start.newline();

    cast(getValue(), ASMTypeSize::QWord);
    Register& exitCode = consumeValue();
    
    _current->newline();

    instantComment("syscall exit");
    _start.syscallExit(exitCode.name());


    _start.disableIndent();
}


void Compiler::finish()
{
    _current = nullptr;

    _asm.nasmDirectiveBits(_bitMode);
    _asm.newline(2);



    _asm.nasmDirectiveSection(".data");
    _asm.newline();

    _asm.insertOther(_data);

    _asm.newline(2);



    _asm.nasmDirectiveSection(".text");
    _asm.newline();

    _asm.insertOther(_start);

    _asm.newline(3);


    
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





void Compiler::comment(const std::string& comment) noexcept
{
    if (_shouldComment)
        _current->comment(comment);
}

void Compiler::instantComment(const std::string& comment) noexcept
{
    if (_shouldComment)
        _current->instantComment(comment);
}



void Compiler::moveToFreeRegister(Register& reg, const std::string& data)
{
    _current->instruction("mov", reg.name(), data);
    _registers.pushRegisterToBusy(reg);
}


void Compiler::moveZeroExtendToFreeRegister(Register& reg, const std::string& data)
{
    _current->instruction("movzx", reg.name(), data);
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

    _current->instruction("sub", stackPointerRegister, sizeInBytesString);
    _current->instruction("mov", address, value);
}



void Compiler::call(const std::string& function)
{
    const Identifier* const identifier = _scope.getIdentifier(function);

    if (!identifier)
        throw internal_e0000_argument();

    const ASMTypeSize returnSize = identifier->size;

    // processing expression "identifier" is not necessary

    _current->instruction("call", function);

    // move the return value from AX to a general register immediately
    const Register& returnRegister = _registers.returnRegister().getRegisterOfSize(returnSize);
    moveToFirstFreeRegisterOfSize(returnSize, returnRegister.name());
}



void Compiler::cast(Register& value, const ASMTypeSize size)
{
    const unsigned int sourceSize = (unsigned int)value.size();
    const unsigned int targetSize = (unsigned int)size;

    Register& rightSizeSource = value.family()->getRegisterOfSize(size);
    Register& target = _registers.getFirstFreeRegisterOfSize(size);

    bool isDWordToQWord = sourceSize == (unsigned int)ASMTypeSize::DWord
                            && targetSize == (unsigned int)ASMTypeSize::QWord;

    // movzx doesn't work when casting 32-bit to a 64-bit value,
    // while mov does that automatically (only when in 64-bit mode)

    if (sourceSize < targetSize && !isDWordToQWord)
        moveZeroExtendToFreeRegister(target, value.name()); // cast up
    else
        moveToFreeRegister(target, rightSizeSource.name()); // cast down

    // the register storing the original value is now useless
    _registers.freeRegister(rightSizeSource);
}



void Compiler::stackFrameBegin() noexcept
{
    instantComment("stack frame begin");
    _current->instruction("push", stackFrameRegister);
    _current->instruction("mov", stackFrameRegister, stackPointerRegister);
    _current->instruction("add", stackFrameRegister, "16");
    _current->newline(2);
}


void Compiler::stackFrameEnd() noexcept
{
    _current->newline();
    instantComment("stack frame end");
    _current->instruction("sub", stackFrameRegister, "16");
    _current->instruction("mov", stackPointerRegister, stackFrameRegister);
    _current->instruction("pop", stackFrameRegister);
    _current->newline();
}



void Compiler::scopeBegin() noexcept
{
    _scopeLocal.scopeBegin();
}


void Compiler::scopeEnd() noexcept
{
    const std::string sizeInBytesString = std::to_string(_scopeLocal.sizeOfCurrentScopeInBytes());

    _scopeLocal.scopeEnd();

    _current->instruction("add", stackPointerRegister, sizeInBytesString);
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
    instantComment(_astPrinter.print(statement));

    const Statement* oldStatement = _currentStatement;
    _currentStatement = &statement;

    _typeChecker.process(statement);

    _currentStatementDepth++;
    statement.process(*this);
    _currentStatementDepth--;

    _currentStatement = oldStatement;

    _current->newline();

    // after a statement ends, all registers are supposed to be free.
    // note that only one register is supposed to be busy at the end of a statement.
    if (!_returnValue)
        _registers.freeAllBusyRegisters();

    _returnValue = false;
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
    const ASMTypeSize size = (ASMTypeSize)TypeChecker::stringToTypeSize(statement.type.lexeme);
    const Register& value = consumeValueOfExpression(*statement.value);

    allocateIdentifierOnStack(Identifier{ statement.name.lexeme, size, statement.name.position }, value.name());
}


void Compiler::processFunctionDeclaration(const FunctionDeclarationStatement& statement)
{
    // depth 1 is file scope

    if (_currentStatementDepth > 1)
        throw torque_e3003(statement.source().position);

    const IdentifierManager oldScope = _scopeLocal;
    

    _scopeLocal.clear(); // function scope cannot access another function's scope

    _current->label(statement.name.lexeme);
    _current->enableIndent();

    stackFrameBegin();
    processFunctionBlock(*statement.body); // don't use Compiler::processBlock because it creates a scope. Function scope is handled by stack frames.
    stackFrameEnd();

    _current->instruction("ret");

    _current->disableIndent();
    _current->newline(4);


    _scopeLocal = oldScope;
}


void Compiler::processReturn(const ReturnStatement& statement)
{
    const Register& value = consumeValueOfExpression(*statement.expression);
    const Register& returnRegister = _registers.returnRegister().getRegisterOfSize(value.size());

    _current->instruction("mov", returnRegister.name(), value.name());
}


void Compiler::processBlock(const BlockStatement& statement)
{
    instantComment("start");
    _current->newline();


    scopeBegin();
    process(statement.statements);
    scopeEnd();


    _current->newline();
    instantComment("end");
}



void Compiler::processFunctionBlock(const BlockStatement& statement)
{
    process(statement.statements);
}



void Compiler::defineFunctionIdentifierInGlobal(const FunctionDeclarationStatement& statement)
{
    const ASMTypeSize returnType = (ASMTypeSize)TypeChecker::stringToTypeSize(statement.returnType.lexeme);
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
        value = std::to_string((int)Token::stringToBoolean(value));

    moveToFirstFreeRegisterOfSize(Torque::sizes()->defaultAsmTypeSize(), value);
}


void Compiler::processBinary(const BinaryExpression& expression)
{
    bool leftProcessed = false;
    bool rightProcessed = false;

    bool leftProcessedFirst = false;
    bool rightProcessedFirst = false;

    processBinaryOperands(expression, leftProcessed, rightProcessed, leftProcessedFirst, rightProcessedFirst);

    Register* left = nullptr;
    Register* right = nullptr;

    if (leftProcessedFirst)
    {
        right = &consumeValue();
        left = &consumeValue();
    }
    else
    {
        left = &consumeValue();
        right = &consumeValue();
    }

    // the result of a binary operation is stored in the left register,
    // so it's needed to re-occupy it after consuming (freeing) it to avoid
    // the loss of the result

    _registers.pushRegisterToBusy(*left);

    switch (expression.op.type)
    {
    case TokenType::Plus:
        _current->instruction("add", left->name(), right->name());
        break;

    case TokenType::Minus:
        _current->instruction("sub", left->name(), right->name());
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
    const ASMTypeSize registerSize = isInstructionAddress ? (ASMTypeSize)TypeSize::UInt64 : identifier->size;

    comment(identifier->name);
    moveToFirstFreeRegisterOfSize(registerSize, value);
}


void Compiler::processCall(const CallExpression& expression)
{
    // TODO: choose if torque will accept first class functions.

    const IdentifierExpression* const identifier = dynamic_cast<const IdentifierExpression*>(expression.callee);

    if (!identifier)
        throw internal_e0000();

    call(identifier->identifier.lexeme);

    _returnValue = true;
}


void Compiler::processCast(const CastExpression& expression)
{
    Register& value = getValueOfExpression(*expression.expression);
    const ASMTypeSize newSize = (ASMTypeSize)TypeChecker::stringToTypeSize(expression.type.lexeme);

    cast(value, newSize);
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





Register& Compiler::consumeValueOfExpression(const Expression& expression)
{
    process(expression);

    return consumeValue();
}


Register& Compiler::consumeValue()
{
    return _registers.popLastBusyRegisterFromBusy();
}



Register& Compiler::getValueOfExpression(const Expression& expression)
{
    process(expression);

    return getValue();
}


Register& Compiler::getValue()
{
    return _registers.getLastBusyRegisterFromBusy();
}





void Compiler::throwIfAnyNullStatement(const std::vector<const Statement*>& statements)
{
    for (const Statement* const statement : statements)
        if (!statement)
            throw internal_e0000_nullptr();
}