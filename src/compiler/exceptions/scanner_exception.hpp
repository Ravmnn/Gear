#pragma once

#include <compiler/language/token.hpp>
#include <compiler/exceptions/gear_exception.hpp>



struct TokenPosition;


class ScannerException : public GearException
{
protected:
    const TokenPosition m_position;


public:
    explicit ScannerException(unsigned int id, const std::string& message, const TokenPosition& position);


    const TokenPosition& position() const noexcept;


    std::string format() const noexcept override;


protected:
    std::string formatExceptionInfo() const noexcept override;
};