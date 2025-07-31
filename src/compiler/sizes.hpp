#pragma once



enum class TypeSize
{
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