#include "KeywordRule.h"

KeywordRule::KeywordRule() {
    keywords.insert("if");
    keywords.insert("while");
    keywords.insert("then");
    keywords.insert("else");
    keywords.insert("procedure");
    keywords.insert("call");
    keywords.insert("read");
    keywords.insert("print");
}

bool KeywordRule::match(const std::string& input, size_t position, std::vector<Token>& tokens) {
    for (const auto &keyword: keywords) {
        if (input.compare(position, keyword.length(), keyword) == 0) {
            size_t nextPos = position + keyword.length();
            if (nextPos == input.length() || input[nextPos] == '(' || input[nextPos] == '{' || isspace(input[nextPos])) {
                tokens.push_back(createToken(keyword));
                return true;
            }
        }
    }
    return false;
}

Token KeywordRule::createToken(const std::string& value) {
    return Token(TokenType::KEYWORD, value);
}