#include <compiler/scanner.hpp>

#include <compiler/token.hpp>
#include <compiler/exceptions/exceptions.hpp>

#include <iostream>



Scanner::Scanner(const std::string& source) noexcept
    : _source(source)
{
    reset();
}



std::vector<Token> Scanner::scan()
{
    reset();

    while (!atEnd())
    {
        _start = _end;
        _positionInFile.start = _positionInFile.end;
        scanToken();
    }

    return _tokens;
}


void Scanner::scanToken()
{
    const char current = advance();

    switch (current)
    {
    case ' ':
    case '\t':
    case '\n':
        break;

    case '@': addToken(TokenType::Attribute); break;
    case ':': addToken(TokenType::Colon); break;
    case ';': addToken(TokenType::SemiColon); break;
    case '-': addToken((match('>') ? TokenType::Arrow : TokenType::Minus)); break;
    case '+': addToken(TokenType::Plus); break;
    case '*': addToken(TokenType::Star); break;
    case '/': addToken(TokenType::Slash); break;
    case '=': addToken(TokenType::Equal); break;
    case '(': addToken(TokenType::ParenLeft); break;
    case ')': addToken(TokenType::ParenRight); break;

    case '#':
        if (match('>'))
            multilineComment();
        else
            comment();

        break;

    default:
        if (isalpha(current))
            identifier();

        else if (isdigit(current))
            value();

        else
            throw gear_e1000(_positionInFile);
    };
}


void Scanner::addToken(const TokenType type) noexcept
{
    _tokens.push_back(Token{ getSourceCurrentSubstring(), _positionInFile, type });
}


std::string Scanner::getSourceCurrentSubstring() const noexcept
{
    return _source.substr(_start, _end - _start);
}



void Scanner::reset() noexcept
{
    _start = _end = 0;
    _positionInFile = TokenPosition{ .start = 0, .end = 0, .line = 1 };
    _tokens.clear();
}


void Scanner::nextLine() noexcept
{
    _positionInFile.start = _positionInFile.end = 0;
    _positionInFile.line++;
}



void Scanner::identifier() noexcept
{
    while (isalnum(peek()))
        advance();

    const std::string currentSubstring = getSourceCurrentSubstring();

    if (Token::isKeyword(currentSubstring))
        addToken(Token::keywords.at(currentSubstring));

    else if (Token::isType(currentSubstring))
        addToken(TokenType::Type);

    else if (Token::isBoolean(currentSubstring))
        addToken(TokenType::Value);

    else
        addToken(TokenType::Identifier);
}


void Scanner::value() noexcept
{
    while (isdigit(peek()))
        advance();

    addToken(TokenType::Value);
}



void Scanner::comment() noexcept
{
    while (peek() != '\n' && !atEnd())
        advance();
}


void Scanner::multilineComment()
{
    const TokenPosition startPosition = _positionInFile;

    while ((peek() != '<' || peekNext() != '#') && !atEnd())
        advance();

    if (atEnd())
        throw gear_e1001(startPosition);

    advance(); // advance '<'
    advance(); // advance '#'
}



char Scanner::advance() noexcept
{
    if (!atEnd())
    {
        _positionInFile.end++;

        if (peek() == '\n')
            nextLine();

        return _source[_end++];
    }

    return '\0';
}


char Scanner::peek() const noexcept
{
    return _source[_end];
}


char Scanner::peekNext() const noexcept
{
    return !atEnd() ? _source[_end + 1] : '\0';
}


bool Scanner::match(const char ch) noexcept
{
    if (peek() == ch)
    {
        advance();
        return true;
    }

    return false;
}


bool Scanner::atEnd() const noexcept
{
    return _end >= _source.size();
}