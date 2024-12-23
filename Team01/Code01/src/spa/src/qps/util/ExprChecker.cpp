#include "ExprChecker.h"
#include <cctype>

// ai-gen start(gpt, 0, e)
// prompt: https://chat.openai.com/share/5e071a7a-5a60-46e4-ba11-1fbb629473f1
bool ExprChecker::parseExpression(const std::string& input) {
    size_t pos = 0;
    return expr(input, pos) && pos == input.length();
}

void ExprChecker::skipWhitespace(const std::string& s, size_t& pos) {
    while (pos < s.length() && std::isspace(s[pos])) {
        pos++;
    }
}

bool ExprChecker::isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%';
}

bool ExprChecker::isNameChar(char c) {
    return std::isalnum(c) || c == '_';
}

bool ExprChecker::isDigit(char c) {
    return std::isdigit(c);
}

bool ExprChecker::isInteger(const std::string& s) {
    if (s.empty()) return false;
    for (char c : s) {
        if (!std::isdigit(c)) return false;
    }
    return true;
}

bool ExprChecker::expr(const std::string& s, size_t& pos) {
    if (!term(s, pos)) return false;
    skipWhitespace(s, pos);
    while (pos < s.length() && isOperator(s[pos])) {
        pos++;
        skipWhitespace(s, pos);
        if (!term(s, pos)) return false;
        skipWhitespace(s, pos);
    }
    return true;
}

bool ExprChecker::term(const std::string& s, size_t& pos) {
    if (!factor(s, pos)) return false;
    skipWhitespace(s, pos);
    while (pos < s.length() && (s[pos] == '*' || s[pos] == '/' || s[pos] == '%')) {
        pos++;
        skipWhitespace(s, pos);
        if (!factor(s, pos)) return false;
        skipWhitespace(s, pos);
    }
    return true;
}

bool ExprChecker::factor(const std::string& s, size_t& pos) {
    skipWhitespace(s, pos);
    if (pos < s.length() && s[pos] == '(') {
        pos++;
        if (!expr(s, pos)) return false;
        skipWhitespace(s, pos);
        if (pos >= s.length() || s[pos] != ')') return false;
        pos++;
        skipWhitespace(s, pos);
        return true;
    }
    return varName(s, pos) || constValue(s, pos);
}

bool ExprChecker::varName(const std::string& s, size_t& pos) {
    if (pos < s.length() && std::isalpha(s[pos])) {
        pos++;
        while (pos < s.length() && isNameChar(s[pos])) {
            pos++;
        }
        skipWhitespace(s, pos);
        return true;
    }
    return false;
}

bool ExprChecker::constValue(const std::string& s, size_t& pos) {
    std::string num;
    while (pos < s.length() && isDigit(s[pos])) {
        num += s[pos];
        pos++;
    }
    if (!num.empty()) {
        skipWhitespace(s, pos);
        return isInteger(num);
    }
    return false;
}
// ai-gen end
