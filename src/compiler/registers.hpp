#pragma once

#include <string>
#include <vector>
#include <array>



enum class ASMTypeSize;

class RegisterFamily;


class Register
{
private:
    std::string _name;
    ASMTypeSize _size;

    RegisterFamily* _family; // this should never be a nullptr

    bool _manualFree;


    friend class RegisterFamily;


    Register(const std::string& name, ASMTypeSize size, RegisterFamily* family, bool manualFree = false);


public:
    Register();


    const std::string& name() const noexcept;
    ASMTypeSize size() const noexcept;
    RegisterFamily* family() noexcept;
    bool manualFree() const noexcept;


    bool isBusy() const noexcept;
};


class RegisterFamily
{
private:
    unsigned int _familyId;

    std::array<Register, 4> _registers;
    Register* _busyRegister;

    bool _manualFree;


public:
    enum Family
    {
        None,

        AX,

        DI,
        SI,
        DX,
        CX,
        R8,
        R9,
        R10,
        R11
    };


    enum SizeIndex
    {
        QWord, // register at the index 0 has the size QWord...
        DWord,
        Word,
        Byte
    };


    RegisterFamily(const std::initializer_list<std::string>& registersName, unsigned int familyId, bool manualFree = false);


    unsigned int familyId() const noexcept;
    const std::array<Register, 4>& registers() const noexcept;
    Register* busyRegister() noexcept;
    bool manualFree() const noexcept;


    void setBusyRegister(Register* busyRegister) noexcept;


    bool isBusy() const noexcept;

    bool hasRegister(const std::string& registerName) const noexcept;


    Register& getRegisterOfSize(ASMTypeSize size) noexcept;
    Register& getRegisterOfName(const std::string& name);


private:
    static ASMTypeSize sizeIndexToASMTypeSize(const SizeIndex sizeIndex);
    static SizeIndex asmTypeSizeToSizeIndex(const ASMTypeSize size);
};





class RegisterManager
{
private:
    std::array<RegisterFamily, 8> _generalRegisters;
    RegisterFamily _returnRegister;

    std::vector<RegisterFamily*> _busyRegisters;

    bool _prioritizeReturnRegister;


public:
    RegisterManager();


    const std::array<RegisterFamily, 8>& generalRegisters() const noexcept;
    RegisterFamily& returnRegister() noexcept;

    const std::vector<RegisterFamily*>& busyRegisters() const noexcept;


    void prioritizeReturnRegister() noexcept;


    std::string print() const noexcept;


    void pushRegisterToBusy(Register& reg);

    Register& popRegisterAtIndex(size_t index);
    Register& popLastRegisterFromBusy();
    Register& popRegisterOfFamily(unsigned int familyId);

    void freeAllBusyRegisters() noexcept;


    Register& getFirstFreeRegisterOfSize(ASMTypeSize size);
};