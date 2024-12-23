#include "Token.h"

Token::Token(TokenType type, const std::string& value)
        : type(type), value(value) {
}