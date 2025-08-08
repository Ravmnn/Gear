#include <compiler/registers.hpp>

#include <sstream>

#include <compiler/sizes.hpp>
#include <compiler/exceptions/exceptions.hpp>



RegisterManager::RegisterManager()
{
    _generalRegisters = {
        Register{"rdi", ASMTypeSize::QWord, 1}, Register{"edi", ASMTypeSize::DWord, 1}, Register{"di", ASMTypeSize::Word, 1}, Register{"dil", ASMTypeSize::Byte, 1},
        Register{"rsi", ASMTypeSize::QWord, 2}, Register{"esi", ASMTypeSize::DWord, 2}, Register{"si", ASMTypeSize::Word, 2}, Register{"sl", ASMTypeSize::Byte, 2},
        Register{"rdx", ASMTypeSize::QWord, 3}, Register{"edx", ASMTypeSize::DWord, 3}, Register{"dx", ASMTypeSize::Word, 3}, Register{"dl", ASMTypeSize::Byte, 3},
        Register{"rcx", ASMTypeSize::QWord, 4}, Register{"ecx", ASMTypeSize::DWord, 4}, Register{"cx", ASMTypeSize::Word, 4}, Register{"cl", ASMTypeSize::Byte, 4},
        Register{"r8", ASMTypeSize::QWord, 5}, Register{"r8d", ASMTypeSize::DWord, 5}, Register{"r8w", ASMTypeSize::Word, 5}, Register{"r8b", ASMTypeSize::Byte, 5},
        Register{"r9", ASMTypeSize::QWord, 6}, Register{"r9d", ASMTypeSize::DWord, 6}, Register{"r9w", ASMTypeSize::Word, 6}, Register{"r9b", ASMTypeSize::Byte, 6},
        Register{"r10", ASMTypeSize::QWord, 7}, Register{"r10d", ASMTypeSize::DWord, 7}, Register{"r10w", ASMTypeSize::Word, 7}, Register{"r10b", ASMTypeSize::Byte, 7},
        Register{"r11", ASMTypeSize::QWord, 8}, Register{"r11d", ASMTypeSize::DWord, 8}, Register{"r11w", ASMTypeSize::Word, 8}, Register{"r11b", ASMTypeSize::Byte, 8}
    };

    _returnRegister = {
        Register{"rax", ASMTypeSize::QWord, 9, true},
        Register{"eax", ASMTypeSize::DWord, 9, true},
        Register{"ax", ASMTypeSize::Word, 9, true},
        Register{"al", ASMTypeSize::Byte, 9, true}
    };

    _prioritizeReturnRegister = false;
}



const std::array<Register, 32>& RegisterManager::generalRegisters() const noexcept
{
    return _generalRegisters;
}


const std::array<Register, 4>& RegisterManager::returnRegister() const noexcept
{
    return _returnRegister;
}


const std::vector<Register>& RegisterManager::busyRegisters() const noexcept
{
    return _busyRegisters;
}



void RegisterManager::prioritizeReturnRegister() noexcept
{
    _prioritizeReturnRegister = true;
}



bool RegisterManager::isRegisterBusy(const std::string& reg) const noexcept
{
    for (const Register& generalRegister : _generalRegisters)
        if (generalRegister.name == reg)
            return isRegisterOfFamilyBusy(generalRegister.family);

    return false;
}


bool RegisterManager::isRegisterOfFamilyBusy(unsigned int family) const noexcept
{
    for (const Register& busyRegister : _busyRegisters)
        if (busyRegister.family == family)
            return true;

    return false;
}





std::string RegisterManager::print() const noexcept
{
    std::stringstream stream;

    for (const Register& busyRegister : _busyRegisters)
        stream << std::boolalpha << busyRegister.name << " - " << busyRegister.ignoreAutomaticFree << std::endl;

    return stream.str();
}





void RegisterManager::pushRegisterToBusy(const Register& reg)
{
    if (isRegisterBusy(reg.name))
        throw internal_e0000_argument();

    _busyRegisters.push_back(reg);  
}



const Register& RegisterManager::popRegisterAtIndex(const size_t index)
{
    if (_busyRegisters.empty())
        throw internal_e0000();

    if (index >= _busyRegisters.size())
        throw internal_e0000_argument();

    const Register& reg = _busyRegisters[index];
    _busyRegisters.erase(_busyRegisters.cbegin() + index);

    return reg;
}


const Register& RegisterManager::popLastRegisterFromBusy()
{
    return popRegisterAtIndex(_busyRegisters.size() - 1);
}


const Register& RegisterManager::popRegisterOfFamily(unsigned int family)
{
    for (size_t i = 0; i < _busyRegisters.size(); i++)
        if (_busyRegisters[i].family == family)
            return popRegisterAtIndex(i);

    throw internal_e0000_argument();
}



void RegisterManager::freeAllBusyRegisters() noexcept
{
    for (size_t i = 0; i < _busyRegisters.size(); i++)
    {
        const Register& reg = _busyRegisters[i];

        if (!reg.ignoreAutomaticFree)
            _busyRegisters.erase(_busyRegisters.cbegin() + i);
    }
}



std::vector<Register> RegisterManager::getGeneralRegistersOfFamily(unsigned int family) const noexcept
{
    std::vector<Register> registers;

    for (const Register& reg : _generalRegisters)
        if (reg.family == family)
            registers.push_back(reg);

    return registers;
}


const Register& RegisterManager::getFirstFreeRegisterOfSize(const ASMTypeSize size)
{
    // TODO: terrible approach, improve

    if (_prioritizeReturnRegister)
    {
        _prioritizeReturnRegister = false;

        for (const Register& reg : _returnRegister)
            if (!isRegisterBusy(reg.name) && reg.size == size)
                return reg;
    }

    else
        for (const Register& generalRegister : _generalRegisters)
            if (!isRegisterBusy(generalRegister.name) && generalRegister.size == size)
                return generalRegister;
    
    throw internal_e0000();
}


const Register& RegisterManager::getReturnRegisterOfSize(ASMTypeSize size)
{
    for (const Register& reg : _returnRegister)
        if (reg.size == size)
            return reg;

    throw internal_e0000();
}