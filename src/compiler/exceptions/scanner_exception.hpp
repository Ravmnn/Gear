#pragma once

#include <compiler/exceptions/locatable_exception.hpp>



struct TokenPosition;


class ScannerException : public LocatableException
{
public:
    explicit ScannerException(unsigned int id, const std::string& message, const TokenPosition& position) noexcept;
};