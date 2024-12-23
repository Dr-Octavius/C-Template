#pragma once

#include <string>

// ai-gen start(gpt, 0, e)
class ExprChecker {
public:
    static bool parseExpression(const std::string& input);

private:
    static void skipWhitespace(const std::string& s, size_t& pos);
    static bool isOperator(char c);
    static bool isNameChar(char c);
    static bool isDigit(char c);
    static bool isInteger(const std::string& s);
    static bool expr(const std::string& s, size_t& pos);
    static bool term(const std::string& s, size_t& pos);
    static bool factor(const std::string& s, size_t& pos);
    static bool varName(const std::string& s, size_t& pos);
    static bool constValue(const std::string& s, size_t& pos);
};
// ai-gen end