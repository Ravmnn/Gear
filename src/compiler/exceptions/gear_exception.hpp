#pragma once

#include <string>
#include <exception>



enum class ExceptionSection
{
    CommandLine,
    Scanner,
    Parser
};


class GearException : public std::exception
{
protected:
    std::string m_message;
    unsigned int m_section;
    unsigned int m_id;


public:
    explicit GearException(unsigned int section, unsigned int id, const std::string& message) noexcept;


    const std::string& message() const noexcept;
    unsigned int id() const noexcept;


    const char* what() const noexcept override;
    
    virtual std::string format() const noexcept = 0;


protected:
    std::string formatExceptionInfo() const noexcept;
};