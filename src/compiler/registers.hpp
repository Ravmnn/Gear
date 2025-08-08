#pragma once

#include <string>
#include <vector>
#include <array>



enum class ASMTypeSize;


struct Register
{
    std::string name;
    ASMTypeSize size;

    unsigned int family;

    bool ignoreAutomaticFree = false;
};


class RegisterManager
{
private:
    std::array<Register, 32> _generalRegisters;
    std::array<Register, 4> _returnRegister;

    std::vector<Register> _busyRegisters;

    bool _prioritizeReturnRegister;


public:
    RegisterManager();


    const std::array<Register, 32>& generalRegisters() const noexcept;
    const std::array<Register, 4>& returnRegister() const noexcept;
    const std::vector<Register>& busyRegisters() const noexcept;


    void prioritizeReturnRegister() noexcept;


    bool isRegisterBusy(const std::string& reg) const noexcept;
    bool isRegisterOfFamilyBusy(unsigned int family) const noexcept;


    std::string print() const noexcept;


    void pushRegisterToBusy(const Register& reg);

    const Register& popRegisterAtIndex(size_t index);
    const Register& popLastRegisterFromBusy();
    const Register& popRegisterOfFamily(unsigned int family);

    void freeAllBusyRegisters() noexcept;


    std::vector<Register> getGeneralRegistersOfFamily(unsigned int family) const noexcept;

    const Register& getFirstFreeRegisterOfSize(ASMTypeSize size);
    const Register& getReturnRegisterOfSize(ASMTypeSize size);
};