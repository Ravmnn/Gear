#pragma once

#include <string>
#include <vector>



struct TokenPosition;


std::string generateFinalExceptionMessage(const std::string& message, const TokenPosition& position) noexcept;
std::string generateTokenLocationHint(const std::vector<std::string>& sourceLines, const TokenPosition& position) noexcept;
std::string generateTokenLocationHintArrow(const TokenPosition& position) noexcept;