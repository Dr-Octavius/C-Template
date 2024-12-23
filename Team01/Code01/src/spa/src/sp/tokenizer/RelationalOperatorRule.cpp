#include "RelationalOperatorRule.h"

bool RelationalOperatorRule::match(const std::string& input, size_t position, std::vector<Token>& tokens) {
    if (position + 1 < input.size()) {
        std::string value = input.substr(position, 2);

        if (value == "==" || value == "!=" || value == ">=" || value == "<=") {
            tokens.push_back(createToken(value));
            return true;
        }
    }

    if (position < input.size()) {
        char character = input[position];
        std::string value(1, character);

        if (character == '>' || character == '<') {
            tokens.push_back(createToken(value));
            return true;
        }
    }

    return false;
}

Token RelationalOperatorRule::createToken(const std::string& value) {
    return Token(TokenType::RELATIONAL_OPERATOR, value);
}
