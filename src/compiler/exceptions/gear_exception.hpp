#pragma once

#include <string>
#include <exception>



class GearException : public std::exception
{
private:
    std::string _message;


public:
    explicit GearException(const std::string& message) noexcept;


    const char* what() const noexcept override;
};