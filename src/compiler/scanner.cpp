#include <compiler/scanner.hpp>

#include <compiler/token.hpp>



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
        scanToken();
    }

    return _tokens;
}


void Scanner::scanToken()
{
    const char current = advance();

    switch (current)
    {
    case '@': addToken(TokenType::Attribute); break;
    case ':': addToken(TokenType::Colon); break;
    case '-': addToken((advance() == '>' ? TokenType::Arrow : TokenType::Minus)); break;
    case '+': addToken(TokenType::Plus); break;
    case '*': addToken(TokenType::Star); break;
    case '/': addToken(TokenType::Slash); break;
    case '=': addToken(TokenType::Equal); break;
    case '(': addToken(TokenType::ParenLeft); break;
    case ')': addToken(TokenType::ParenRight); break;

    case '#':
        if (peek() == '>')
            multilineComment();
        else
            comment();

        break;

    default:
        if (isalpha(current))
            identifier();

        else if (isdigit(current))
            value();
    };
}


void Scanner::addToken(const TokenType type) noexcept
{
    _tokens.push_back(Token{ getSourceCurrentSubstring(), _start, _end, type });
}


std::string Scanner::getSourceCurrentSubstring() const noexcept
{
    return _source.substr(_start, _end - _start);
}



void Scanner::reset() noexcept
{
    _start = _end = 0;
    _tokens.clear();
}



void Scanner::identifier() noexcept
{
    while (isalnum(peek()))
        advance();

    const std::string currentSubstring = getSourceCurrentSubstring();

    if (Token::isKeyword(currentSubstring))
        addToken(Token::keywords.at(currentSubstring));
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


void Scanner::multilineComment() noexcept
{
    advance(); // advance ">" from "#>"

    while ((peek() != '<' || peekNext() != '#') && !atEnd())
        advance();

    advance(); // advance '<'
    advance(); // advance '#'
}



char Scanner::advance() noexcept
{
    return !atEnd() ? _source[_end++] : '\0';
}


char Scanner::peek() const noexcept
{
    return _source[_end];
}


char Scanner::peekNext() const noexcept
{
    return !atEnd() ? _source[_end + 1] : '\0';
}


bool Scanner::atEnd() const noexcept
{
    return _end >= _source.size();
}