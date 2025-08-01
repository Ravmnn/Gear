#include <compiler/compiler.hpp>

#include <algorithm>

#include <compiler/exceptions/exceptions.hpp>
#include <compiler/assembly/nasm.hpp>
#include <compiler/sizes.hpp>
#include <log.hpp>



const std::string Compiler::startLabelName = "_start";

const TypeSize Compiler::defaultTypeSize = TypeSize::Int32;
const ASMTypeSize Compiler::asmDefaultTypeSize = ASMTypeSize::DWord;



Compiler::Compiler(const std::vector<const Statement*>& statements, const BitMode bitMode, const std::string& entryPoint) noexcept
    : _statements(statements), _generalRegisters({
        Register{"rdi", ASMTypeSize::QWord}, Register{"edi", ASMTypeSize::DWord}, Register{"di", ASMTypeSize::Word}, Register{"dil", ASMTypeSize::Byte},
        Register{"rsi", ASMTypeSize::QWord}, Register{"esi", ASMTypeSize::DWord}, Register{"si", ASMTypeSize::Word}, Register{"sl", ASMTypeSize::Byte},
        Register{"rdx", ASMTypeSize::QWord}, Register{"edx", ASMTypeSize::DWord}, Register{"dx", ASMTypeSize::Word}, Register{"dl", ASMTypeSize::Byte},
        Register{"rcx", ASMTypeSize::QWord}, Register{"ecx", ASMTypeSize::DWord}, Register{"cx", ASMTypeSize::Word}, Register{"cl", ASMTypeSize::Byte}
    })
{
    _astPrinter.setIgnoreBlocks(true);
    _astPrinter.setNoNewlines(true);


    _currentExpressionDepth = 0;


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
    functionDeclarations();

    startLabel();

    finish();

    println(_asm.get());
}


void Compiler::functionDeclarations()
{
    for (const Statement* const statement : _statements)
    {
        const FunctionDeclarationStatement* function = dynamic_cast<const FunctionDeclarationStatement*>(statement);

        if (function)
            processFunctionDeclaration(*function);
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

    comment(_start, "syscall exit");
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





void Compiler::process(const Statement& statement)
{
    statement.process(*this);
}



void Compiler::processExpression(const ExpressionStatement& statement)
{
    process(*statement.expression);
}


void Compiler::processDeclaration(const DeclarationStatement& statement)
{

}


void Compiler::processFunctionDeclaration(const FunctionDeclarationStatement& statement)
{
    println("before:\n", _code.get());

    instantComment(_code, "this is a test");
    println("after:\n", _code.get());
    
    _code.label(statement.name.lexeme);
    _code.enableIndent();

    processBlock(*statement.body);

    _code.disableIndent();
}


void Compiler::processReturn(const ReturnStatement& statement)
{
    _code.instruction("ret");
}


void Compiler::processBlock(const BlockStatement& statement)
{
    for (const Statement* const statement : statement.statements)
        process(*statement);
}





void Compiler::process(const Expression& expression)
{
    if (_currentExpressionDepth == 0)
        instantComment(_code, _astPrinter.print(expression));

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

    const Register left = leftProcessedFirst ? popFirstRegisterFromBusy() : popLastRegisterFromBusy();
    const Register right = leftProcessedFirst ? popFirstRegisterFromBusy() : popLastRegisterFromBusy();

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

}





bool Compiler::isRegisterBusy(const Register& reg) const noexcept
{
    for (const Register& busyRegister : _busyRegisters)
        if (busyRegister.name == reg.name)
            return true;

    return false;
}



void Compiler::pushRegisterToBusy(const Register& reg)
{
    if (isRegisterBusy(reg))
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


Register Compiler::popFirstRegisterFromBusy()
{
    if (_busyRegisters.empty())
        throw internal_e0003();

    Register reg = _busyRegisters[0];
    _busyRegisters.erase(_busyRegisters.cbegin());

    return reg;
}



const Register& Compiler::getFirstFreeRegisterOfSize(const ASMTypeSize size)
{
    for (const Register& generalRegister : _generalRegisters)
        if (!isRegisterBusy(generalRegister) && generalRegister.size == size)
            return generalRegister;

    throw internal_e0005();
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