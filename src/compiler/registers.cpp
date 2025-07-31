#include <compiler/registers.hpp>



Register::Register(const std::string& name, const ASMTypeSize size) noexcept
    : name(name), size(size)
    //, busy(false)
{}


// void Register::occupy() noexcept
// {
//     busy = true;
// }

// void Register::free() noexcept
// {
//     busy = false;
// }