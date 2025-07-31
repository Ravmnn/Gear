#include <compiler/compiler.hpp>

#include <algorithm>

#include <compiler/exceptions/exceptions.hpp>
#include <compiler/assembly/nasm.hpp>
#include <compiler/sizes.hpp>



const std::string Compiler::startLabelName = "_start";

const TypeSize Compiler::defaultTypeSize = TypeSize::Int32;
const ASMTypeSize Compiler::asmDefaultTypeSize = ASMTypeSize::DWord;



Compiler::Compiler(const std::vector<Statement*>& statements, const BitMode bitMode, const std::string& entryPoint) noexcept
    : _statements(statements), _generalRegisters({
        Register{"rdi", ASMTypeSize::QWord}, Register{"edi", ASMTypeSize::DWord}, Register{"di", ASMTypeSize::Word}, Register{"dil", ASMTypeSize::Byte},
        Register{"rsi", ASMTypeSize::QWord}, Register{"esi", ASMTypeSize::DWord}, Register{"si", ASMTypeSize::Word}, Register{"sl", ASMTypeSize::Byte},
        Register{"rdx", ASMTypeSize::QWord}, Register{"edx", ASMTypeSize::DWord}, Register{"dx", ASMTypeSize::Word}, Register{"dl", ASMTypeSize::Byte},
        Register{"rcx", ASMTypeSize::QWord}, Register{"ecx", ASMTypeSize::DWord}, Register{"cx", ASMTypeSize::Word}, Register{"cl", ASMTypeSize::Byte}
    })
{
    _bitMode = bitMode;
    _entryPoint = entryPoint;
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




void Compiler::setBitmode(const BitMode bitMode) noexcept
{
    _bitMode = bitMode;
}


void Compiler::setEntryPoint(const std::string& entryPoint) noexcept
{
    _entryPoint = entryPoint;
}




void Compiler::compile()
{
    functionDeclarations();

    startLabel();

    finish();
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


    _start.instruction("mov", "rbp", "rsp");
    _start.newline();

    _start.instruction("call", _entryPoint);
    _start.newline();

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


    
    _asm.insertOther(_funcs);
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
    _funcs.label(statement.name.lexeme);
    _funcs.enableIndent();

    processBlock(*statement.body);

    _funcs.disableIndent();
}


void Compiler::processReturn(const ReturnStatement& statement)
{
    _funcs.instruction("ret");
}


void Compiler::processBlock(const BlockStatement& statement)
{
    for (const Statement* const statement : statement.statements)
        process(*statement);
}




void Compiler::process(const Expression& expression)
{
    expression.process(*this);
}



void Compiler::processLiteral(const LiteralExpression& expression)
{
    moveToFirstFreeRegisterOfSize(asmDefaultTypeSize, expression.value.lexeme);
}


void Compiler::processBinary(const BinaryExpression& expression)
{
    process(*expression.left);
    process(*expression.right);

    // consume at reverse order

    Register& right = popRegisterFromBusy();
    Register& left = popRegisterFromBusy();

    // the result of a binary operation is stored at the left register,
    // so it's needed to re-occupy it after consuming (freeing) to avoid
    // the loss of the result
    
    pushRegisterToBusy(left);

    switch (expression.op.type)
    {
    case TokenType::Plus:
        _funcs.instruction("add", left.name, right.name);
        break;

    case TokenType::Minus:
        _funcs.instruction("sub", left.name, right.name);
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





void Compiler::moveToFreeRegister(Register& reg, const std::string& data)
{
    _funcs.instruction("mov", reg.name, data);

    pushRegisterToBusy(reg);
}


void Compiler::moveToFirstFreeRegisterOfSize(const ASMTypeSize size, const std::string& data)
{
    moveToFreeRegister(getFirstFreeRegisterOfSize(size), data);
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


Register& Compiler::popRegisterFromBusy()
{
    if (_busyRegisters.empty())
        throw internal_e0003();

    Register& reg = *(_busyRegisters.end() - 1);
    _busyRegisters.pop_back();

    return reg;
}



Register& Compiler::getFirstFreeRegisterOfSize(const ASMTypeSize size)
{
    for (Register& generalRegister : _generalRegisters)
        if (!isRegisterBusy(generalRegister) && generalRegister.size == size)
            return generalRegister;
}


// Register& Compiler::getLastBusyRegisterOfSize(const ASMTypeSize size)
// {
//     for (Register& generalRegister : _busyRegisters)
//         if (isRegisterBusy(generalRegister) && generalRegister.size == size)
//             return generalRegister;
// }



// Register& Compiler::consumeLastBusyRegisterOfSize(const ASMTypeSize size)
// {
//     Register& reg = getLastBusyRegisterOfSize(size);
//     _busyRegisters.pop_back();

//     return reg;
// }





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