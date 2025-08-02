#include <compiler/compiler.hpp>

#include <algorithm>

#include <compiler/language/statement.hpp>
#include <compiler/language/expression.hpp>
#include <compiler/exceptions/exceptions.hpp>
#include <compiler/assembly/nasm.hpp>
#include <compiler/sizes.hpp>
#include <log.hpp>



// TODO: add support to variables
// TODO: scope, including correct stack frame handling

// TODO: add support to function calling
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



Compiler::Compiler(const std::vector<const Statement*>& statements, const BitMode bitMode, const std::string& entryPoint) noexcept
    : _statements(statements), _generalRegisters({
        Register{"rdi", ASMTypeSize::QWord}, Register{"edi", ASMTypeSize::DWord}, Register{"di", ASMTypeSize::Word}, Register{"dil", ASMTypeSize::Byte},
        Register{"rsi", ASMTypeSize::QWord}, Register{"esi", ASMTypeSize::DWord}, Register{"si", ASMTypeSize::Word}, Register{"sl", ASMTypeSize::Byte},
        Register{"rdx", ASMTypeSize::QWord}, Register{"edx", ASMTypeSize::DWord}, Register{"dx", ASMTypeSize::Word}, Register{"dl", ASMTypeSize::Byte},
        Register{"rcx", ASMTypeSize::QWord}, Register{"ecx", ASMTypeSize::DWord}, Register{"cx", ASMTypeSize::Word}, Register{"cl", ASMTypeSize::Byte},
        Register{"r8", ASMTypeSize::QWord}, Register{"r8d", ASMTypeSize::DWord}, Register{"r8w", ASMTypeSize::Word}, Register{"r8b", ASMTypeSize::Byte},
        Register{"r9", ASMTypeSize::QWord}, Register{"r9d", ASMTypeSize::DWord}, Register{"r9w", ASMTypeSize::Word}, Register{"r9b", ASMTypeSize::Byte},
        Register{"r10", ASMTypeSize::QWord}, Register{"r10d", ASMTypeSize::DWord}, Register{"r10w", ASMTypeSize::Word}, Register{"r10b", ASMTypeSize::Byte},
        Register{"r11", ASMTypeSize::QWord}, Register{"r11d", ASMTypeSize::DWord}, Register{"r11w", ASMTypeSize::Word}, Register{"r11b", ASMTypeSize::Byte}
    })
{
    _astPrinter.setIgnoreBlocks(true);
    _astPrinter.setNoNewlines(true);


    _currentExpressionDepth = 0;
    _currentStatementDepth = 0;


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




void Compiler::compile()
{
    throwIfAnyNullStatement(_statements);

    functionDeclarations();

    startLabel();

    finish();
}


void Compiler::functionDeclarations()
{
    for (const Statement* const statement : _statements)
    {
        const FunctionDeclarationStatement* function = dynamic_cast<const FunctionDeclarationStatement*>(statement);

        // TODO: change to "process" and use instantNewline only in "process" (statement)

        if (function)
            processFunctionDeclaration(*function);
        else
            throw gear_e3000(statement->source().position);
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





void Compiler::throwIfAnyNullStatement(const std::vector<const Statement*>& statements) const
{
    for (const Statement* const statement : statements)
        if (!statement)
            throw internal_e0002();
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

    pushRegisterToBusy(reg);
}


void Compiler::moveToFirstFreeRegisterOfSize(const ASMTypeSize size, const std::string& data)
{
    moveToFreeRegister(getFirstFreeRegisterOfSize(size), data);
}



void Compiler::allocateIdentifierOnStack(const Identifier& identifier, const std::string& value)
{
    defineIdentifier(identifier);

    const std::string sizeInBytesString = std::to_string((unsigned int)identifier.size);
    const std::string address = addressOfIdentifierOnStack(identifier);

    _code.instruction("sub", stackPointerRegister, sizeInBytesString);
    _code.instruction("mov", address, value);
}



void Compiler::stackFrameBegin() noexcept
{
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





unsigned int Compiler::addressDisplacementOfIdentifierOnStack(const Identifier& identifier) const
{
    // TODO: exceptions that repeat a lot should be simplified in exceptions.hpp by creating a function

    if (!isIdentifierDefined(identifier.name))
        throw gear_e3001(identifier.position);

    unsigned int offsetInBytes = 0;

    for (int i = _identifiers.size() - 1; i >= 0; i--)
    {
        const Identifier& currentIdentifier = _identifiers[i];

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

    _currentStatementDepth++;
    statement.process(*this);
    _currentStatementDepth--;

    _code.newline();

    freeAllBusyRegisters();
}



void Compiler::processExpression(const ExpressionStatement& statement)
{
    process(*statement.expression);
}


void Compiler::processDeclaration(const DeclarationStatement& statement)
{
    const ASMTypeSize size = (ASMTypeSize)stringToTypeSize(statement.type.lexeme);
    
    process(*statement.value);
    const std::string value = popLastRegisterFromBusy().name;

    allocateIdentifierOnStack(Identifier{ statement.name.lexeme, size, statement.name.position }, value);
}


void Compiler::processFunctionDeclaration(const FunctionDeclarationStatement& statement)
{
    if (_currentStatementDepth >= 1)
        throw gear_e3003(statement.source().position);

    _code.label(statement.name.lexeme);
    _code.enableIndent();

    stackFrameBegin();
    processBlock(*statement.body);
    stackFrameEnd();

    _code.instruction("ret");

    _code.disableIndent();
}


void Compiler::processReturn(const ReturnStatement& statement)
{
    //_code.instruction("ret");
}


void Compiler::processBlock(const BlockStatement& statement)
{
    for (const Statement* const statement : statement.statements)
        process(*statement);
}





void Compiler::process(const Expression& expression)
{
    _currentExpressionDepth++;
    expression.process(*this);
    _currentExpressionDepth--;
}



void Compiler::processLiteral(const LiteralExpression& expression)
{
    moveToFirstFreeRegisterOfSize(asmDefaultTypeSize, expression.value.lexeme);
}


void Compiler::processBinary(const BinaryExpression& expression)
{
    // TODO: clear this
    // TODO: registers are not infinite: Or allocate at the stack if no register is available or throw an exception

    bool leftProcessed = false;
    bool rightProcessed = false;

    bool leftProcessedFirst = false;
    bool rightProcessedFirst = false;

    if (!dynamic_cast<const LiteralExpression*>(expression.left))
    {
        process(*expression.left);
        leftProcessed = true;

        if (!rightProcessedFirst)
            leftProcessedFirst = true;
    }
    
    if (!dynamic_cast<const LiteralExpression*>(expression.right))
    {
        process(*expression.right);
        rightProcessed = true;

        if (!leftProcessedFirst)
            rightProcessedFirst = true;
    }

    if (!leftProcessed)
    {
        process(*expression.left);

        if (!rightProcessedFirst)
            leftProcessedFirst = true;
    }

    if (!rightProcessed)
    {
        process(*expression.right);

        if (!leftProcessedFirst)
            rightProcessedFirst = true;
    }

    Register right = popLastRegisterFromBusy();
    Register left = popLastRegisterFromBusy();

    if (rightProcessedFirst)
        std::swap(left, right);

    // the result of a binary operation is stored in the left register,
    // so it's needed to re-occupy it after consuming (freeing) it to avoid
    // the loss of the result

    pushRegisterToBusy(left);

    switch (expression.op.type)
    {
    case TokenType::Plus:
        _code.instruction("add", left.name, right.name);
        break;

    case TokenType::Minus:
        _code.instruction("sub", left.name, right.name);
        break;

    default:
        throw internal_e0001();
    }
}


void Compiler::processGrouping(const GroupingExpression& expression)
{
    process(*expression.expression);
}


void Compiler::processIdentifier(const IdentifierExpression& expression)
{
    const Identifier identifier = getIdentifier(expression.identifier);
    const std::string value = addressOfIdentifierOnStack(identifier);

    moveToFirstFreeRegisterOfSize(identifier.size, value);
}





bool Compiler::isRegisterBusy(const std::string& reg) const noexcept
{
    for (const Register& busyRegister : _busyRegisters)
        if (busyRegister.name == reg)
            return true;

    return false;
}



void Compiler::pushRegisterToBusy(const Register& reg)
{
    if (isRegisterBusy(reg.name))
        throw internal_e0004();

    _busyRegisters.push_back(reg);  
}


Register Compiler::popLastRegisterFromBusy()
{
    if (_busyRegisters.empty())
        throw internal_e0003();

    Register reg = _busyRegisters[_busyRegisters.size() - 1];
    _busyRegisters.pop_back();

    return reg;
}



const Register& Compiler::getFirstFreeRegisterOfSize(const ASMTypeSize size)
{
    for (const Register& generalRegister : _generalRegisters)
        if (!isRegisterBusy(generalRegister.name) && generalRegister.size == size)
            return generalRegister;

    throw internal_e0005();
}



void Compiler::freeAllBusyRegisters() noexcept
{
    _busyRegisters.clear();
}





bool Compiler::isIdentifierDefined(const std::string& identifier) const noexcept
{
    for (const Identifier& definedIdentifier : _identifiers)
        if (definedIdentifier.name == identifier)
            return true;

    return false;
}



void Compiler::defineIdentifier(const Identifier& identifier)
{
    if (isIdentifierDefined(identifier.name))
        throw gear_e3002(identifier.position);

    _identifiers.push_back(identifier);
}


Identifier Compiler::getIdentifier(const Token& identifier) const
{
    for (const Identifier& declaredIdentifier : _identifiers)
        if (declaredIdentifier.name == identifier.lexeme)
            return declaredIdentifier;

    throw gear_e3001(identifier.position);
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

    throw internal_e0000();
}