#include "IdentifierRule.h"

bool IdentifierRule::match(const std::string& input, size_t position, std::vector<Token>& tokens) {
    size_t start = position;

    if (position < input.size() && isalpha(input[position]) ) {
        ++position;

        while (position < input.size() && (isalpha(input[position]) || isdigit(input[position]))) {
            ++position;
        }

        std::string identifier = input.substr(start, position - start);
        tokens.push_back(createToken(identifier));
        return true;
    }

    return false;
}

Token IdentifierRule::createToken(const std::string& value) {
    return Token(TokenType::IDENTIFIER, value);
}
