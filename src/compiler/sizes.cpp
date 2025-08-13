#include <compiler/sizes.hpp>

#include <compiler/assembly/assembly_generator.hpp>
#include <compiler/exceptions/exceptions.hpp>



const unsigned int SizeManager::bitsInOneByte = 8;



SizeManager::SizeManager(const BitMode bitMode) noexcept
    : _bitMode(bitMode)
{}


BitMode SizeManager::bitMode() const noexcept
{
    return _bitMode;
}



TypeSize SizeManager::defaultTypeSize() const noexcept
{
    switch (_bitMode)
    {
    case BitMode::Bits64:
    case BitMode::Bits32:
        return TypeSize::Int32;

    default:
        return TypeSize::Int16;
    }
}


ASMTypeSize SizeManager::defaultAsmTypeSize() const noexcept
{
    return (ASMTypeSize)defaultTypeSize();
}



TypeSize SizeManager::pointer() const noexcept
{
    return (TypeSize)max();
}

ASMTypeSize SizeManager::asmPointer() const noexcept
{
    return max();
}


ASMTypeSize SizeManager::max() const noexcept
{
    return (ASMTypeSize)((unsigned int)_bitMode / bitsInOneByte);
}



TypeSize SizeManager::check(const TypeSize typeSize) const
{
    if ((unsigned int)typeSize > (unsigned int)_bitMode / bitsInOneByte)
        throw internal_e0005(_bitMode);

    return typeSize;
}


ASMTypeSize SizeManager::check(const ASMTypeSize asmTypeSize) const
{
    return (ASMTypeSize)check((TypeSize)asmTypeSize);
}