#include "AssignmentOperatorRule.h"

bool AssignmentOperatorRule::match(const std::string& input, size_t position, std::vector<Token>& tokens) {
    if (position < input.size() && input[position] == '=') {
        tokens.push_back(createToken("="));
        return true;
    }
    return false;
}

Token AssignmentOperatorRule::createToken(const std::string& value) {
    return Token(TokenType::ASSIGNMENT_OPERATOR, value);
}
