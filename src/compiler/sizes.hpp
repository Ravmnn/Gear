#pragma once



enum class TypeSize
{
    None = 0,

    Byte = 1,
    Char = 1,
    Bool = 1,

    UInt8 = 1,
    UInt16 = 2,
    UInt32 = 4,
    UInt64 = 8,

    Int8 = 1,
    Int16 = 2,
    Int32 = 4,
    Int64 = 8
};


enum class ASMTypeSize
{
    Byte = 1,
    Word = 2,
    DWord = 4,
    QWord = 8
};


enum class BitMode;


class SizeManager
{
private:
    const BitMode _bitMode;


public:
    static const unsigned int bitsInOneByte;


    explicit SizeManager(BitMode bitMode) noexcept;


    BitMode bitMode() const noexcept;


    TypeSize defaultTypeSize() const noexcept;
    ASMTypeSize defaultAsmTypeSize() const noexcept;

    TypeSize pointer() const noexcept;
    ASMTypeSize asmPointer() const noexcept;

    ASMTypeSize max() const noexcept;


    TypeSize check(TypeSize typeSize) const;
    ASMTypeSize check(ASMTypeSize asmTypeSize) const;
};