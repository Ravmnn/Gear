#pragma once

#include <string>
#include <vector>
#include <array>



enum class ASMTypeSize;


struct Register
{
    std::string name;
    ASMTypeSize size;
};


class RegisterManager
{
private:
    std::array<Register, 32> _generalRegisters;
    std::vector<Register> _busyRegisters;


public:
    RegisterManager();


    const std::array<Register, 32>& generalRegisters() const noexcept;
    const std::vector<Register>& busyRegisters() const noexcept;


    bool isRegisterBusy(const std::string& reg) const noexcept;

    void pushRegisterToBusy(const Register& reg);
    const Register& popLastRegisterFromBusy();

    const Register& getFirstFreeRegisterOfSize(ASMTypeSize size);


    void freeAllBusyRegisters() noexcept;
};