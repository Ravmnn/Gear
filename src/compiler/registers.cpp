#include <compiler/registers.hpp>

#include <compiler/sizes.hpp>
#include <compiler/exceptions/exceptions.hpp>



RegisterManager::RegisterManager()
{
    _generalRegisters = {
        Register{"rdi", ASMTypeSize::QWord}, Register{"edi", ASMTypeSize::DWord}, Register{"di", ASMTypeSize::Word}, Register{"dil", ASMTypeSize::Byte},
        Register{"rsi", ASMTypeSize::QWord}, Register{"esi", ASMTypeSize::DWord}, Register{"si", ASMTypeSize::Word}, Register{"sl", ASMTypeSize::Byte},
        Register{"rdx", ASMTypeSize::QWord}, Register{"edx", ASMTypeSize::DWord}, Register{"dx", ASMTypeSize::Word}, Register{"dl", ASMTypeSize::Byte},
        Register{"rcx", ASMTypeSize::QWord}, Register{"ecx", ASMTypeSize::DWord}, Register{"cx", ASMTypeSize::Word}, Register{"cl", ASMTypeSize::Byte},
        Register{"r8", ASMTypeSize::QWord}, Register{"r8d", ASMTypeSize::DWord}, Register{"r8w", ASMTypeSize::Word}, Register{"r8b", ASMTypeSize::Byte},
        Register{"r9", ASMTypeSize::QWord}, Register{"r9d", ASMTypeSize::DWord}, Register{"r9w", ASMTypeSize::Word}, Register{"r9b", ASMTypeSize::Byte},
        Register{"r10", ASMTypeSize::QWord}, Register{"r10d", ASMTypeSize::DWord}, Register{"r10w", ASMTypeSize::Word}, Register{"r10b", ASMTypeSize::Byte},
        Register{"r11", ASMTypeSize::QWord}, Register{"r11d", ASMTypeSize::DWord}, Register{"r11w", ASMTypeSize::Word}, Register{"r11b", ASMTypeSize::Byte}
    };
}



const std::array<Register, 32>& RegisterManager::generalRegisters() const noexcept
{
    return _generalRegisters;
}


const std::vector<Register>& RegisterManager::busyRegisters() const noexcept
{
    return _busyRegisters;
}



bool RegisterManager::isRegisterBusy(const std::string& reg) const noexcept
{
    for (const Register& busyRegister : _busyRegisters)
        if (busyRegister.name == reg)
            return true;

    return false;
}



void RegisterManager::pushRegisterToBusy(const Register& reg)
{
    if (isRegisterBusy(reg.name))
        throw internal_e0000_argument();

    _busyRegisters.push_back(reg);  
}


const Register& RegisterManager::popLastRegisterFromBusy()
{
    if (_busyRegisters.empty())
        throw internal_e0000();

    const Register& reg = _busyRegisters[_busyRegisters.size() - 1];
    _busyRegisters.pop_back();

    return reg;
}



const Register& RegisterManager::getFirstFreeRegisterOfSize(const ASMTypeSize size)
{
    for (const Register& generalRegister : _generalRegisters)
        if (!isRegisterBusy(generalRegister.name) && generalRegister.size == size)
            return generalRegister;

    throw internal_e0000();
}



void RegisterManager::freeAllBusyRegisters() noexcept
{
    _busyRegisters.clear();
}
