#include <compiler/registers.hpp>

#include <sstream>

#include <compiler/sizes.hpp>
#include <compiler/exceptions/exceptions.hpp>



Register::Register()
{
    _size = ASMTypeSize::Byte;
    _family = nullptr;
    _manualFree = false;
}


Register::Register(const std::string& name, const ASMTypeSize size, RegisterFamily* const family, const bool manualFree)
{
    if (!family)
        throw internal_e0000_nullptr();

    _name = name;
    _size = size;
    _family = family;
    _manualFree = manualFree;
}


const std::string& Register::name() const noexcept
{
    return _name;
}


ASMTypeSize Register::size() const noexcept
{
    return _size;
}


RegisterFamily* Register::family() noexcept
{
    return _family;
}


bool Register::manualFree() const noexcept
{
    return _manualFree;
}



bool Register::isBusy() const noexcept
{
    return _family->isBusy();
}



ASMTypeSize RegisterFamily::sizeIndexToASMTypeSize(const SizeIndex index)
{
    switch (index)
    {
    case SizeIndex::Byte: return ASMTypeSize::Byte;
    case SizeIndex::Word: return ASMTypeSize::Word;
    case SizeIndex::DWord: return ASMTypeSize::DWord;
    case SizeIndex::QWord: return ASMTypeSize::QWord;

    default: throw internal_e0000_argument();
    }
}


RegisterFamily::SizeIndex RegisterFamily::asmTypeSizeToSizeIndex(const ASMTypeSize size)
{
    switch (size)
    {
    case ASMTypeSize::Byte: return SizeIndex::Byte;
    case ASMTypeSize::Word: return SizeIndex::Word;
    case ASMTypeSize::DWord: return SizeIndex::DWord;
    case ASMTypeSize::QWord: return SizeIndex::QWord;

    default: throw internal_e0000_argument();
    }
}


RegisterFamily::RegisterFamily(const std::initializer_list<std::string>& registersName, const unsigned int familyId, const bool manualFree)
    : _familyId(familyId), _manualFree(manualFree)
{
    _busyRegister = nullptr;

    if (registersName.size() != 4)
        throw internal_e0000_argument();

    for (size_t i = 0; i < registersName.size(); i++)
    {
        const std::string name = *(registersName.begin() + i);
        const ASMTypeSize size = sizeIndexToASMTypeSize((SizeIndex)i);

        _registers[i] = Register{ name, size, this, manualFree };
    }
}


unsigned int RegisterFamily::familyId() const noexcept
{
    return _familyId;
}


const std::array<Register, 4>& RegisterFamily::registers() const noexcept
{
    return _registers;
}


Register* RegisterFamily::busyRegister() noexcept
{
    return _busyRegister;
}


bool RegisterFamily::manualFree() const noexcept
{
    return _manualFree;
}



void RegisterFamily::setBusyRegister(Register* busyRegister) noexcept
{
    _busyRegister = busyRegister;
}



bool RegisterFamily::isBusy() const noexcept
{
    return _busyRegister != nullptr;
}


bool RegisterFamily::hasRegister(const std::string& registerName) const noexcept
{
    for (const Register& reg : _registers)
        if (reg.name() == registerName)
            return true;

    return false;
}



Register& RegisterFamily::getRegisterOfSize(const ASMTypeSize size) noexcept
{
    const SizeIndex index = asmTypeSizeToSizeIndex(size);

    return _registers[index];
}


Register& RegisterFamily::getRegisterOfName(const std::string& name)
{
    for (Register& reg : _registers)
        if (reg.name() == name)
            return reg;

    throw internal_e0000_argument();
}



RegisterManager::RegisterManager()
    : _generalRegisters({
        RegisterFamily({ "rdi", "edi", "di", "dil" }, RegisterFamily::DI),
        RegisterFamily({ "rsi", "esi", "si", "sl" }, RegisterFamily::SI),
        RegisterFamily({ "rdx", "edx", "dx", "dl" }, RegisterFamily::DX),
        RegisterFamily({ "rcx", "ecx", "cx", "cl" }, RegisterFamily::CX),
        RegisterFamily({ "r8", "r8d", "r8w", "r8b" }, RegisterFamily::R8),
        RegisterFamily({ "r9", "r9d", "r9w", "r9b" }, RegisterFamily::R9),
        RegisterFamily({ "r10", "r10d", "r10w", "r10b" }, RegisterFamily::R10),
        RegisterFamily({ "r11", "r11d", "r11w", "r11b" }, RegisterFamily::R11),
    }), _returnRegister(RegisterFamily({ "rax", "eax", "ax", "al" }, RegisterFamily::AX))
{}



const std::array<RegisterFamily, 8>& RegisterManager::generalRegisters() const noexcept
{
    return _generalRegisters;
}


const std::vector<RegisterFamily*>& RegisterManager::busyRegisters() const noexcept
{
    return _busyRegisters;
}


RegisterFamily& RegisterManager::returnRegister() noexcept
{
    return _returnRegister;
}





std::string RegisterManager::print() const noexcept
{
    std::stringstream stream;

    // for (const RegisterFamily& busyRegister : _busyRegisters)
    //     stream << std::boolalpha << busyRegister.name << " - " << busyRegister.manualFree << std::endl;

    return stream.str();
}





void RegisterManager::pushRegisterToBusy(Register& reg)
{
    if (reg.family()->isBusy())
        throw internal_e0000_argument();

    RegisterFamily* const registerFamily = reg.family();
    registerFamily->setBusyRegister(&reg);

    _busyRegisters.push_back(reg.family());
}



Register& RegisterManager::popRegisterAtIndex(const size_t index)
{
    if (_busyRegisters.empty())
        throw internal_e0000();

    if (index >= _busyRegisters.size())
        throw internal_e0000_argument();

    RegisterFamily& registerFamily = *_busyRegisters[index];
    Register& busyRegister = *registerFamily.busyRegister();

    registerFamily.setBusyRegister(nullptr);

    _busyRegisters.erase(_busyRegisters.cbegin() + index);

    return busyRegister;
}


Register& RegisterManager::popLastRegisterFromBusy()
{
    return popRegisterAtIndex(_busyRegisters.size() - 1);
}


Register& RegisterManager::popRegisterOfFamily(unsigned int familyId)
{
    for (size_t i = 0; i < _busyRegisters.size(); i++)
        if (_busyRegisters[i]->familyId() == familyId)
            return popRegisterAtIndex(i);

    throw internal_e0000_argument();
}



void RegisterManager::freeAllBusyRegisters() noexcept
{
    for (size_t i = 0; i < _busyRegisters.size(); i++)
    {
        const RegisterFamily& registerFamily = *_busyRegisters[i];

        if (!registerFamily.manualFree())
            popRegisterAtIndex(i);
    }
}



Register& RegisterManager::getFirstFreeRegisterOfSize(const ASMTypeSize size)
{
    for (RegisterFamily& generalRegister : _generalRegisters)
        if (!generalRegister.isBusy())
            return generalRegister.getRegisterOfSize(size);

    throw internal_e0000();
}


Register& RegisterManager::getRegisterOfName(const std::string& name)
{
    for (RegisterFamily& registerFamily : _generalRegisters)
        if (registerFamily.hasRegister(name))
            return registerFamily.getRegisterOfName(name);

    throw internal_e0000_argument();
}


RegisterFamily& RegisterManager::getRegisterFamilyOfId(unsigned int familyId)
{
    for (RegisterFamily& registerFamily : _generalRegisters)
        if (registerFamily.familyId() == familyId)
            return registerFamily;
            
    throw internal_e0000_argument();
}