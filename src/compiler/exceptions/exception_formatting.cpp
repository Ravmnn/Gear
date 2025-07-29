#include <compiler/exceptions/exception_formatting.hpp>

#include <sstream>

#include <compiler/token.hpp>
#include <gear.hpp>



std::string generateFinalExceptionMessage(const std::string& message, const TokenPosition& position) noexcept
{
    std::stringstream stream;

    stream << message << std::endl << std::endl;
    stream << generateTokenLocationHint(Gear::sourceAsLines(), position);

    return stream.str();
}


static std::string getLineBeginningOffset(const int lineNumber)
{
    std::string offset;
    int characters = std::to_string(lineNumber).size() + 2;

    for (size_t i = 0; i < characters; i++)
        offset += ' ';

    return offset;
}


std::string generateTokenLocationHint(const std::vector<std::string>& sourceLines, const TokenPosition& position) noexcept
{
    std::stringstream stream;
    const std::string stringLine = sourceLines[position.line - 1];
    const std::string hintArrow = generateTokenLocationHintArrow(position);


    stream << position.line << ". " << stringLine << std::endl;
    stream << getLineBeginningOffset(position.line) << hintArrow;

    return stream.str();
}



std::string generateTokenLocationHintArrow(const TokenPosition& position) noexcept
{
    std::stringstream stream;

    for (size_t i = 0; i < position.end; i++)
    {
        if (i == position.start)
            stream << '^';

        else if (i < position.start)
            stream << ' ';

        else if (i > position.start)
            stream << "~";
    }


    return stream.str();
}