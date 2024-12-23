#include "NumberRule.h"

bool NumberRule::match(const std::string& input, size_t position, std::vector<Token>& tokens) {
    size_t start = position;
    bool posWithinLimit = (position + 1) < input.size();
    if (input[position] == '0' && posWithinLimit && isdigit(input[position+1])) {
        return false;
    }

    while (position < input.size() && isdigit(input[position])) {
        ++position;
    }

    if (position > start && (position+1) < input.size()) {
        if (isalpha(input[position])) {
            throw SimpleSyntaxError("Variable name cannot start with a number: " + std::to_string(position));
        }
    }

    if (start < position) {
        std::string numberString = input.substr(start, position - start);
        tokens.push_back(Token(createToken(numberString)));
        return true;
    }

    return false;
}

Token NumberRule::createToken(const std::string& value) {
    return Token(TokenType::CONSTANT, value);
}
