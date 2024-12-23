#include "PunctuationRule.h"

bool PunctuationRule::match(const std::string& input, size_t position, std::vector<Token>& tokens) {
    if (position < input.size()) {
        char character = input[position];
        std::string value(1, character);

        switch (character) {
            case ';':
            case '(':
            case ')':
            case '{':
            case '}':
                tokens.push_back(createToken(value));
                return true;
            default:
                return false;
        }
    }
    return false; // No match at the current position
}

Token PunctuationRule::createToken(const std::string& value) {
    return Token(TokenType::PUNCTUATION, value);
}
