#pragma once

#include <compiler/exceptions/gear_exception.hpp>



struct TokenPosition;


class ScannerException : public GearException
{
private:
    const TokenPosition& _position;


public:
    explicit ScannerException(const std::string& message, const TokenPosition& position);


    const char* what() const noexcept override;
};



class InvalidTokenException : public ScannerException
{
public:
    explicit InvalidTokenException(const TokenPosition& position);
};