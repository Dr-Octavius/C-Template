#ifndef SPA_TOKEN_H
#define SPA_TOKEN_H

#include <string>

enum class TokenType {
    KEYWORD,
    ARITHMETIC_OPERATOR,
    RELATIONAL_OPERATOR,
    CONDITIONAL_OPERATOR,
    PUNCTUATION,
    IDENTIFIER,
    CONSTANT,
    ASSIGNMENT_OPERATOR
};

class Token {
public:
    TokenType type;
    std::string value;

    Token(TokenType type, const std::string& value);
};

#endif //SPA_TOKEN_H
