#pragma once

#include <compiler/exceptions/torque_exception.hpp>



class InternalException : public TorqueException
{
public:
    InternalException(unsigned int code, const std::string& message) noexcept;


    std::string format() const noexcept override;


protected:
    std::string formatExceptionInfo() const noexcept override;
};