#pragma once

#include <string>



struct TokenPosition;


std::string generateTokenLocationHint(const std::string& source, const TokenPosition& position);